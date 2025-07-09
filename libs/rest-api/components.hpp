//
// Created by sheyme on 08/07/25.
//

#pragma once

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/mime/ContentMappers.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/json/ObjectMapper.hpp"
#include "oatpp/macro/component.hpp"

class Components {
 public:
  OATPP_CREATE_COMPONENT(std::shared_ptr<
      oatpp::network::ServerConnectionProvider>,
      server_connection_provider)([]{
    return oatpp::network::tcp::server::ConnectionProvider::createShared({
      "0.0.0.0",
      8000,
      oatpp::network::Address::IP_4
    });
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
      http_router)([]{
    return oatpp::web::server::HttpRouter::createShared();
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
      server_connection_handler)([]{
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    return oatpp::web::server::HttpConnectionHandler::createShared(router);
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>,
      api_content_mappers)([]{
    auto json = std::make_shared<oatpp::json::ObjectMapper>();
    json->serializerConfig().json.useBeautifier = true;
    auto mappers = std::make_shared<oatpp::web::mime::ContentMappers>();
    mappers->putMapper(json);
    return mappers;
  }());
};
