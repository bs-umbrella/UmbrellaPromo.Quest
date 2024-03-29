#include "main.hpp"

#include "hooking.hpp"

#include "PromoRepo.hpp"

static modloader::ModInfo modInfo{MOD_ID, VERSION, 0};

Configuration &getConfig() {
  static Configuration config(modInfo);
  return config;
}

MOD_EXPORT void setup(CModInfo *info) noexcept {
  *info = modInfo.to_c();

  getConfig().Load();

  Paper::Logger::RegisterFileContextId(UmbrellaLogger.tag);

  UmbrellaLogger.info("Completed setup!");
}

MOD_EXPORT void late_load() noexcept {
  il2cpp_functions::Init();

  UmbrellaLogger.info("Installing hooks...");

  Umbrella::Hooking::InstallHooks();

  UmbrellaLogger.info("Installed all hooks!");

  Umbrella::PromoRepo::RegisterCustomPromos();
}