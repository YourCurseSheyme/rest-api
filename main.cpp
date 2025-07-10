//
// Created by sheyme on 03/07/25.
//

#include <iostream>
#include <thread>
#include <csignal>
#include <atomic>

#include "connman_handler.hpp"
#include "rest_handler.hpp"

const char kInterface[] = "wifi";
const char kProfile[] = "wlp0s20f3";
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
  try {
    RestTest();
  } catch (const std::exception& exception) {
    std::cerr << exception.what() << std::endl;
    return 1;
  }

  std::cout << "+---------------------------\n";
  std::cout << "tests done\n";
}
