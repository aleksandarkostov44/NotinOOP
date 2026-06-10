#pragma once
#include <vector>
#include <iostream>
#include "Fragrance.h"

enum class PurchaseState {
    PENDING,
    DELIVERED,
    CANCELED
};

class Purchase {
private:
    unsigned int purchaseId;
    unsigned int userId;
    std::vector<Fragrance> fragrances;
    PurchaseState state;

public:
    Purchase(unsigned int pId, unsigned int uId, const std::vector<Fragrance>& frags);

    unsigned int getPurchaseId() const;
    unsigned int getUserId() const;
    PurchaseState getState() const;
    const std::vector<Fragrance>& getFragrances() const;

    void setState(PurchaseState newState);

    double getTotalPrice() const;

    void show() const;
};