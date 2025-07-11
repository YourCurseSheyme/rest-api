//
// Created by sheyme on 08/07/25.
//

#pragma once

#include <optional>
#include <memory>

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/web/mime/ContentMappers.hpp"
#include "oatpp/macro/codegen.hpp"

#include "connman_handler.hpp"

#include "dto/dtos.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

class ConnmanController : public oatpp::web::server::api::ApiController,
    std::enable_shared_from_this<ConnmanController> {
 public:
  inline static std::shared_ptr<ConnmanController>
  createShared(const std::shared_ptr<
      oatpp::web::mime::ContentMappers>& api_content_mappers) {
    return std::make_shared<ConnmanController>(api_content_mappers);
  }

  inline explicit ConnmanController(const std::shared_ptr<
      oatpp::web::mime::ContentMappers>& api_content_mappers)
      : oatpp::web::server::api::ApiController(api_content_mappers) {
    handler_ = std::make_unique<ConnmanHandler>();
  }

 private:
  ENDPOINT("POST", "/connman/set/dhcp", setDhcpIp,
           BODY_DTO(oatpp::Object<DhcpIpConfigDTO>, config)) {
    auto&& is_applied = ApplyDhcp(config);
    if (!is_applied) {
      auto response = StatusDTO::createShared();
      response->message = "Changed IP to dynamic successfully";
      return createDtoResponse(Status::CODE_200, std::move(response));
    }
    auto error = ErrorDTO::createShared();
    error->error = is_applied.value();
    return createDtoResponse(Status::CODE_500, std::move(error));
  }

  ENDPOINT("POST", "/connman/set/static", setNonDhcpIp,
           BODY_DTO(oatpp::Object<NonDhcpIpConfigDTO>, config)) {
    auto&& is_applied = ApplyStatic(config);
    if (!is_applied) {
      auto response = StatusDTO::createShared();
      response->message = "Changed IP to static successfully";
      return createDtoResponse(Status::CODE_200, std::move(response));
    }
    auto error = ErrorDTO::createShared();
    error->error = is_applied.value();
    return createDtoResponse(Status::CODE_500, std::move(error));
  }

  ENDPOINT("GET", "/connman/get/{profile}", getIpConfig,
           PATH(String, profile)) {
    try {
      const auto& config
          = handler_->GetConfiguration(profile.operator std::string());  // Удел сильных -> operator std::string()
      auto response = NonDhcpIpConfigDTO::createShared();
      response->interface = profile;
      response->ip = config.ip;
      response->netmask = config.netmask;
      response->gateway = config.gateway;
      response->is_dhcp = config.is_dhcp;
      return createDtoResponse(Status::CODE_200, std::move(response));
    } catch (const std::runtime_error& exception) {
      auto error = ErrorDTO::createShared();
      error->error = "Runtime error: ";
      error->error->append(exception.what());
      return createDtoResponse(Status::CODE_500, std::move(error));
    }
  }

  std::optional<std::string>
  ApplyDhcp(const oatpp::Object<DhcpIpConfigDTO>& query);

  std::optional<std::string>
  ApplyStatic(const oatpp::Object<NonDhcpIpConfigDTO>& query);

  std::optional<std::string> TryApply(const ConnmanHandler::IpConfig& config);

  std::unique_ptr<ConnmanHandler> handler_;
};

#include OATPP_CODEGEN_END(ApiController)
