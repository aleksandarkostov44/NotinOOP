#include "Purchase.h"

Purchase::Purchase(unsigned int pId, unsigned int uId, const std::vector<Fragrance>& frags)
    : purchaseId(pId), userId(uId), fragrances(frags), state(PurchaseState::PENDING)
{}

unsigned int Purchase::getPurchaseId() const {
    return purchaseId;
}

unsigned int Purchase::getUserId() const {
    return userId;
}

PurchaseState Purchase::getState() const {
    return state;
}

const std::vector<Fragrance>& Purchase::getFragrances() const {
    return fragrances;
}

void Purchase::setState(PurchaseState newState) {
    state = newState;
}

double Purchase::getTotalPrice() const {
    double total = 0.0;
    for (const Fragrance& f : fragrances) {
        total += f.getPrice();
    }
    return total;
}

void Purchase::show() const {
    std::cout << "=== Поръчка #" << purchaseId << " ===\n";
    std::cout << "Потребител ID: " << userId << "\n";

    std::cout << "Статус: ";
    switch (state) {
    case PurchaseState::PENDING: std::cout << "PENDING\n"; break;
    case PurchaseState::DELIVERED: std::cout << "DELIVERED\n"; break;
    case PurchaseState::CANCELED: std::cout << "CANCELED\n"; break;
    }

    std::cout << "Парфюми:\n";
    if (fragrances.empty()) {
        std::cout << " - (няма парфюми)\n";
    }
    else {
        for (const Fragrance& f : fragrances) {
            std::cout << " - " << f.getName() << " (" << f.getBrand() << ") : " << f.getPrice() << " лв.\n";
        }
    }

    std::cout << "Обща цена: " << getTotalPrice() << " лв.\n";
    std::cout << "======================\n";
}