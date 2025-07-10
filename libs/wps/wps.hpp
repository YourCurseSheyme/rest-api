//
// Created by sheyme on 09/07/25.
//

#pragma once

#include "utils.hpp"

class WpsHandler : public InterfaceHandler {
 public:
  WpsHandler() = default;

  inline void ApplyConfiguration(const NetConfig& config) override {
    if (IsServiceActive()) {
      StopService();
    }
    StartService(config.interface);
  }

  inline void StartService(std::string_view interface = "") override {
    std::string prompt = "wpa_supplicant -i ";
    prompt += interface;
    prompt += " -D wired -c /etc/wpa_supplicant.conf";
    Execute(prompt);
  }

  inline void StopService() override {
    Execute("killall wpa_supplicant");
  }

  [[nodiscard]] inline bool IsServiceActive() const override {
    std::string output;
    Execute("pgrep wpa_supplicant", &output);
    return !output.empty();
  }
};