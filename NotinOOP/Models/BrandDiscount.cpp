#include "BrandDiscount.h"

BrandDiscount::BrandDiscount(unsigned int id, double percent, const std::string& bName)
    : Discount(id, percent), brandName(bName) {}

double BrandDiscount::apply(double currentPrice, const std::string& fragranceBrand) const {
    if (fragranceBrand == brandName) {
        return Discount::apply(currentPrice, fragranceBrand);
    }

    return currentPrice;
}

const std::string& BrandDiscount::getBrand() const
{
    return brandName;
}
