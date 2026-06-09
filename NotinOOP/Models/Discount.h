#pragma once
#include <string>

class Discount {
private:
    unsigned int discountId;
    double discountPercent;

public:
    Discount(unsigned int id, double percent);

    virtual ~Discount() = default;

    unsigned int getDiscountId() const;
    double getDiscountPercent() const;

    virtual double apply(double currentPrice, const std::string& fragranceBrand) const;
};