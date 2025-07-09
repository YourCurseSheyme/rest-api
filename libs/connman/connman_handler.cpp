//
// Created by sheyme on 07/07/25.
//

#include "connman_handler.hpp"

std::ostream& operator<<(std::ostream& ostream,
    const ConnmanHandler::IpConfig& config) {
  ostream << config.interface << ": " << config.ip << '/' << config.netmask;
  ostream << ' ' << config.gateway << ' ';
  ostream << (config.is_dhcp ? "dhcp" : "non-dhcp");
  return ostream;
}

std::string ConnmanHandler::ExecuteCommand(const std::string& command) {
  std::string result;
  result.reserve(kBufferSize);
  std::unique_ptr<FILE, decltype(&pclose)> pipe(
      popen(command.c_str(), "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("Error during command execution");
  }
  char buffer[kBufferSize];
  while (fgets(buffer, sizeof(buffer), pipe.get())) {
    result += buffer;
  }
  result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
  return result;
}

std::vector<std::string>
ConnmanHandler::ExecuteLines(const std::string& command) {
  const auto& output = ExecuteCommand(command);
  std::vector<std::string> lines;
  std::istringstream istringstream(output);
  std::string line;
  while (std::getline(istringstream, line)) {
    if (!line.empty()) {
      line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
      lines.push_back(line);
    }
  }
  return lines;
}

std::string ConnmanHandler::GetServiceName() {
  if (!cached_service_name_.empty()) {
    return cached_service_name_;
  }
  const auto& lines = ExecuteLines("connmanctl services");
  std::regex service_regex(
      R"((wifi|ethernet)_[a-zA-Z0-9_]+(?:_managed_\w+)?)");
  for (const auto& line : lines) {
    std::smatch match;
    if (std::regex_search(line, match, service_regex)) {
      std::string service = match[0].str();
      size_t invalid_char = service.find_first_not_of(
          "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");
      if (invalid_char != std::string::npos) {
        service = service.substr(0, invalid_char);
      }
      cached_service_name_ = service;
      return service;
    }
  }
  return "";
}

ConnmanHandler::IpConfig
ConnmanHandler::GetConfiguration(const std::string& profile) {
  IpConfig actual_state = { profile };
  const auto& ip_output = ExecuteCommand("ip -o -4 addr show "
      + profile + " | awk '{print $4}'");
  if (!ip_output.empty()) {
    size_t slash_pos = ip_output.find('/');
    actual_state.ip = ip_output.substr(0, slash_pos);
    actual_state.netmask = ip_output.substr(slash_pos + 1);
  }
  actual_state.gateway = ExecuteCommand("ip route | awk '/default/ && /"
      + profile + "/ {print $3}'");
  const auto& service = GetServiceName();
  if (!service.empty()) {
    const auto& config_ouput = ExecuteCommand("connmanctl services "
        + service);
    if (config_ouput.find("IPv4 =") != std::string::npos) {
      actual_state.is_dhcp =
          (config_ouput.find("Method=dhcp") != std::string::npos);
    }
  }
  return actual_state;
}

void
ConnmanHandler::SetConfiguration(const IpConfig& config) {
  if (!IsConnmanAvailable()) {
    throw std::runtime_error("Connman not available");
  }
  if (!config.ValidateConfig()) {
    throw std::invalid_argument("Invalid ip config");
  }
  const auto& service = GetServiceName();
  if (service.empty()) {
    throw std::runtime_error("Incorrect interface");
  }
  std::string command = "connmanctl config " + service;
  if (config.is_dhcp) {
    command += " --ipv4 dhcp";
  } else {
    command += " --ipv4 manual " + config.ip + " " + config.netmask
        + " " + config.gateway;
  }
  if (ExecuteCommand(command).find("error") != std::string::npos) {
    throw std::runtime_error("Error while configuring interface");
  }
//  ApplyConfig();
}

void ConnmanHandler::DisconnectFromService() {
  std::string command = "connmanctl disconnect " + cached_service_name_;
  auto result = ExecuteCommand(command);
  if (result.find("Error") != std::string::npos) {
    throw std::runtime_error("Error while disconnecting");
  }
}

void ConnmanHandler::ConnectToService() {
  std::string command = "connmanctl connect " + cached_service_name_;
  auto result = ExecuteCommand(command);
  if (result.find("Error") != std::string::npos) {
    throw std::runtime_error("Error while connecting");
  }
}

void ConnmanHandler::ApplyConfig() {
  if (cached_service_name_.empty()) {
    throw std::runtime_error("Net hasn't been configured");
  }
  DisconnectFromService();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  ConnectToService();
  std::this_thread::sleep_for(std::chrono::seconds(2));
}