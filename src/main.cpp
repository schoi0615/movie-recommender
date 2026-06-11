#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"
#include "StressRecommender.h"
#include <iostream>
#include <string>

void printMenu() {
    std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║      영화 추천 프로그램  M4 Edition  ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    std::cout << "\n  [영화 관리]" << std::endl;
    std::cout << "   1. 영화 추가" << std::endl;
    std::cout << "   2. 제목으로 검색" << std::endl;
    std::cout << "   3. 전체 목록 출력" << std::endl;
    std::cout << "   4. 평점순 정렬 출력" << std::endl;
    std::cout << "\n  [사용자 관리]" << std::endl;
    std::cout << "   5. 사용자 추가" << std::endl;
    std::cout << "   6. 사용자 목록 출력" << std::endl;
    std::cout << "\n  [평점 관리]" << std::endl;
    std::cout << "   7. 평점 입력" << std::endl;
    std::cout << "   8. 영화별 평점 보기" << std::endl;
    std::cout << "\n  [추천 기능]" << std::endl;
    std::cout << "   9. 맞춤 영화 추천받기 (협업 필터링)" << std::endl;
    std::cout << "\n  [M4 확장 기능]" << std::endl;
    std::cout << "  10. 장르 필터 — 원하는 장르 영화만 보기" << std::endl;
    std::cout << "  11. 통계 보기 — 평균 평점 / 장르별 분석 / TOP 10" << std::endl;
    std::cout << "  12. 스트레스 처방전 영화 추천" << std::endl;
    std::cout << "\n   0. 종료" << std::endl;
    std::cout << "\n  선택 > ";
}

int main() {
    MovieManager  movieMgr;
    UserManager   userMgr;
    RatingManager ratingMgr;
    Recommender       recommender(movieMgr, ratingMgr);
    StressRecommender stressRec(movieMgr);

    constexpr int MAX_BUFFER_SIZE = 1000; // 입력 버퍼 최대 크기

    movieMgr.loadFromFile("data/movies.csv");
    userMgr.loadFromFile("data/users.csv");
    ratingMgr.loadFromFile("data/ratings.csv");

    int choice;
    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(MAX_BUFFER_SIZE, '\n');
            std::cout << "잘못된 입력입니다. 번호를 다시 입력해주세요." << std::endl;
            continue;
        }

        if (choice == 0) {
            std::cout << "프로그램을 종료합니다. 데이터 저장 중..." << std::endl;
            movieMgr.saveToFile("data/movies.csv");
            userMgr.saveToFile("data/users.csv");
            ratingMgr.saveToFile("data/ratings.csv");
            break;
        }

        try {
            switch (choice) {

                // ─── 영화 관리 ──────────────────────────────────────────
                case 1: {
                    int id, year;
                    double rating;
                    std::string title, genre;

                    std::cout << "영화 ID (숫자): ";
                    while (!(std::cin >> id)) {
                        std::cin.clear(); std::cin.ignore(MAX_BUFFER_SIZE, '\n');
                        std::cout << "올바른 숫자를 입력해주세요: ";
                    }
                    std::cin.ignore();

                    std::cout << "영화 제목: ";
                    std::getline(std::cin, title);

                    std::cout << "개봉 연도: ";
                    while (!(std::cin >> year)) {
                        std::cin.clear(); std::cin.ignore(MAX_BUFFER_SIZE, '\n');
                        std::cout << "올바른 숫자를 입력해주세요: ";
                    }

                    std::cout << "기본 평점 (0.0~5.0): ";
                    while (!(std::cin >> rating)) {
                        std::cin.clear(); std::cin.ignore(MAX_BUFFER_SIZE, '\n');
                        std::cout << "올바른 숫자를 입력해주세요: ";
                    }
                    std::cin.ignore();

                    std::cout << "장르 (예: Action/Comedy/Drama/Mystery/SF/Thriller/Family/Animation/Romance/Horror): ";
                    std::getline(std::cin, genre);
                    if (genre.empty()) genre = "Unknown";

                    movieMgr.addMovie(Movie(id, title, year, rating, genre));
                    std::cout << "영화가 추가되었습니다." << std::endl;
                    break;
                }
                case 2: {
                    std::string title;
                    std::cin.ignore();
                    std::cout << "검색할 제목: ";
                    std::getline(std::cin, title);
                    movieMgr.searchByTitle(title);
                    break;
                }
                case 3:
                    std::cout << "\n--- 전체 영화 목록 ---" << std::endl;
                    movieMgr.printAll();
                    break;
                case 4: {
                    // getSortedByRating(): 원본 보호 — 원본 배열 순서 변경 없음
                    std::cout << "\n--- 평점순 영화 목록 ---" << std::endl;
                    auto sorted = movieMgr.getSortedByRating();
                    for (const auto& m : sorted) {
                        std::cout << "  " << m << std::endl;
                    }
                    break;
                }

                // ─── 사용자 관리 ────────────────────────────────────────
                case 5: {
                    int id, age;
                    std::string name;

                    std::cout << "사용자 ID (숫자): ";
                    while (!(std::cin >> id)) {
                        std::cin.clear(); std::cin.ignore(MAX_BUFFER_SIZE, '\n');
                        std::cout << "올바른 숫자를 입력해주세요: ";
                    }
                    std::cin.ignore();

                    std::cout << "이름: ";
                    std::getline(std::cin, name);

                    std::cout << "나이 (숫자): ";
                    while (!(std::cin >> age)) {
                        std::cin.clear(); std::cin.ignore(MAX_BUFFER_SIZE, '\n');
                        std::cout << "올바른 숫자를 입력해주세요: ";
                    }

                    userMgr.addUser(User(id, name, age));
                    std::cout << "사용자가 추가되었습니다." << std::endl;
                    break;
                }
                case 6:
                    std::cout << "\n--- 전체 사용자 목록 ---" << std::endl;
                    userMgr.printAll();
                    break;

                // ─── 평점 관리 ──────────────────────────────────────────
                case 7: {
                    int uId, mId;
                    double score; 

                    std::cout << "유저 ID: ";
                    while (!(std::cin >> uId)) {
                        std::cin.clear(); std::cin.ignore(MAX_BUFFER_SIZE, '\n');
                        std::cout << "올바른 숫자를 입력해주세요: ";
                    }
                    std::cout << "영화 ID: ";
                    while (!(std::cin >> mId)) {
                        std::cin.clear(); std::cin.ignore(MAX_BUFFER_SIZE, '\n');
                        std::cout << "올바른 숫자를 입력해주세요: ";
                    }
                    
                    while (true) {
                        std::cout << "평점 (1.0~5.0): "; 
                        // double로 입력받고 버퍼의 남은 찌꺼기를 전부 무시함
                        if (std::cin >> score && score >= 1.0 && score <= 5.0) {
                            std::cin.ignore(MAX_BUFFER_SIZE, '\n'); 
                            break;
                        }
                        std::cin.clear();
                        std::cin.ignore(MAX_BUFFER_SIZE, '\n');
                        std::cout << "1.0~5.0 사이 값을 입력해주세요." << std::endl;
                    }
                    
                    std::cout << "평점이 등록되었습니다." << std::endl;
                    break;
                }
                
                case 8: {
                    int mId;
                    std::cout << "영화 ID 입력: ";
                    while (!(std::cin >> mId)) {
                        std::cin.clear(); std::cin.ignore(MAX_BUFFER_SIZE, '\n');
                        std::cout << "올바른 숫자를 입력해주세요: ";
                    }
                    std::cout << "\n--- 선택한 영화의 평점 목록 ---" << std::endl;
                    ratingMgr.printRatingsForMovie(mId);
                    break;
                }

                // ─── 추천 기능 ──────────────────────────────────────────
                case 9: {
                    int uId, k, n;
                    std::cout << "추천 대상 유저 ID: "; std::cin >> uId;
                    std::cout << "참고할 이웃 수(K): "; std::cin >> k;
                    std::cout << "추천받을 영화 수(N): "; std::cin >> n;

                     auto recs = recommender.recommend(uId, k, n);
                     if (recs.empty()) {
                        std::cout << "추천할 영화가 없습니다." << std::endl;
                    } else {
                        std::cout << "\n--- 유저 " << uId << "번 맞춤 추천 TOP " << recs.size() << " ---" << std::endl;
                        for (const auto& item : recs) {
                            std::cout << item.first.getTitle() << " [추천 점수: " << item.second << "점]" << std::endl;
                        }
                    }
                break;
}

                // ─── M4 확장: 장르 필터 ─────────────────────────────────
                case 10: {
                    auto genres = movieMgr.getAvailableGenres();
                    if (genres.empty()) {
                        std::cout << "  등록된 장르가 없습니다." << std::endl;
                        break;
                    }
                    std::cout << "\n  사용 가능한 장르:" << std::endl;
                    for (size_t i = 0; i < genres.size(); ++i) {
                        std::cout << "   " << (i + 1) << ". " << genres[i] << std::endl;
                    }
                    
                    std::cout << "\n  원하는 장르 번호를 입력하세요: ";
                    int genreChoice;
                    
                    // 숫자 입력을 확인하고, 1번부터 장르 개수 사이의 값인지 검사
                    if (!(std::cin >> genreChoice) || genreChoice < 1 || genreChoice > static_cast<int>(genres.size())) {
                        std::cin.clear();
                        std::cin.ignore(MAX_BUFFER_SIZE, '\n');
                        std::cout << "  잘못된 번호입니다. 메뉴로 돌아갑니다." << std::endl;
                        break;
                    }
                    
                    // 사용자가 입력한 번호에 맞는 장르 텍스트를 찾아서 넘겨줌 (배열 인덱스는 0부터 시작하므로 -1 해줌)
                    std::string selectedGenre = genres[genreChoice - 1];
                    movieMgr.printByGenre(selectedGenre);
                    break;
                }

                // ─── M4 확장: 통계 서브메뉴 (PPT 슬라이드 21) ──────────
                case 11:
                    movieMgr.showStatisticsMenu();
                    break;

                // ─── M4 확장: 스트레스 처방전 ───────────────────────────
                case 12:
                    stressRec.run();
                    break;

                default:
                    std::cout << "잘못된 선택입니다. 다시 선택해주세요." << std::endl;
                    break;
            }
        } catch (const std::invalid_argument& e) {
            // addMovie() ID 중복 등 입력 오류
            std::cerr << "  입력 오류: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "  오류: " << e.what() << std::endl;
        }
    }
    return 0;
}