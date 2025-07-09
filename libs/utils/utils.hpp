//
// Created by sheyme on 09/07/25.
//

#pragma once

#include <sstream>
#include <algorithm>
#include <utility>
#include <memory>
#include <vector>
#include <string>

struct NetConfig {
  virtual ~NetConfig() = default;

  std::string interface;
};

struct IpConfig : public NetConfig {
  [[nodiscard]] inline bool ValidateConfig() const {
    if (interface.empty()) {
      return false;
    }
    return !ip.empty() && !netmask.empty() && !gateway.empty();
  }

  std::string ip;
  std::string netmask;
  std::string gateway;
};

class CommandExecutor {
 protected:
  inline static void Execute(std::string_view command,
                             std::string* output = nullptr) {
    std::unique_ptr<FILE, decltype(&pclose)> pipe(
        popen(command.data(), "r"), pclose);
    if (!pipe) {
      throw std::runtime_error("Error during command execution");
    }
    std::ostringstream result;
    char buffer[kBufferSize];
    while (fgets(buffer, sizeof(buffer), pipe.get())) {
      result << buffer;
    }
    if (output != nullptr) {
      *output = result.str();
      if (!output->empty() && output->back() == '\n') {
        output->pop_back();
      }
    }
  }

  inline static std::vector<std::string>
  HandleLines(std::string_view command) {
    std::string output;
    Execute(command, &output);
    std::vector<std::string> lines;
    std::istringstream istringstream(output);
    std::string line;
    while (std::getline(istringstream, line)) {
      line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
      if (!line.empty()) {
        lines.push_back(std::move(line));
      }
    }
    return lines;
  }

 private:
  inline static constexpr size_t kBufferSize = 4096;
};


class InterfaceHandler : public CommandExecutor {
 public:
  virtual void ApplyConfiguration(const NetConfig& config) = 0;

  static inline IpConfig GetConfiguration(const std::string& profile) {
    IpConfig actual_state;
    actual_state.interface = profile;
    std::string ip_output;
    Execute("ip -o -4 addr show " + profile + " | awk '{print $4}'",
            &ip_output);
    if (!ip_output.empty()) {
      size_t slash_pos = ip_output.find('/');
      actual_state.ip = ip_output.substr(0, slash_pos);
      actual_state.netmask = ip_output.substr(slash_pos + 1);
    }
    Execute("ip route | awk '/default/ && /" + profile + "/ {print $3}'",
            &actual_state.gateway);
    return actual_state;
  }

  virtual void StartService() = 0;

  virtual void StopService() = 0;

  [[nodiscard]] virtual bool IsServiceActive() const = 0;

  virtual ~InterfaceHandler() = default;
};
