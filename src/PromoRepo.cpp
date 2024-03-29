#include "PromoRepo.hpp"

#include "System/Collections/Generic/List_1.hpp"
#include "GlobalNamespace/PlayerSensitivityFlag.hpp"

#include "bsml/shared/Helpers/utilities.hpp"

#include "assets.hpp"

using namespace GlobalNamespace;

std::vector<Umbrella::Promo*> Umbrella::PromoRepo::promoRepo;
bool Umbrella::PromoRepo::hasRegistered = false;

void Umbrella::PromoRepo::RegisterPromos(DlcPromoPanelModel* promoModel) {
    UmbrellaLogger.info("Registering Promos...");

    auto promoInfo = reinterpret_cast<DlcPromoPanelModel::PromoInfo*>(il2cpp_functions::object_new(classof(DlcPromoPanelModel::PromoInfo*)));
    promoInfo->promoType = DlcPromoPanelModel::PromoInfo::PromoType::Pack;
    promoInfo->id = "custom_levelpack_CustomLevels";
    promoInfo->contentRating = PlayerSensitivityFlag::Safe;
    promoInfo->bannerImage = BSML::Utilities::LoadSpriteRaw(Assets::Customs_Banner_Notepad_png);
    promoInfo->bannerPromoText = "";
    promoInfo->bannerPromoTextPosition = -420.69f;
    auto customLevelsRepo = new Promo("custom_levels", PromoType::CustomLevels, true, promoInfo);
    RegisterPromo(customLevelsRepo);

    ListW<GlobalNamespace::DlcPromoPanelModel::PromoInfo*> promoInfos = ListW<GlobalNamespace::DlcPromoPanelModel::PromoInfo*>::New();
    promoInfos->AddRange(promoModel->_promoInfos->i___System__Collections__Generic__IEnumerable_1_T_());

    for (auto DLCPromoInfo : promoInfos) {
        UmbrellaLogger.info("Registering Promo: {}", fmt::ptr(DLCPromoInfo));
        Promo* promo = new Promo();
        std::string id = DLCPromoInfo->id;
        promo->promoId = id;
        promo->promoType = PromoType::OST_DLC;
        promo->supportedOnQuest = true;
        promo->promoInfo = DLCPromoInfo;
        RegisterPromo(promo);
    }

    // TODO: Implement featured playlist loading when json is available

    hasRegistered = true;
}

void Umbrella::PromoRepo::RegisterPromo(Umbrella::Promo* promo) {
    promoRepo.push_back(promo);
}

std::vector<Umbrella::Promo*> Umbrella::PromoRepo::GetPromos() {
    return promoRepo;
}

bool Umbrella::PromoRepo::HasRegistered() {
    return hasRegistered;
}