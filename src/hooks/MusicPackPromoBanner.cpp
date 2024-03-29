#include "main.hpp"
#include "hooking.hpp"

#include "GlobalNamespace/MusicPackPromoBanner.hpp"
#include "GlobalNamespace/DlcPromoPanelModel.hpp"

#include "TMPro/TextMeshProUGUI.hpp"

#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/GameObject.hpp"

using namespace GlobalNamespace;

MAKE_AUTO_HOOK_MATCH(MusicPackPromoBanner_Setup, &MusicPackPromoBanner::Setup, void, MusicPackPromoBanner* self, DlcPromoPanelModel::PromoInfo* newPromoInfo, bool probablyOwned) {
    UmbrellaLogger.info("Setting up Promo [MusicPackPromoBanner::Setup]");
    if(newPromoInfo == nullptr) {
        return;
    }
    MusicPackPromoBanner_Setup(self, newPromoInfo, probablyOwned);
    self->____promoText->set_richText(true);
}