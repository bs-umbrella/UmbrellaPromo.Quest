#pragma once

#include "_config.hpp"

#include "scotland2/shared/modloader.h"

#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"

#include "paper/shared/logger.hpp"

Configuration &getConfig();

constexpr auto UmbrellaLogger = Paper::ConstLoggerContext("umbrella-promo");