//
// Created by sheyme on 08/07/25.
//

#include "rest_handler.hpp"

RestHandler::RestHandler() {
  components_ = std::make_shared<Components>();
  auto router = components_->http_router.getObject();
  auto mappers = components_->api_content_mappers.getObject();
  auto controller = ConnmanController::createShared(mappers);
  router->addController(controller);
  server_ = std::make_shared<oatpp::network::Server>(
      components_->server_connection_provider.getObject(),
      components_->server_connection_handler.getObject()
  );
}

void RestHandler::Run() {
  server_->run();
}

void RestHandler::Stop() {
  server_->stop();
}