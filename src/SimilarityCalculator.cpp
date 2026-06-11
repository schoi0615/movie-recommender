#include "SimilarityCalculator.h"
#include <cstdlib>

// 공통 평점 없을 때 반환하는 sentinel (불가능한 정상 점수)
static constexpr int NO_COMMON_RATING  = -100;
// 공통 영화 1개당 기본 유사도 가중치
static constexpr int SIMILARITY_WEIGHT = 10;

int SimilarityCalculator::calculate(const std::vector<Rating>& user1,
                                    const std::vector<Rating>& user2) {
    int commonCount  = 0;
    int scoreDiffSum = 0;

    for (const Rating& r1 : user1) {
        for (const Rating& r2 : user2) {
            if (r1.getMovieId() == r2.getMovieId()) {
                ++commonCount;
                scoreDiffSum += std::abs(r1.getScore() - r2.getScore());
            }
        }
    }

    if (commonCount == 0) {
        return NO_COMMON_RATING;
    }
    // 공통 영화가 많고 평점 차이가 작을수록 높은 점수
    return (commonCount * SIMILARITY_WEIGHT) - scoreDiffSum;
}
