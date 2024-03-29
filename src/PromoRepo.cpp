#include "PromoRepo.hpp"

#include "System/Collections/Generic/List_1.hpp"
#include "GlobalNamespace/PlayerSensitivityFlag.hpp"

#include "bsml/shared/Helpers/utilities.hpp"

#include "assets.hpp"

#include "DownloaderUtility.hpp"

using namespace GlobalNamespace;

std::vector<Umbrella::Promo*> Umbrella::PromoRepo::promoRepo;
bool Umbrella::PromoRepo::hasRegistered = false;

template<typename R>
bool is_ready(std::future<R> const& f)
    { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

void Umbrella::PromoRepo::RegisterCustomPromos() {
    UmbrellaLogger.info("Registering Custom Promos...");

    auto downloaderUtility = Umbrella::DownloaderUtility();

    std::string base = "https://raw.githubusercontent.com/bs-umbrella/UmbrellaRepo/main/Promo/";

    //Download index json and parse with rapidjson
    auto indexJson = downloaderUtility.GetJson(base + "index.json");
    
    while(!is_ready(indexJson)) {
        indexJson.wait_until(std::chrono::system_clock::time_point::min());
    }

    auto indexJsonResult = indexJson.get();

    if (!indexJsonResult) {
        UmbrellaLogger.error("Failed to download index.json");
        return;
    }

    auto featuredJson = indexJsonResult->FindMember("featured");
    auto url = base + featuredJson->value.GetString();

    auto featuredPromos = downloaderUtility.GetJson(url);

    while(!is_ready(featuredPromos)) {
        featuredPromos.wait_until(std::chrono::system_clock::time_point::min());
    }

    auto featuredPromosResult = featuredPromos.get();

    if (!featuredPromosResult) {
        UmbrellaLogger.error("Failed to download featured.json");
        return;
    }

    auto packs = featuredPromosResult->FindMember("promos");

    auto& array = packs->value;
    for (size_t i = 0; i < array.Size(); i++) {
        auto& pack = array[i];
        auto name = pack.FindMember("name")->value.GetString();
        auto url = pack.FindMember("url")->value.GetString();
        auto quest = pack.FindMember("quest")->value.GetBool();
        auto image = pack.FindMember("image")->value.GetString();
        UmbrellaLogger.info("Pack: {} | URL: {} | Quest: {} | Image: {}", name, url, quest, image);
    }
}

void Umbrella::PromoRepo::RegisterAdditionalPromos(DlcPromoPanelModel* promoModel) {
    UmbrellaLogger.info("Registering Promos...");

    auto promoInfo = reinterpret_cast<DlcPromoPanelModel::PromoInfo*>(il2cpp_functions::object_new(classof(DlcPromoPanelModel::PromoInfo*)));
    promoInfo->promoType = DlcPromoPanelModel::PromoInfo::PromoType::Pack;
    promoInfo->id = "custom_levelpack_CustomLevels";
    promoInfo->contentRating = PlayerSensitivityFlag::Safe;
    promoInfo->bannerImage = BSML::Utilities::LoadSpriteRaw(Assets::Customs_Banner_Notepad_png);
    promoInfo->bannerPromoText = "<color=#bdacd1><size=150%>Custom Levels";
    promoInfo->bannerPromoTextPosition = 37.0f;
    auto customLevelsRepo = new Promo("custom_levels", PromoType::CustomLevels, true, 15, promoInfo);
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