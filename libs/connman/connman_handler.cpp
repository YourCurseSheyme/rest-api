//
// Created by sheyme on 09/07/25.
//

#include "connman_handler.hpp"

std::string ConnmanHandler::GetServiceName() {
  const auto& lines = HandleLines("connmanctl services");
  std::regex service_regex(
      R"((wifi|ethernet)_[a-zA-Z0-9_]+(?:_managed_\w+)?)");
  for (const auto& line : lines) {
    std::smatch match;
    if (std::regex_search(line, match, service_regex)) {
      return match[0].str();
    }
  }
  return "";
}

void ConnmanHandler::ApplyConfiguration(const NetConfig& config) {
  if (!IsServiceActive()) {
    throw std::runtime_error("Connman isn't available");
  }
  const auto* ip_config = dynamic_cast<const IpConfig*>(&config);
  const std::string& service = GetServiceName();
  if (service.empty()) {
    throw std::runtime_error("Incorrect interface");
  }
  std::string command = "connmanctl config " + service;
  if (ip_config == nullptr) {
    command += " --ipv4 dhcp";
  } else {
    if (!ip_config->ValidateConfig()) {
      throw std::runtime_error("Invalid ip config");
    }
    command += " --ipv4 manual " + ip_config->ip + " " + ip_config->netmask
        + " " + ip_config->gateway;
  }
  Execute(command);
}

bool ConnmanHandler::IsDhcpEnabled() {
  const auto& service = GetServiceName();
  if (service.empty()) {
    throw std::runtime_error("Interface not found in connman services");
  }
  std::string command = "connmanctl services " + service;
  std::string output;
  Execute(command, &output);
  std::istringstream iss(output);
  std::string line;
  while (std::getline(iss, line)) {
    if (line.find("IPv4") != std::string::npos &&
        line.find("Method") != std::string::npos &&
        line.find("dhcp") != std::string::npos) {
      return true;
    }
  }
  return false;
}
