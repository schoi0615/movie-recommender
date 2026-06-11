#include "Recommender.h"
#include "SimilarityCalculator.h"
#include <algorithm>
#include <set>
#include <map>
#include <iostream>

static constexpr int NO_COMMON_RATING = -100;

Recommender::Recommender(MovieManager& mMgr, RatingManager& rMgr)
    : movieMgr(mMgr), ratingMgr(rMgr) {}

int Recommender::calculateSimilarity(const std::vector<Rating>& ratingsA,
                                     const std::vector<Rating>& ratingsB) {
    return SimilarityCalculator::calculate(ratingsA, ratingsB);
}

std::vector<std::pair<Movie, int>> Recommender::recommend(int userId, int k, int n) {
    std::vector<Rating> myRatings = ratingMgr.findByUser(userId);
    if (myRatings.empty()) return {};

    std::vector<std::pair<int, int>> similarities;
    std::vector<int> allUserIds = ratingMgr.getAllUserIds();

    for (int otherId : allUserIds) {
        if (otherId == userId) continue;
        int sim = calculateSimilarity(myRatings, ratingMgr.findByUser(otherId));
        if (sim != NO_COMMON_RATING) {
            similarities.push_back({otherId, sim});
        }
    }

    std::sort(similarities.begin(), similarities.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second > b.second;
        });

    size_t actualK = std::min(static_cast<size_t>(k), similarities.size());
    std::set<int> myMovieIds;
    for (const auto& r : myRatings) myMovieIds.insert(r.getMovieId());

    std::map<int, int> movieScores;
    for (size_t i = 0; i < actualK; ++i) {
        int otherId = similarities[i].first;
        for (const auto& r : ratingMgr.findByUser(otherId)) {
            if (myMovieIds.find(r.getMovieId()) == myMovieIds.end()) {
                movieScores[r.getMovieId()] += r.getScore();
            }
        }
    }

    std::vector<std::pair<int, int>> sortedMovies(movieScores.begin(), movieScores.end());
    std::sort(sortedMovies.begin(), sortedMovies.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second > b.second;
        });

    std::vector<std::pair<Movie, int>> recommendedMovies;
    size_t actualN = std::min(static_cast<size_t>(n), sortedMovies.size());
    for (size_t i = 0; i < actualN; ++i) {
        Movie m = movieMgr.findById(sortedMovies[i].first);
        if (m.getId() != -1) {
            // sortedMovies[i].second에 추천 점수(또는 유사도 점수)를 담아 리턴
            recommendedMovies.push_back({m, sortedMovies[i].second});
        }
    }
    return recommendedMovies;
}