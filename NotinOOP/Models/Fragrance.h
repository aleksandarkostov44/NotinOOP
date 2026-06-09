#pragma once
#include <string>
#include <vector>
#include "Review.h"

class Fragrance {
private:
    unsigned int fragranceId;
    std::string name;
    std::string brand;
    double price;
    std::string fragranceFamily;
    unsigned int quantity;
    std::vector<Review> reviews;

public:
    Fragrance(unsigned int id, const std::string& fName, const std::string& fBrand,
        double fPrice, const std::string& fFamily, unsigned int fQuantity);

    unsigned int getFragranceId() const;
    const std::string& getName() const;
    const std::string& getBrand() const;
    double getPrice() const;
    const std::string& getFragranceFamily() const;
    unsigned int getQuantity() const;

    const std::vector<Review>& getReviews() const;

    void setPrice(double newPrice);
    void addQuantity(unsigned int amount);
    bool reduceQuantity(unsigned int amount);

    void addReview(const Review& review);
    bool removeReview(unsigned int reviewId);

    double getRating() const;
};