//
// Created by sheyme on 07/07/25.
//

#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <memory>
#include <string>
#include <vector>
#include <regex>

class ConnmanHandler {
 public:
  struct IpConfig {
    [[nodiscard]] inline bool ValidateConfig() const {
      if (interface.empty()) {
        return false;
      }
      if (is_dhcp) {
        return true;
      }
      return !ip.empty() && !netmask.empty() && !gateway.empty();
    }

    friend std::ostream& operator<<(std::ostream& ostream,
        const IpConfig& config);

    std::string interface;
    std::string ip;
    std::string netmask;
    std::string gateway;
    bool is_dhcp{false};
  };

  ConnmanHandler() = default;

  explicit ConnmanHandler(const std::string& interface);

  [[nodiscard]] inline std::string GetInterface() const {
    return interface_;
  }

  inline void SetInterface(const std::string& interface) {
    interface_ = interface;
    cached_service_name_.clear();
  }

  [[nodiscard]] IpConfig GetConfiguration(const std::string& profile);

  void SetConfiguration(const IpConfig& config);

  ~ConnmanHandler() = default;

 private:
  [[nodiscard]] inline static bool IsConnmanAvailable() {
    return system("which connmanctl > /dev/null 2>&1") == 0;
  }

  std::string GetServiceName();

  void DisconnectFromService();
  void ConnectToService();
  void ApplyConfig();

  static std::string ExecuteCommand(const std::string& command);
  static std::vector<std::string> ExecuteLines(const std::string& command);

  inline static constexpr size_t kBufferSize = 4096;
  std::string interface_;
  std::string cached_service_name_;
};
