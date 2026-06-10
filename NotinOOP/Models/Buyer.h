#pragma once
#include "User.h"
#include "Fragrance.h"
#include "Purchase.h"
#include "Discount.h"
#include <vector>

class Buyer : public User {
private:
    double balance;
    std::vector<Fragrance> cart;
    std::vector<Fragrance> wishlist;
    std::vector<Purchase> purchases;
    std::vector<Discount*> discounts;

    void clearDiscounts();
    void copyDiscounts(const std::vector<Discount*>& otherDiscounts);

public:
    Buyer(unsigned int id, const std::string& uname, const std::string& pass, double initialBalance = 0.0);

    ~Buyer();
    Buyer(const Buyer& other);
    Buyer& operator=(const Buyer& other);

    double getBalance() const;
    const std::vector<Fragrance>& getCart() const;
    const std::vector<Fragrance>& getWishlist() const;
    const std::vector<Purchase>& getPurchases() const;
    const std::vector<Discount*>& getDiscounts() const;

    void addToBalance(double amount);
    void addToWishlist(const Fragrance& fragrance);
    bool removeFromWishlist(const std::string& fragranceName);
    void addToCart(const Fragrance& fragrance);
    bool removeFromCart(const std::string& fragranceName);

    void viewCart() const;
    void viewBought() const;
    void viewPurchases() const;

    void makeReview(Fragrance& fragrance, unsigned short rating, const std::string& comment);
    bool cancelPurchase(unsigned int purchaseId);
    void addDiscount(Discount* discount);

    void checkout(unsigned int& nextPurchaseId);
    void recommend(const std::vector<Fragrance>& marketCatalog) const;

    void help() const override;
};