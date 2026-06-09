#include "Review.h"

Review::Review(unsigned int id, const std::string& fName, unsigned int uId, const std::string& comm, unsigned short r)
    : reviewId(id), fragranceName(fName), userId(uId)
{
    setComment(comm);
    setRating(r);
}

unsigned int Review::getReviewId() const {
    return reviewId;
}

const std::string& Review::getFragranceName() const {
    return fragranceName;
}

unsigned int Review::getUserId() const {
    return userId;
}

const std::string& Review::getComment() const {
    return comment;
}

unsigned short Review::getRating() const {
    return rating;
}

void Review::setComment(const std::string& newComment) {
    if (newComment.length() > 1000) {
        throw std::invalid_argument("Коментарът не може да надвишава 1000 символа!");
    }
    comment = newComment;
}

void Review::setRating(unsigned short newRating) {
    if (newRating > 5) {
        throw std::invalid_argument("Рейтингът трябва да бъде между 0 и 5!");
    }
    rating = newRating;
}