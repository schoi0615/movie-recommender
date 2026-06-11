#include "MovieManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <set>

void MovieManager::addMovie(const Movie& movie) {
    // ID 중복 검사
    for (const auto& m : movies) {
        if (m.getId() == movie.getId()) {
            throw std::invalid_argument(
                "ID " + std::to_string(movie.getId()) + "는 이미 존재합니다.");
        }
    }
    movies.push_back(movie);
}

void MovieManager::printAll() const {
    if (movies.empty()) {
        std::cout << "  등록된 영화가 없습니다." << std::endl;
        return;
    }
    for (const auto& m : movies) {
        std::cout << "  " << m << std::endl;
    }
}

void MovieManager::sortByRating() {
    std::sort(movies.begin(), movies.end(), [](const Movie& a, const Movie& b) {
        return b < a;
    });
}

// 원본을 건드리지 않고 정렬된 복사본 반환 (const 보장)
std::vector<Movie> MovieManager::getSortedByRating() const {
    auto sorted = movies;
    std::sort(sorted.begin(), sorted.end(), [](const Movie& a, const Movie& b) {
        return a.getRating() > b.getRating();
    });
    return sorted;
}

void MovieManager::searchByTitle(const std::string& title) const {
    bool found = false;
    for (const auto& m : movies) {
        if (m.getTitle().find(title) != std::string::npos) {
            std::cout << "  " << m << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "  검색 결과가 없습니다." << std::endl;
    }
}

Movie MovieManager::findById(int id) const {
    for (const auto& m : movies) {
        if (m.getId() == id) return m;
    }
    return Movie(-1, "Unknown", 0, 0.0);
}

// ─── M4: 장르 필터 ───────────────────────────────────────────────
std::vector<Movie> MovieManager::filterByGenre(const std::string& genre) const {
    std::vector<Movie> result;
    std::copy_if(movies.begin(), movies.end(),
                 std::back_inserter(result),
                 [&genre](const Movie& m) {
                     return m.getGenre() == genre;
                 });
    return result;
}

void MovieManager::printByGenre(const std::string& genre) const {
    auto filtered = filterByGenre(genre);
    if (filtered.empty()) {
        std::cout << "  [" << genre << "] 장르의 영화가 없습니다." << std::endl;
        return;
    }
    std::cout << "\n  --- [" << genre << "] 장르 영화 목록 (" << filtered.size() << "편) ---" << std::endl;
    for (const auto& m : filtered) {
        std::cout << "  " << m << std::endl;
    }
}

std::vector<std::string> MovieManager::getAvailableGenres() const {
    std::set<std::string> genreSet;
    for (const auto& m : movies) {
        genreSet.insert(m.getGenre());
    }
    return std::vector<std::string>(genreSet.begin(), genreSet.end());
}

// ─── M4: 통계 ────────────────────────────────────────────────────
double MovieManager::getAverageRating() const {
    if (movies.empty()) {
        throw std::runtime_error("영화 데이터가 없습니다.");
    }
    double sum = std::accumulate(
        movies.begin(), movies.end(), 0.0,
        [](double acc, const Movie& m) { return acc + m.getRating(); });
    return sum / movies.size();
}

std::map<std::string, double> MovieManager::getAverageRatingByGenre() const {
    std::map<std::string, double> sumByGenre;
    std::map<std::string, int>    countByGenre;

    for (const auto& movie : movies) {
        sumByGenre[movie.getGenre()]   += movie.getRating();
        countByGenre[movie.getGenre()]++;
    }

    std::map<std::string, double> avgByGenre;
    for (const auto& [genre, sum] : sumByGenre) {   
        avgByGenre[genre] = sum / countByGenre[genre];
    }
    return avgByGenre;
}

std::vector<Movie> MovieManager::getTopN(int n) const {
    if (n <= 0 || movies.empty()) return {};

    // n이 전체 영화 개수보다 크면 영화 개수만큼만 처리
    size_t count = std::min(static_cast<size_t>(n), movies.size());
    
    std::vector<Movie> result = movies; // 원본 보호를 위한 복사
    
    // 전체 정렬 대신 상위 count개까지만 정렬하여 성능 극대화
    std::partial_sort(result.begin(), result.begin() + count, result.end(),
        [](const Movie& a, const Movie& b) {
            return a.getRating() > b.getRating();   // 내림차순
        });
        
    // 상위 count개만 남기고 배열 자르기
    result.resize(count);
    return result;
}

void MovieManager::printStatistics() const {
    if (movies.empty()) {
        std::cout << "  통계를 계산할 영화 데이터가 없습니다." << std::endl;
        return;
    }

    std::cout << "\n  ┌─────────────────────────────────────┐" << std::endl;
    std::cout << "  │           영화 데이터 통계              │" << std::endl;
    std::cout << "  ├─────────────────────────────────────┤" << std::endl;
    std::cout << "  │ 총 영화 수  : " << movies.size() << "편" << std::endl;

    try {
        std::cout << "  │ 전체 평균 평점 : " << getAverageRating() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  │ 전체 평균 평점 : 계산 불가" << std::endl;
    }

    std::cout << "  ├─────────────────────────────────────┤" << std::endl;
    std::cout << "  │ [장르별 평균 평점]" << std::endl;
    auto avgByGenre = getAverageRatingByGenre();
    for (const auto& [genre, avg] : avgByGenre) {
        std::cout << "  │   " << genre << " : " << avg << std::endl;
    }

    std::cout << "  ├─────────────────────────────────────┤" << std::endl;
    std::cout << "  │ [평점 TOP 5]" << std::endl;
    auto top5 = getTopN(5);
    for (size_t i = 0; i < top5.size(); ++i) {
        std::cout << "  │  " << (i + 1) << "위: "
                  << top5[i].getTitle()
                  << " ★" << top5[i].getRating() << std::endl;
    }
    std::cout << "  └─────────────────────────────────────┘" << std::endl;
}

// ─── 파일 I/O ────────────────────────────────────────────────────
void MovieManager::loadFromFile(const std::string& filename) {
    movies.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 파일을 열 수 없습니다." << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line); // 헤더 건너뛰기
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;

        std::getline(ss, token, ','); int id    = std::stoi(token);
        std::getline(ss, token, ','); std::string title = token;
        std::getline(ss, token, ','); int year  = std::stoi(token);
        std::getline(ss, token, ','); double rating = std::stod(token);
        std::string genre = "Unknown";
        if (std::getline(ss, token, ',')) genre = token;

        movies.push_back(Movie(id, title, year, rating, genre));
    }
    file.close();
    std::cout << filename << " 로드 완료: " << movies.size() << "건" << std::endl;
}

void MovieManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 저장 실패" << std::endl;
        return;
    }
    file << "id,title,year,rating,genre\n";
    for (const auto& m : movies) {
        file << m.getId() << ","
             << m.getTitle() << ","
             << m.getReleaseYear() << ","
             << m.getRating() << ","
             << m.getGenre() << "\n";
    }
    file.close();
    std::cout << filename << " 저장 완료: " << movies.size() << "건" << std::endl;
}

int MovieManager::size() const {
    return static_cast<int>(movies.size());
}

// ─── M4: 통계 서브메뉴 (PPT 슬라이드 21 패턴: while + try-catch) ───
void MovieManager::showStatisticsMenu() const {
    while (true) {
        std::cout << "\n  === 통계 메뉴 ===" << std::endl;
        std::cout << "  1. 전체 평균 평점" << std::endl;
        std::cout << "  2. 장르별 평균 평점" << std::endl;
        std::cout << "  3. 평점 TOP 10" << std::endl;
        std::cout << "  0. 돌아가기" << std::endl;
        std::cout << "  선택 > ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "  잘못된 입력입니다." << std::endl;
            continue;
        }

        try {
            switch (choice) {
                case 1: {
                    double avg = getAverageRating();
                    std::cout << "\n  전체 평균 평점: " << avg << std::endl;
                    break;
                }
                case 2: {
                    auto avgByGenre = getAverageRatingByGenre();
                    if (avgByGenre.empty()) {
                        std::cout << "  데이터가 없습니다." << std::endl;
                        break;
                    }
                    std::cout << "\n  [장르별 평균 평점]" << std::endl;
                    for (const auto& [genre, avg] : avgByGenre) {
                        std::cout << "   " << genre << " : " << avg << std::endl;
                    }
                    break;
                }
                case 3: {
                    auto top10 = getTopN(10);
                    if (top10.empty()) {
                        std::cout << "  데이터가 없습니다." << std::endl;
                        break;
                    }
                    std::cout << "\n  [평점 TOP " << top10.size() << "]" << std::endl;
                    for (size_t i = 0; i < top10.size(); ++i) {
                        std::cout << "  " << (i + 1) << "위: "
                                  << top10[i].getTitle()
                                  << " ★" << top10[i].getRating() << std::endl;
                    }
                    break;
                }
                case 0:
                    return;
                default:
                    std::cout << "  잘못된 선택입니다." << std::endl;
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "  오류: " << e.what() << std::endl;
        }
    }
}
