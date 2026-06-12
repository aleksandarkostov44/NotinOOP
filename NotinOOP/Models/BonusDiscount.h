#pragma once
#include "Discount.h"

class BonusDiscount : public Discount {
private:
    double bonus;

public:
    BonusDiscount(unsigned int id, double percent, double bonusAmount);

    double apply(double currentPrice, const std::string& fragranceBrand) const override;

    double getAmount() const;
};
