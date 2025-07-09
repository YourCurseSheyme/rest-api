//
// Created by sheyme on 09/07/25.
//

#pragma once

#include <regex>

#include "utils.hpp"

class ConnmanHandler : public InterfaceHandler {
 public:
  ConnmanHandler() = default;

  void ApplyConfiguration(const NetConfig& config) override;

  inline void StartService() override {
    Execute("systemctl start connman");
  }

  inline void StopService() override {
    Execute("systemctl stop connman");
  }

  [[nodiscard]] inline bool IsServiceActive() const override {
    std::string output;
    Execute("systemctl is-active connman", &output);
    return output == "active";
  }

 private:
  static std::string GetServiceName(const std::string& interface);
};
