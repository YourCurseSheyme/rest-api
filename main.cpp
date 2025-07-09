//
// Created by sheyme on 03/07/25.
//

#include <iostream>

#include "connman_handler.hpp"
#include "rest_handler.hpp"

const char kInterface[] = "wifi";
const char kProfile[] = "wlp0s20f3";

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
