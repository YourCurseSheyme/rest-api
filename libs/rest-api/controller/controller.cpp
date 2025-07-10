//
// Created by sheyme on 08/07/25.
//

#include "controller.hpp"

ConnmanController::ConnmanController(const std::shared_ptr<
    oatpp::web::mime::ContentMappers>& api_content_mappers)
    : oatpp::web::server::api::ApiController(api_content_mappers) {
  connman_ = std::make_unique<ConnmanHandler>();
  wps_ = std::make_unique<WpsHandler>();
  is_dhcp_ = connman_->IsDhcpEnabled();
}

std::optional<std::string> ConnmanController::ApplyDhcp(const oatpp::Object<
    DhcpIpConfigDTO>& query) {
  NetConfig config;
  config.interface = query->interface;
  return TryApply(config);
}

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
    if (is_dhcp_) { /*
      if (connman_->IsServiceActive()) {
        connman_->StopService();
      }
      wps_->ApplyConfiguration(config);
      connman_->StartService(); */
      connman_->ApplyConfiguration(config);
      return std::nullopt;
    } /*
    if (wps_->IsServiceActive()) {
        wps_->StopService();
    }
    if (!connman_->IsServiceActive()) {
      connman_->StartService();
    } */
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
