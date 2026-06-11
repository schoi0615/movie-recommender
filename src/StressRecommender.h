#ifndef STRESS_RECOMMENDER_H
#define STRESS_RECOMMENDER_H

#include "MovieManager.h"
#include <vector>
#include <string>

// ================================================================
// StressRecommender — 감성 기반 스트레스 처방전 추천 시스템
//
// 사용자의 감정 상태(Affective Computing)를 고려한 감성 기반
// 추천 시스템의 요소를 도입하여, 프로그램의 휴먼 인터페이스적
// 가치를 높였습니다.
// ================================================================

enum class StressLevel {
    CALM,    //  0~40: 두뇌 가동 가능 — 추리/SF/반전 영화
    TIRED,   // 41~80: 힐링 필요     — 코미디/가족 영화
    EXPLODE  // 81~100: 폭발 직전   — 액션/팝콘 무비
};

class StressRecommender {
private:
    MovieManager& movieMgr;

    static StressLevel classify(int stress);
    static std::string getStressEmoji(int stress);
    static std::string getDiagnosis(int stress);
    static std::string getPrescriptionLabel(StressLevel level);
    static std::vector<std::string> getTargetGenres(StressLevel level);
    static std::string getAffectiveComment(StressLevel level);

public:
    explicit StressRecommender(MovieManager& mgr);

    // 스트레스 지수를 입력받아 처방전 출력
    void run() const;

    // 특정 스트레스 점수로 추천 결과를 반환 (테스트용)
    std::vector<Movie> recommend(int stress, int n = 5) const;
};

#endif
