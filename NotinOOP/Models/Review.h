#pragma once
#include <string>
#include <stdexcept>

class Review {
private:
    unsigned int reviewId;
    std::string fragranceName;
    unsigned int userId;
    std::string comment;
    unsigned short rating;

public:
    Review(unsigned int reviewId, const std::string& fragranceName, unsigned int userId, const std::string& comment, unsigned short rating);

    unsigned int getReviewId() const;
    const std::string& getFragranceName() const;
    unsigned int getUserId() const;
    const std::string& getComment() const;
    unsigned short getRating() const;

    void setComment(const std::string& newComment);
    void setRating(unsigned short newRating);
};