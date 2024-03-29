#include "main.hpp"

namespace Umbrella {

    struct Promo {
        std::string_view promoId;
        SafePtr<GlobalNamespace::DlcPromoPanelModel::PromoInfo> promoInfo;
    };

    class PromoRepo {
    public:
        static void RegisterPromos();
        static void AddPromo(Promo promo);
        static std::vector<Promo> GetPromos();

    private:
        inline static std::vector<Promo> promoRepo;
    };
}