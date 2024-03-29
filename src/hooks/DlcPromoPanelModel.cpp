#include "main.hpp"
#include "hooking.hpp"

#include "PromoRepo.hpp"

#include "GlobalNamespace/MusicPackPromoBanner.hpp"
#include "GlobalNamespace/DlcPromoPanelModel.hpp"
#include "GlobalNamespace/PlayerData.hpp"
#include "GlobalNamespace/PlayerDataModel.hpp"
#include "GlobalNamespace/PlayerSensitivityFlag.hpp"

#include <ranges>
#include <random>

using namespace GlobalNamespace;

MAKE_AUTO_HOOK_MATCH(DlcPromoPanelModel_GetPackDataForMainMenuPromoBanner, &DlcPromoPanelModel::GetPackDataForMainMenuPromoBanner, DlcPromoPanelModel::PromoInfo*, DlcPromoPanelModel* self, ByRef<bool> owned) {
    UmbrellaLogger.info("Generating Promo For Use [DlcPromoPanelModel::GetPackDataForMainMenuPromoBanner]");

    // Check for registration
    if(!Umbrella::PromoRepo::HasRegistered()) {
        UmbrellaLogger.info("PromoRepo has not registered promos, registering promos...");
        Umbrella::PromoRepo::RegisterPromos(self);
    }

    //TODO: If user selects no promo return null here

    // Grab promos from repo

    auto promos = Umbrella::PromoRepo::GetPromos();

    // Filter by type, sensitivity, and quest support

    auto sensitivityFlag = self->_playerDataModel->playerData->desiredSensitivityFlag;
    if (sensitivityFlag == PlayerSensitivityFlag::Unknown) {
        sensitivityFlag = PlayerSensitivityFlag::Themes;
    }

    std::vector<Umbrella::PromoType> preferredTypes = { Umbrella::PromoType::OST_DLC, Umbrella::PromoType::CustomLevels };

    std::vector<Umbrella::Promo*> filteredPromos;
    std::copy_if(promos.begin(), promos.end(), std::back_inserter(filteredPromos),
        [&preferredTypes, &sensitivityFlag](Umbrella::Promo* promo) {
            return std::find(preferredTypes.begin(), preferredTypes.end(), promo->promoType) != preferredTypes.end() && promo->promoInfo->contentRating <= sensitivityFlag && promo->supportedOnQuest;
        });

    // Select a random promo

    if(filteredPromos.size() == 0) {
        UmbrellaLogger.info("No promos found, returning nullptr");
        return self->_defaultPromoInfo;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, filteredPromos.size() - 1);

    auto promo = filteredPromos[dis(gen)];

    if(!promo->promoInfo) {
        UmbrellaLogger.info("Selected Promo is null, returning nullptr");
        return self->_defaultPromoInfo;
    }

    UmbrellaLogger.info("Selected Promo: {}", promo->promoInfo->id);

    // Return the promo

    owned = true;
    
    return promo->promoInfo.ptr();
}