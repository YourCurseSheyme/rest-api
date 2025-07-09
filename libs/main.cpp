//
// Created by sheyme on 03/07/25.
//

#include <iostream>
#include <thread>
#include <csignal>
#include <atomic>

#include "connman_handler.hpp"

using IpConfig = ConnmanHandler::IpConfig;

const char kInterface[] = "wifi";
const char kProfile[] = "wlp0s20f3";
const IpConfig original_config = {
    kInterface,
    "192.168.30.35",
    "255.255.255.0",
    "192.168.30.1",
    false
};

void ConnmanTest() {
  ConnmanHandler handler(kInterface);
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
    handler.SetConfiguration(dhcp_config);
    std::cout << handler.GetConfiguration(kProfile) << '\n';
    std::cout << "+-------------------------\n";
//    handler.SetConfiguration(original_config);
    std::cout << handler.GetConfiguration(kProfile) << '\n';
    std::cout << "+-------------------------\n";
  } catch (const std::exception& exception) {
    std::cerr << "Error: " << exception.what() << std::endl;
  }

  std::cout << "Success </3\n\n";
}

#include "rest_handler.hpp"

std::atomic<bool> running(true);

void HandleSigInt(int signal) {
  if (signal == SIGINT) {
    std::cout << "\nReceived Ctrl+C\n";
    running = false;
  }
}

void RestTest() {
  RestHandler rest_handler;
  std::signal(SIGINT, HandleSigInt);
  std::thread server_thread([&rest_handler](){
    rest_handler.Run();
  });
  while (running) {
    std::string command;
    std::cout << "> ";
    std::getline(std::cin, command);
    if (command == "stop") {
      running = false;
      break;
    }
  }
  std::cout << "Stopping server...\n";
  rest_handler.Stop();
  if (server_thread.joinable()) {
    server_thread.join();
  }
  std::cout << "Stopped\n";
}

int main() {
  RestTest();
}
