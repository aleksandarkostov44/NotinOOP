#include "Admin.h"
#include <iostream>

Admin::Admin(unsigned int id, const std::string& uname, const std::string& pass)
    : User(id, uname, pass) {}

void Admin::help() const {
    std::cout << "=== Налични команди за Администратор ===\n";
    std::cout << " 1. block-user <username>\n";
    std::cout << " 2. create-fragrance <name> <brand> <price> <fragranceFamily>\n";
    std::cout << " 3. add-quantity <fragrance-name> <quantity>\n";
    std::cout << " 4. deliver <purchase-id>\n";
    std::cout << " 5. remove-review <fragrance-id> <review-id>\n";
    std::cout << " 6. logout\n";
    std::cout << "========================================\n";
}

void Admin::createFragrance(std::vector<Fragrance>& catalog, unsigned int nextFragranceId,
    const std::string& name, const std::string& brand,
    double price, const std::string& family, unsigned int qty) {
    Fragrance newFrag(nextFragranceId, name, brand, price, family, qty);
    catalog.push_back(newFrag);
    std::cout << "Парфюмът '" << name << "' беше успешно добавен в каталога!\n";
}

bool Admin::addQuantity(std::vector<Fragrance>& catalog, const std::string& fragranceName, unsigned int qty) {
    for (auto& f : catalog) {
        if (f.getName() == fragranceName) {
            f.addQuantity(qty);
            std::cout << "Добавено количество " << qty << " към парфюма '" << fragranceName << "'.\n";
            return true;
        }
    }
    return false;
}

bool Admin::deliver(std::vector<Purchase>& allPurchases, unsigned int purchaseId) {
    for (auto& p : allPurchases) {
        if (p.getPurchaseId() == purchaseId) {
            if (p.getState() == PurchaseState::PENDING) {
                p.setState(PurchaseState::DELIVERED);
                std::cout << "Поръчка #" << purchaseId << " е маркирана като ДОСТАВЕНА.\n";
                return true;
            }
            else {
                std::cout << "Грешка: Поръчка #" << purchaseId << " вече е обработена или отказана.\n";
                return false;
            }
        }
    }
    return false;
}

unsigned int Admin::removeReview(std::vector<Fragrance>& catalog, unsigned int fragranceId, unsigned int reviewId) {
    for (auto& f : catalog) {
        if (f.getFragranceId() == fragranceId) {
            const std::vector<Review>& revs = f.getReviews();
            unsigned int authorId = 0;
            bool found = false;

            for (const auto& r : revs) {
                if (r.getReviewId() == reviewId) {
                    authorId = r.getUserId();
                    found = true;
                    break;
                }
            }

            if (found) {
                f.removeReview(reviewId);
                std::cout << "Ревю #" << reviewId << " беше премахнато от парфюма.\n";
                return authorId;
            }
        }
    }
    return 0;
}

bool Admin::blockUser(std::vector<User*>& users, const std::string& targetUsername) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->getUsername() == targetUsername) {
            if (dynamic_cast<Admin*>(*it) != nullptr) {
                std::cout << "Грешка: Не можете да блокирате друг администратор!\n";
                return false;
            }

            delete* it;
            users.erase(it);
            std::cout << "Потребител '" << targetUsername << "' беше успешно блокиран и изтрит.\n";
            return true;
        }
    }
    return false;
}