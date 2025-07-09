//
// Created by sheyme on 08/07/25.
//

#include "controller.hpp"

std::optional<std::string> ConnmanController::ApplyDhcp(const oatpp::Object<
    DhcpIpConfigDTO>& query) {
  ConnmanHandler::IpConfig config {
    query->interface,
    "",
    "",
    "",
    true
  };
  return TryApply(config);
}

std::optional<std::string>
ConnmanController::ApplyStatic(
    const oatpp::Object<NonDhcpIpConfigDTO>& query) {
  ConnmanHandler::IpConfig config = {
      query->interface,
      query->ip,
      query->netmask,
      query->gateway,
      false
  };
  return TryApply(config);
}

std::optional<std::string>
ConnmanController::TryApply(const ConnmanHandler::IpConfig& config) {
  try {
    handler_->SetConfiguration(config);
    return std::nullopt;
  } catch (const std::runtime_error& exception) {
    const std::string& prompt = "Runtime error: ";
    return std::make_optional<std::string>(prompt + exception.what());
  } catch (const std::exception& exception) {
    const std::string& prompt = "Unexpected error: ";
    return std::make_optional<std::string>(prompt + exception.what());
  }
}
