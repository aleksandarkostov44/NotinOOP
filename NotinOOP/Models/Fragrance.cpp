#include "Fragrance.h"
#include <stdexcept>

Fragrance::Fragrance(unsigned int id, const std::string& fName, const std::string& fBrand,
    double fPrice, const std::string& fFamily, unsigned int fQuantity)
    : fragranceId(id), name(fName), brand(fBrand), fragranceFamily(fFamily)
{
    setPrice(fPrice);
    quantity = fQuantity;
}

unsigned int Fragrance::getFragranceId() const { return fragranceId; }
const std::string& Fragrance::getName() const { return name; }
const std::string& Fragrance::getBrand() const { return brand; }
double Fragrance::getPrice() const { return price; }
const std::string& Fragrance::getFragranceFamily() const { return fragranceFamily; }
unsigned int Fragrance::getQuantity() const { return quantity; }
const std::vector<Review>& Fragrance::getReviews() const { return reviews; }

void Fragrance::setPrice(double newPrice) {
    if (newPrice < 0.0) {
        throw std::invalid_argument("Цената не може да бъде отрицателна!");
    }
    price = newPrice;
}

void Fragrance::addQuantity(unsigned int amount) {
    quantity += amount;
}

bool Fragrance::reduceQuantity(unsigned int amount) {
    if (quantity >= amount) {
        quantity -= amount;
        return true;
    }
    return false;
}

void Fragrance::addReview(const Review& review) {
    reviews.push_back(review);
}

bool Fragrance::removeReview(unsigned int reviewId) {
    for (auto it = reviews.begin(); it != reviews.end(); ++it) {
        if (it->getReviewId() == reviewId) {
            reviews.erase(it);
            return true;
        }
    }
    return false;
}

double Fragrance::getRating() const {
    if (reviews.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (const Review& review : reviews) {
        sum += review.getRating();
    }

    return sum / reviews.size();
}