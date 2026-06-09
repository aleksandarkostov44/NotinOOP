#include "BonusDiscount.h"

BonusDiscount::BonusDiscount(unsigned int id, double percent, double bonusAmount)
    : Discount(id, percent), bonus(bonusAmount) {}

double BonusDiscount::apply(double currentPrice, const std::string& fragranceBrand) const {
    double priceAfterPercent = Discount::apply(currentPrice, fragranceBrand);

    double finalPrice = priceAfterPercent - bonus;

    return (finalPrice > 0.0) ? finalPrice : 0.0;
}