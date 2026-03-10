#pragma once
#include "JackCDevService.h"

JACKC_COMMON_EXPORT std::string base64_encode(unsigned char const* , unsigned int len);
JACKC_COMMON_EXPORT std::string base64_decode(std::string const& s);