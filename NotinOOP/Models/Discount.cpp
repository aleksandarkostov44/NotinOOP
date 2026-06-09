#include "Discount.h"
#include <stdexcept>

Discount::Discount(unsigned int id, double percent) : discountId(id) {
    if (percent < 0.0 || percent > 100.0) {
        throw std::invalid_argument("Процентът трябва да е между 0 и 100.");
    }
    discountPercent = percent;
}

unsigned int Discount::getDiscountId() const { return discountId; }
double Discount::getDiscountPercent() const { return discountPercent; }

double Discount::apply(double currentPrice, const std::string& fragranceBrand) const {
    return currentPrice - (currentPrice * (discountPercent / 100.0));
}