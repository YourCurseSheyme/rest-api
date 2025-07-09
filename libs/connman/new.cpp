//
// Created by sheyme on 09/07/25.
//

#include "new.hpp"

std::string ConnmanHandler::GetServiceName(const std::string& interface) {
  const auto& lines = HandleLines("connmanctl services");
  std::regex service_regex(
      R"((wifi|ethernet)_[a-zA-Z0-9_]+(?:_managed_\w+)?)");
  for (const auto& line : lines) {
    std::smatch match;
    if (std::regex_search(line, match, service_regex)) {
      std::string service = match[0].str();
      if (service.find(interface) != std::string::npos) {
        return service;
      }
    }
  }
  return "";
}

void ConnmanHandler::ApplyConfiguration(const NetConfig& config) {
  const auto* ip_config = dynamic_cast<const IpConfig*>(&config);
  if (!ip_config) {
    throw std::invalid_argument("Invalid config type");
  }
  if (!ip_config->ValidateConfig()) {
    throw std::invalid_argument("Invalid ip config");
  }
  const auto& service = GetServiceName(config.interface);
  if (service.empty()) {
    throw std::runtime_error("Incorrect interface");
  }
  std::string command = "connmanctl config " + service;
  command += " --ipv4 manual " + ip_config->ip + " " + ip_config->netmask
      + " " + ip_config->gateway;
  std::string output;
  Execute(command, &output);
  if (output.find("error") != std::string::npos) {
    throw std::runtime_error("Error while configuring interface");
  }
}
