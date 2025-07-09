//
// Created by sheyme on 08/07/25.
//

#pragma once

#include "oatpp/network/Server.hpp"

#include "controller/controller.hpp"
#include "components.hpp"

class RestHandler {
 public:
  RestHandler();

  void Run();

  void Stop();

  ~RestHandler() = default;

 private:
  std::shared_ptr<oatpp::network::Server> server_;
  std::shared_ptr<Components> components_;
};
