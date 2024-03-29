#include "main.hpp"

#include "GlobalNamespace/DlcPromoPanelModel.hpp"

namespace Umbrella {

    enum PromoType {
        OST_DLC,
        CustomLevels,
        FeaturedPlaylist
    };

    class Promo {
    public:
        Promo() = default;
        Promo(std::string promoId, PromoType promoType, bool supportedOnQuest, int weight, SafePtr<GlobalNamespace::DlcPromoPanelModel::PromoInfo> promoInfo) : promoId(promoId), promoType(promoType), supportedOnQuest(supportedOnQuest), weight(weight), promoInfo(promoInfo) {}
        std::string promoId;
        PromoType promoType;
        bool supportedOnQuest;
        int weight = 1;
        SafePtr<GlobalNamespace::DlcPromoPanelModel::PromoInfo> promoInfo;
    };

    class PromoRepo {
    public:
        static void RegisterCustomPromos();
        static void RegisterAdditionalPromos(GlobalNamespace::DlcPromoPanelModel* promoModel);
        static void RegisterPromo(Umbrella::Promo* promo);
        static std::vector<Umbrella::Promo*> GetPromos();
        static bool HasRegistered();

    private:
        static std::vector<Umbrella::Promo*> promoRepo;
        static bool hasRegistered;
    };
}