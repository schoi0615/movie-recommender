#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include "MovieManager.h"
#include "RatingManager.h"
#include <vector>
#include <utility> // pair를 위해 추가

class Recommender {
private:
    MovieManager& movieMgr;
    RatingManager& ratingMgr;
    int calculateSimilarity(const std::vector<Rating>& ratingsA, const std::vector<Rating>& ratingsB);

public:
    Recommender(MovieManager& mMgr, RatingManager& rMgr);
    // 리턴 타입 수정
    std::vector<std::pair<Movie, int>> recommend(int userId, int k, int n);
};

#endif