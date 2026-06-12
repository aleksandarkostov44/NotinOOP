#include "Buyer.h"
#include "BonusDiscount.h"
#include "BrandDiscount.h"
#include <iostream>

Buyer::Buyer(unsigned int id, const std::string& uname, const std::string& pass, double initialBalance)
    : User(id, uname, pass), balance(initialBalance) {}

void Buyer::clearDiscounts() {
    for (Discount* d : discounts) {
        delete d;
    }
    discounts.clear();
}

void Buyer::copyDiscounts(const std::vector<Discount*>& otherDiscounts) {
    for (Discount* d : otherDiscounts) {
        if (auto* brandD = dynamic_cast<BrandDiscount*>(d)) {
            discounts.push_back(new BrandDiscount(*brandD));
        }
        else if (auto* bonusD = dynamic_cast<BonusDiscount*>(d)) {
            discounts.push_back(new BonusDiscount(*bonusD));
        }
        else {
            discounts.push_back(new Discount(*d));
        }
    }
}

Buyer::~Buyer() {
    clearDiscounts();
}

Buyer::Buyer(const Buyer& other) : User(other) {
    balance = other.balance;
    cart = other.cart;
    wishlist = other.wishlist;
    purchases = other.purchases;
    copyDiscounts(other.discounts);
}

Buyer& Buyer::operator=(const Buyer& other) {
    if (this != &other) {
        User::operator=(other);
        clearDiscounts();
        balance = other.balance;
        cart = other.cart;
        wishlist = other.wishlist;
        purchases = other.purchases;
        copyDiscounts(other.discounts);
    }
    return *this;
}

double Buyer::getBalance() const { return balance; }
const std::vector<Fragrance>& Buyer::getCart() const { return cart; }
const std::vector<Fragrance>& Buyer::getWishlist() const { return wishlist; }
const std::vector<Purchase>& Buyer::getPurchases() const { return purchases; }
const std::vector<Discount*>& Buyer::getDiscounts() const { return discounts; }

void Buyer::addToBalance(double amount) {
    if (amount > 0) balance += amount;
}

void Buyer::addToWishlist(const Fragrance& fragrance) {
    wishlist.push_back(fragrance);
}

bool Buyer::removeFromWishlist(const std::string& fragranceName) {
    for (auto it = wishlist.begin(); it != wishlist.end(); ++it) {
        if (it->getName() == fragranceName) {
            wishlist.erase(it);
                return true;
        }
    }
    return false;
}

void Buyer::addToCart(const Fragrance& fragrance) {
    cart.push_back(fragrance);
}

bool Buyer::removeFromCart(const std::string& fragranceName) {
    for (auto it = cart.begin(); it != cart.end(); ++it) {
        if (it->getName() == fragranceName) {
            cart.erase(it);
                return true;
        }
    }
    return false;
}

void Buyer::viewCart() const {
    std::cout << "--- Вашата количка ---\n";
        if (cart.empty()) {
            std::cout << "Количката е празна.\n";
            return;
        }
    double total = 0;
    for (const auto& f : cart) {
        std::cout << " - " << f.getName() << " | " << f.getPrice() << " лв.\n";
        total += f.getPrice();
    }
    std::cout << "Обща сума без отстъпки: " << total << " лв.\n";
}

void Buyer::viewPurchases() const {
    std::cout << "--- Всички ваши поръчки ---\n";
        if (purchases.empty()) {
            std::cout << "Нямате направени поръчки.\n";
            return;
        }
    for (const auto& p : purchases) {
        p.show();
    }
}

void Buyer::viewBought() const {
    std::cout << "--- Успешни покупки ---\n";
        bool found = false;
    for (const auto& p : purchases) {
        if (p.getState() != PurchaseState::CANCELED) {
            p.show();
                found = true;
        }
    }
    if (!found) std::cout << "Нямате успешни покупки.\n";
}

void Buyer::addDiscount(Discount* discount) {
    if (discount) discounts.push_back(discount);
}

void Buyer::makeReview(Fragrance& fragrance, unsigned int rId, unsigned short rating, const std::string& comment) {
    Review newReview(rId, fragrance.getName(), this->userId, comment, rating);
    fragrance.addReview(newReview);
    std::cout << "Ревюто за " << fragrance.getName() << " беше споделено успешно!\n";
}

bool Buyer::cancelPurchase(unsigned int purchaseId) {
    for (auto& p : purchases) {
        if (p.getPurchaseId() == purchaseId) {
            if (p.getState() == PurchaseState::PENDING) {
                p.setState(PurchaseState::CANCELED);
                    return true;
            }
            break;
        }
    }
    return false;
}

void Buyer::checkout(unsigned int& nextPurchaseId) {
    if (cart.empty()) {
        std::cout << "Количката е празна! Няма какво да купите.\n";
        return;
    }

    double baseTotal = 0.0;
    for (const auto& f : cart) {
        baseTotal += f.getPrice();
    }

    Discount* bestDiscount = nullptr;
    double bestPrice = baseTotal;
    int bestDiscountIdx = -1;

    for (size_t i = 0; i < discounts.size(); ++i) {
        double currentDiscountedTotal = 0.0;
        for (const auto& f : cart) {
            currentDiscountedTotal += discounts[i]->apply(f.getPrice(), f.getBrand());
        }
        if (currentDiscountedTotal < bestPrice) {
            bestPrice = currentDiscountedTotal;
            bestDiscount = discounts[i];
            bestDiscountIdx = static_cast<int>(i);
        }
    }

    if (balance < bestPrice) {
        std::cout << "Нямате достатъчна наличност в сметката! Нужни: " << bestPrice << " лв. Имате: " << balance << " лв.\n";
        return;
    }

    std::vector<Fragrance> finalItems = cart;
    if (bestDiscount) {
        for (auto& f : finalItems) {
            double newPrice = bestDiscount->apply(f.getPrice(), f.getBrand());
            f.setPrice(newPrice);
        }
        std::cout << "Автоматично беше използван най-добрият ваучер! Спестихте: " << (baseTotal - bestPrice) << " лв.\n";

        delete discounts[bestDiscountIdx];
        discounts.erase(discounts.begin() + bestDiscountIdx);
    }

    balance -= bestPrice;
        Purchase newPurchase(nextPurchaseId++, this->userId, finalItems);
        purchases.push_back(newPurchase);

        cart.clear();
        std::cout << "Покупката приключи успешно! Създадена е поръчка #" << newPurchase.getPurchaseId() << "\n";
}

void Buyer::recommend(const std::vector<Fragrance>& marketCatalog) const {
    if (wishlist.empty()) {
        std::cout << "Добавете парфюми в списъка си с любими (wishlist), за да получите препоръки!\n";
        return;
    }

    struct FamilyCount {
        std::string familyName;
        unsigned int count = 0;
    };

    std::vector<FamilyCount> counts;

    for (const auto& f : wishlist) {
        std::string currentFamily = f.getFragranceFamily();
            bool found = false;

        for (auto& c : counts) {
            if (c.familyName == currentFamily) {
                c.count++;
                found = true;
                break;
            }
        }

        if (!found) {
            counts.push_back({ currentFamily, 1 });
        }
    }

    std::string favoriteFamily = "";
    unsigned int maxCount = 0;
    for (const auto& c : counts) {
        if (c.count > maxCount) {
            maxCount = c.count;
            favoriteFamily = c.familyName;
        }
    }

    std::cout << "Вашата любима нотка е: " << favoriteFamily << ". Ето нашите препоръки за Вас:\n";

        std::vector<Fragrance> candidates;
    for (const auto& marketFrag : marketCatalog) {
        if (marketFrag.getFragranceFamily() == favoriteFamily) {
            bool alreadyInWishlist = false;
            for (const auto& wishFrag : wishlist) {
                if (marketFrag.getFragranceId() == wishFrag.getFragranceId()) {
                    alreadyInWishlist = true;
                    break;
                }
            }
            if (!alreadyInWishlist) {
                candidates.push_back(marketFrag);
            }
        }
    }

    if (candidates.empty()) {
        std::cout << "В момента няма други налични парфюми от нотка " << favoriteFamily << " извън любимите Ви.\n";
        return;
    }

    size_t recommendCount = (candidates.size() < 3) ? candidates.size() : 3;
        for (size_t i = 0; i < recommendCount; ++i) {
            size_t idx = (i * 7) % candidates.size();
            std::cout << " * " << candidates[idx].getName() << " от марката " << candidates[idx].getBrand() << " (" << candidates[idx].getPrice() << " лв.)\n";
        }
}

void Buyer::help() const {
    std::cout << "=== Налични команди за Купувач ===\n";
    std::cout << " 1. add-to-balance <amount>\n";
    std::cout << " 2. add-to-wishlist <fragrance-name>\n";
    std::cout << " 3. remove-from-wishlist <fragrance-name>\n";
    std::cout << " 4. add-to-cart <fragrance-name>\n";
    std::cout << " 5. remove-from-cart <fragrance-name>\n";
    std::cout << " 6. view-cart\n";
    std::cout << " 7. view-bought\n";
    std::cout << " 8. view-purchases\n";
    std::cout << " 9. view-wishlist\n";
    std::cout << " 10. recommend\n";
    std::cout << " 11. checkout\n";
    std::cout << " 12. cancel <purchase-id>\n";
    std::cout << " 13. make-review <fragrance-name> <rating> <comment>\n";
    std::cout << " 14. logout\n";
    std::cout << "=================================\n";
}

void Buyer::loadPurchase(const Purchase& purchase) {
    purchases.push_back(purchase);
}

bool Buyer::deliverPurchase(unsigned int purchaseId) {
    for (auto& p : purchases) {
        if (p.getPurchaseId() == purchaseId && p.getState() == PurchaseState::PENDING) {
            p.setState(PurchaseState::DELIVERED);
            return true;
        }
    }
    return false;
}

void Buyer::viewWishlist() const {
    std::cout << "--- Твоите любими парфюми (Wishlist) ---\n";
    if (wishlist.empty()) {
        std::cout << "Списъкът е празен.\n";
        return;
    }
    for (const auto& f : wishlist) {
        std::cout << " - " << f.getName() << " (" << f.getBrand() << ") | " << f.getPrice() << " лв.\n";
    }
    std::cout << "----------------------------------------\n";
}
