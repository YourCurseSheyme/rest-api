//
// Created by sheyme on 03/07/25.
//

#include <iostream>

#include "new.hpp"

const char kInterface[] = "wifi";
const char kProfile[] = "wlp0s20f3"; /*
const IpConfig original_config = {
    kInterface,
    "192.168.30.35",
    "255.255.255.0",
    "192.168.30.1",
    false
};

void ConnmanTest() {
  ConnmanHandler handler;
  const IpConfig test_config = {
      kInterface,
      "192.168.30.36",
      "255.255.255.0",
      "192.168.30.1",
      false
  };
  const IpConfig dhcp_config {
      kInterface,
      "",
      "",
      "",
      true
  };
  try {
    std::cout << handler.GetConfiguration(kProfile) << '\n';
    std::cout << "+-------------------------\n";
    handler.SetConfiguration(test_config);
    std::cout << handler.GetConfiguration(kProfile) << '\n';
    std::cout << "+-------------------------\n";
    handler.SetConfiguration(original_config);
    std::cout << handler.GetConfiguration(kProfile) << '\n';
    std::cout << "+-------------------------\n";
  } catch (const std::exception& exception) {
    std::cerr << "Error: " << exception.what() << std::endl;
  }

  std::cout << "Success </3\n\n";
} */

#include "rest_handler.hpp"

void RestTest() {
  RestHandler rest_handler;
  rest_handler.Run();
}

int main() {
  ConnmanHandler handler_;

  try {
    RestTest();
  } catch (const std::exception& exception) {
    std::cerr << exception.what() << std::endl;
    return 1;
  }

  std::cout << "done\n";
}
