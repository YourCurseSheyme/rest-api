//
// Created by sheyme on 08/07/25.
//

#include "controller.hpp"

ConnmanController::ConnmanController(const std::shared_ptr<
    oatpp::web::mime::ContentMappers>& api_content_mappers)
    : oatpp::web::server::api::ApiController(api_content_mappers) {
  connman_ = std::make_unique<ConnmanHandler>();
  is_dhcp_ = !connman_->IsServiceActive();
}

//std::optional<std::string> ConnmanController::ApplyDhcp(const oatpp::Object<
//    DhcpIpConfigDTO>& query) {
//  NetConfig config;
//  config.interface = query->interface;
//  return TryApply(config);
//}

std::optional<std::string>
ConnmanController::ApplyStatic(const oatpp::Object<
    NonDhcpIpConfigDTO>& query) {
  IpConfig config;
  config.interface = query->interface;
  config.ip = query->ip;
  config.netmask = query->netmask;
  config.gateway = query->gateway;
  return TryApply(config);
}

std::optional<std::string>
ConnmanController::TryApply(const NetConfig& config) {
  try {
    if (is_dhcp_) {
      // TODO: process wpa handler
       return std::nullopt;
    }
    connman_->ApplyConfiguration(config);
    return std::nullopt;
  } catch (const std::runtime_error& exception) {
    const std::string& prompt = "Runtime error: ";
    return std::make_optional<std::string>(prompt + exception.what());
  } catch (const std::exception& exception) {
    const std::string& prompt = "Unexpected error: ";
    return std::make_optional<std::string>(prompt + exception.what());
  }
}
