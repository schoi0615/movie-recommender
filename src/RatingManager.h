#ifndef RATING_MANAGER_H
#define RATING_MANAGER_H

#include "BaseManager.h"
#include "rating.h"
#include <vector>
#include <string>

class RatingManager : public BaseManager {
private:
    std::vector<Rating> ratings;

public:
    void addRating(const Rating& rating);
    std::vector<Rating> findByUser(int userId) const;
    std::vector<int> getAllUserIds() const;
    void printRatingsForMovie(int movieId) const;

    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
    int size() const override;
};

#endif