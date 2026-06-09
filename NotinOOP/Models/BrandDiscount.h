#pragma once
#include "Discount.h"
#include <string>

class BrandDiscount : public Discount {
private:
    std::string brandName;

public:
    BrandDiscount(unsigned int id, double percent, const std::string& bName);

    double apply(double currentPrice, const std::string& fragranceBrand) const override;
};