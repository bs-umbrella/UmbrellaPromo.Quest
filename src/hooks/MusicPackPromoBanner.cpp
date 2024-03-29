#include "main.hpp"
#include "hooking.hpp"

#include "GlobalNamespace/MusicPackPromoBanner.hpp"
#include "GlobalNamespace/DlcPromoPanelModel.hpp"

using namespace GlobalNamespace;

MAKE_AUTO_HOOK_MATCH(MusicPackPromoBanner_Setup, &MusicPackPromoBanner::Setup, void, MusicPackPromoBanner* self, DlcPromoPanelModel::PromoInfo* newPromoInfo, bool probablyOwned) {
    UmbrellaLogger.info("Setting up Promo [MusicPackPromoBanner::Setup]");
    if(newPromoInfo == nullptr) {
        return;
    }
    MusicPackPromoBanner_Setup(self, newPromoInfo, probablyOwned);
}