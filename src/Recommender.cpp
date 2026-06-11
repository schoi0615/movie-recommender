#include "Recommender.h"
#include "SimilarityCalculator.h"
#include <algorithm>
#include <set>
#include <map>
#include <iostream>

// 유사도 계산에서 "공통 평점 없음"을 나타내는 sentinel 값
// SimilarityCalculator::calculate()가 반환하는 불가능 값과 동일
static constexpr int NO_COMMON_RATING  = -100;
// 공통 영화 1개당 기본 유사도 점수
static constexpr int SIMILARITY_WEIGHT = 10;

Recommender::Recommender(MovieManager& mMgr, RatingManager& rMgr)
    : movieMgr(mMgr), ratingMgr(rMgr) {}

// SimilarityCalculator에 위임 — 중복 구현 제거
int Recommender::calculateSimilarity(const std::vector<Rating>& ratingsA,
                                     const std::vector<Rating>& ratingsB) {
    return SimilarityCalculator::calculate(ratingsA, ratingsB);
}

std::vector<Movie> Recommender::recommend(int userId, int k, int n) {
    std::vector<Rating> myRatings = ratingMgr.findByUser(userId);
    if (myRatings.empty()) {
        std::cout << "[안내] 해당 유저의 평점 데이터가 없어 추천이 불가능합니다." << std::endl;
        return std::vector<Movie>();
    }

    // 다른 모든 유저와의 유사도 계산
    std::vector<std::pair<int, int>> similarities;
    std::vector<int> allUserIds = ratingMgr.getAllUserIds();

    for (int otherId : allUserIds) {
        if (otherId == userId) continue;
        std::vector<Rating> otherRatings = ratingMgr.findByUser(otherId);
        int sim = calculateSimilarity(myRatings, otherRatings);
        if (sim != NO_COMMON_RATING) {
            similarities.push_back({otherId, sim});
        }
    }

    // 유사도 내림차순 정렬
    std::sort(similarities.begin(), similarities.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second > b.second;
        });

    size_t actualK = std::min(static_cast<size_t>(k), similarities.size());

    // 내가 이미 본 영화 집합
    std::set<int> myMovieIds;
    for (const auto& r : myRatings) {
        myMovieIds.insert(r.getMovieId());
    }

    // 이웃 K명이 준 점수 합산 (내가 안 본 영화만)
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

    // 점수 내림차순 정렬
    std::vector<std::pair<int, int>> sortedMovies(movieScores.begin(), movieScores.end());
    std::sort(sortedMovies.begin(), sortedMovies.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second > b.second;
        });

    // 상위 N개 반환
    std::vector<Movie> recommendedMovies;
    size_t actualN = std::min(static_cast<size_t>(n), sortedMovies.size());
    for (size_t i = 0; i < actualN; ++i) {
        Movie m = movieMgr.findById(sortedMovies[i].first);
        if (m.getId() != -1) {
            recommendedMovies.push_back(m);
        }
    }
    return recommendedMovies;
}
