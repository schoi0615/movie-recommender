#include "Recommender.h"
#include <cstdlib>
#include <algorithm>
#include <set>
#include <map>
#include <iostream>

Recommender::Recommender(MovieManager& mMgr, RatingManager& rMgr)
    : movieMgr(mMgr), ratingMgr(rMgr) {}

int Recommender::Similaritycalculate(const std::vector<Rating>& ratingsA, const std::vector<Rating>& ratingsB) {
    int commonCount = 0;
    int scoreDiffSum = 0;

    
    for (const Rating& r1 : ratingsA) {
        for (const Rating& r2 : ratingsB) {
            if (r1.getMovieId() == r2.getMovieId()) {
                commonCount++;
                scoreDiffSum += std::abs(r1.getScore() - r2.getScore());
            }
        }
    }

    
    if (commonCount == 0) {
        return -100;
    }

    
    return (commonCount * 10) - scoreDiffSum;
}

std::vector<Movie> Recommender::recommend(int userId, int k, int n) {
    
    std::vector<Rating> myRatings = ratingMgr.findByUser(userId);
    if (myRatings.empty()) {
        std::cout << "[안내] 해당 유저의 평점 데이터가 없어 추천이 불가능합니다." << std::endl;
        return std::vector<Movie>();
    }

    
    std::vector<std::pair<int, int>> similarities; 
    std::vector<int> allUserIds = ratingMgr.getAllUserIds();

    for (int otherId : allUserIds) {
        if (otherId == userId) continue;
        std::vector<Rating> otherRatings = ratingMgr.findByUser(otherId);
        int sim = Similaritycalculate(myRatings, otherRatings);
        if (sim != -100) {
            similarities.push_back({otherId, sim});
        }
    }

    
    std::sort(similarities.begin(), similarities.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;
    });


    size_t actualK = std::min(static_cast<size_t>(k), similarities.size());

    
    std::set<int> myMovieIds;
    for (const auto& r : myRatings) {
        myMovieIds.insert(r.getMovieId());
    }

    std::map<int, int> movieScores; 
    for (size_t i = 0; i < actualK; ++i) {
        int otherId = similarities[i].first;
        std::vector<Rating> otherRatings = ratingMgr.findByUser(otherId);
        for (const auto& r : otherRatings) {
            
            if (myMovieIds.find(r.getMovieId()) == myMovieIds.end()) {
                movieScores[r.getMovieId()] += r.getScore();
            }
        }
    }

    
    std::vector<std::pair<int, int>> sortedMovies(movieScores.begin(), movieScores.end());
    std::sort(sortedMovies.begin(), sortedMovies.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;
    });

    
    std::vector<Movie> recommendedMovies;
    size_t actualN = std::min(static_cast<size_t>(n), sortedMovies.size());

    for (size_t i = 0; i < actualN; ++i) {
        int mId = sortedMovies[i].first;
        Movie m = movieMgr.findById(mId);
        if (m.getId() != -1) {
            recommendedMovies.push_back(m);
        }
    }

    return recommendedMovies;
}