#include "SimilarityCalculator.h"
#include <cstdlib> 

int SimilarityCalculator::calculate(const std::vector<Rating>& user1, const std::vector<Rating>& user2) {
    const int NO_SIMILARITY = -100;
    const int WEIGHT_MULTIPLIER = 10;
    
    int commonCount = 0;
    int scoreDiffSum = 0;

    for (const Rating& r1 : user1) {
        for (const Rating& r2 : user2) {
            if (r1.getMovieId() == r2.getMovieId()) {
                commonCount++;
                scoreDiffSum += std::abs((int)r1.getScore() - (int)r2.getScore());
            }
        }
    }

    if (commonCount == 0) {
        return NO_SIMILARITY; // 매직 넘버 제거
    }

    return (commonCount * WEIGHT_MULTIPLIER) - scoreDiffSum; // 매직 넘버 제거
}