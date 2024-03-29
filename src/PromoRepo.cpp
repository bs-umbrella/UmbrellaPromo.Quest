#include "PromoRepo.hpp"

void Umbrella::PromoRepo::RegisterPromos() {
    UmbrellaLogger.info("Registering Promos...");
    // AddPromo({ "promoId", nullptr });
}

void Umbrella::PromoRepo::AddPromo(Promo promo) {
    promoRepo.push_back(promo);
}

std::vector<Umbrella::Promo> Umbrella::PromoRepo::GetPromos() {
    return promoRepo;
}