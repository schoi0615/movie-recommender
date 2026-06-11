#include "StressRecommender.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

StressRecommender::StressRecommender(MovieManager& mgr)
    : movieMgr(mgr) {}

// ─── 구간 분류 ────────────────────────────────────────────────────
StressLevel StressRecommender::classify(int stress) {
    if (stress <= 40)  return StressLevel::CALM;
    if (stress <= 80)  return StressLevel::TIRED;
    return StressLevel::EXPLODE;
}

std::string StressRecommender::getStressEmoji(int stress) {
    if (stress <= 20)  return "(o_O)";
    if (stress <= 40)  return "(-_-)";
    if (stress <= 60)  return "(>_<)";
    if (stress <= 80)  return "(;_;)";
    if (stress <= 95)  return "(>A<)!!";
    return "(X_X)";
}

std::string StressRecommender::getDiagnosis(int stress) {
    if (stress <= 40) {
        return "두뇌 가동 가능 상태입니다. 지금이 명작을 볼 최적의 타이밍!";
    } else if (stress <= 80) {
        return "방전 직전 상태입니다. 당신의 뇌는 지금 웃음을 원하고 있습니다.";
    } else {
        return "스트레스 지수 위험! 이이제이(以夷制夷) — 폭발로 폭발을 다스립니다.";
    }
}

std::string StressRecommender::getPrescriptionLabel(StressLevel level) {
    switch (level) {
        case StressLevel::CALM:    return "고품격 뇌 자극 요법";
        case StressLevel::TIRED:   return "힐링 & 웃음 처방";
        case StressLevel::EXPLODE: return "화끈한 이이제이 처방";
    }
    return "";
}

std::vector<std::string> StressRecommender::getTargetGenres(StressLevel level) {
    switch (level) {
        case StressLevel::CALM:
            return {"Mystery", "SF", "Thriller", "Drama"};
        case StressLevel::TIRED:
            return {"Comedy", "Family", "Animation", "Romance"};
        case StressLevel::EXPLODE:
            return {"Action", "Adventure", "Horror"};
    }
    return {};
}

std::string StressRecommender::getAffectiveComment(StressLevel level) {
    switch (level) {
        case StressLevel::CALM:
            return "* 사용자의 감정 상태(Affective Computing)를 고려한 감성 기반\n"
                   "  추천 시스템의 요소를 도입하였습니다.\n"
                   "  현재 상태: 고인지 부하 허용 → 복잡한 서사 구조 영화 최적화";
        case StressLevel::TIRED:
            return "* 사용자의 감정 상태(Affective Computing)를 고려한 감성 기반\n"
                   "  추천 시스템의 요소를 도입하였습니다.\n"
                   "  현재 상태: 저인지 부하 선호 → 긍정 정서 유발 콘텐츠 최적화";
        case StressLevel::EXPLODE:
            return "* 사용자의 감정 상태(Affective Computing)를 고려한 감성 기반\n"
                   "  추천 시스템의 요소를 도입하였습니다.\n"
                   "  현재 상태: 각성 수준 최고조 → 고자극 콘텐츠로 감정 카타르시스 유도";
    }
    return "";
}


std::vector<Movie> StressRecommender::recommend(int stress, int n) const {
    if (stress < 0 || stress > 100) {
        throw std::out_of_range("스트레스 지수는 0~100 사이여야 합니다.");
    }

    StressLevel level = classify(stress);
    auto targetGenres = getTargetGenres(level);

    // 해당 장르 영화 수집
    std::vector<Movie> candidates;
    for (const auto& genre : targetGenres) {
        auto filtered = movieMgr.filterByGenre(genre);
        candidates.insert(candidates.end(), filtered.begin(), filtered.end());
    }

    // 중복 제거 후 평점 내림차순 정렬
    std::sort(candidates.begin(), candidates.end(),
        [](const Movie& a, const Movie& b) {
            return a.getRating() > b.getRating();
        });
    candidates.erase(
        std::unique(candidates.begin(), candidates.end()),
        candidates.end());

    // 상위 n개 반환
    if (n > static_cast<int>(candidates.size())) {
        n = static_cast<int>(candidates.size());
    }
    return std::vector<Movie>(candidates.begin(), candidates.begin() + n);
}

// ─── 처방전 출력 ──────────────────────────────────────────────────
void StressRecommender::run() const {
    std::cout << "\n╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║ 🎬  스트레스 처방전 영화 추천 시스템  🎬 ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;
    std::cout << "\n  현재 스트레스 지수를 입력하세요 (0 ~ 100): ";

    int stress;
    while (true) {
        if (std::cin >> stress && stress >= 0 && stress <= 100) break;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "  0~100 사이의 숫자를 입력해주세요: ";
    }

    StressLevel level = classify(stress);

    // ── 진단서 출력 ──
    std::cout << "\n  ┌──────────────────────────────────────────┐" << std::endl;
    std::cout << "  │              진  단  서                    │" << std::endl;
    std::cout << "  ├──────────────────────────────────────────┤" << std::endl;
    std::cout << "  │ 스트레스 지수 : " << stress << "% " << getStressEmoji(stress) << std::endl;
    std::cout << "  │" << std::endl;
    std::cout << "  │ [진단]" << std::endl;
    std::cout << "  │ " << getDiagnosis(stress) << std::endl;
    std::cout << "  │" << std::endl;
    std::cout << "  │ [처방] " << getPrescriptionLabel(level) << std::endl;

    auto targetGenres = getTargetGenres(level);
    std::cout << "  │ 추천 장르: ";
    for (size_t i = 0; i < targetGenres.size(); ++i) {
        std::cout << targetGenres[i];
        if (i + 1 < targetGenres.size()) std::cout << " / ";
    }
    std::cout << std::endl;
    std::cout << "  └──────────────────────────────────────────┘" << std::endl;

   
    try {
        auto recs = recommend(stress, 5);

        if (recs.empty()) {
            std::cout << "\n  해당 장르의 영화 데이터가 없습니다." << std::endl;
            std::cout << "  (영화를 먼저 추가해주세요!)" << std::endl;
        } else {
            std::cout << "\n  ★ 처방 영화 TOP " << recs.size() << " ★" << std::endl;
            for (size_t i = 0; i < recs.size(); ++i) {
                std::cout << "  " << (i + 1) << "위: " << recs[i] << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cout << "  오류: " << e.what() << std::endl;
    }


    std::cout << "\n  ─────────────────────────────────────────────" << std::endl;
    std::cout << "  " << getAffectiveComment(level) << std::endl;
    std::cout << "  ─────────────────────────────────────────────" << std::endl;
}
