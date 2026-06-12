#pragma once
#include <vector>
#include <string>
#include "../Models/User.h"
#include "../Models/Buyer.h"
#include "../Models/Admin.h"
#include "../Models/Fragrance.h"
#include "../Models/Purchase.h"
#include "../Models/BonusDiscount.h"
#include "../Models/BrandDiscount.h"
#include "../Models/Discount.h"

class NotinOOP {
private:
    static NotinOOP* instance;

    std::vector<User*> users;
    std::vector<Fragrance> catalog;
    std::vector<Purchase> allPurchases;

    User* currentUser;

    unsigned int nextUserId;
    unsigned int nextFragranceId;
    unsigned int nextPurchaseId;

    struct UserStrikes {
        unsigned int userId;
        int deletedReviewsCount;
    };
    std::vector<UserStrikes> strikeList;

    NotinOOP();

    ~NotinOOP();

    NotinOOP(const NotinOOP&) = delete;
    NotinOOP& operator=(const NotinOOP&) = delete;

    void processGuestCommand(const std::string& command);
    void processBuyerCommand(Buyer* buyer, const std::string& command);
    void processAdminCommand(Admin* admin, const std::string& command);

    void addStrike(unsigned int uId);

public:
    static NotinOOP* getInstance();

    static void releaseInstance();

    void run();
};