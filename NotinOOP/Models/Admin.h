#pragma once
#include "User.h"
#include "Fragrance.h"
#include "Purchase.h"
#include <vector>
#include <string>

class Admin : public User {
public:
    Admin(unsigned int id, const std::string& uname, const std::string& pass);

    void help() const override;

    void createFragrance(std::vector<Fragrance>& catalog, unsigned int nextFragranceId,
        const std::string& name, const std::string& brand,
        double price, const std::string& family, unsigned int qty);

    bool addQuantity(std::vector<Fragrance>& catalog, const std::string& fragranceName, unsigned int qty);

    bool deliver(std::vector<Purchase>& allPurchases, unsigned int purchaseId);

    unsigned int removeReview(std::vector<Fragrance>& catalog, unsigned int fragranceId, unsigned int reviewId);

    bool blockUser(std::vector<User*>& users, const std::string& targetUsername);
};