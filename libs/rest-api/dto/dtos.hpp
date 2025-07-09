//
// Created by sheyme on 08/07/25.
//

#pragma once

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class StatusDTO : public oatpp::DTO {
  DTO_INIT(StatusDTO, DTO)

  DTO_FIELD(Boolean, success) = true;
  DTO_FIELD(String, message);
};

class ErrorDTO : public oatpp::DTO {
  DTO_INIT(ErrorDTO, DTO)

  DTO_FIELD(Int16, code) = 400;
  DTO_FIELD(String, error);
};

class NonDhcpIpConfigDTO : public oatpp::DTO {
  DTO_INIT(NonDhcpIpConfigDTO, DTO)

  DTO_FIELD(String, interface);
  DTO_FIELD(String, ip);
  DTO_FIELD(String, netmask);
  DTO_FIELD(String, gateway);
  DTO_FIELD(Boolean, is_dhcp) = false;
};

class DhcpIpConfigDTO : public oatpp::DTO {
  DTO_INIT(DhcpIpConfigDTO, DTO)

  DTO_FIELD(String, interface);
};

#include OATPP_CODEGEN_END(DTO)