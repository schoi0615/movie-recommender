#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include "MovieManager.h"
#include "RatingManager.h"
#include "movie.h"
#include "rating.h"
#include <vector>

class Recommender {
private:
    MovieManager& movieMgr;
    RatingManager& ratingMgr;

public:
    Recommender(MovieManager& mMgr, RatingManager& rMgr);

    static int Similaritycalculate(const std::vector<Rating>& ratingsA, const std::vector<Rating>& ratingsB);
    std::vector<Movie> recommend(int userId, int k, int n);
};

#endif