#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"
#include <iostream>
#include <string>

void printMenu() {
    std::cout << "\n=== 영화 추천 프로그램 ===" << std::endl;
    std::cout << "\n[영화 관리]" << std::endl;
    std::cout << "1. 영화 추가" << std::endl;
    std::cout << "2. 제목으로 검색" << std::endl;
    std::cout << "3. 전체 목록 출력" << std::endl;
    std::cout << "4. 평점순 정렬 출력" << std::endl;
    std::cout << "\n[사용자 관리]" << std::endl;
    std::cout << "5. 사용자 추가" << std::endl;
    std::cout << "6. 사용자 목록 출력" << std::endl;
    std::cout << "\n[평점 관리]" << std::endl;
    std::cout << "7. 평점 입력" << std::endl;
    std::cout << "8. 영화별 평점 보기" << std::endl;
    std::cout << "\n[추천 기능]" << std::endl;
    std::cout << "9. 맞춤 영화 추천받기" << std::endl;
    std::cout << "\n0. 종료" << std::endl;
    std::cout << "\n선택 > ";
}

int main() {
    MovieManager movieMgr;
    UserManager userMgr;
    RatingManager ratingMgr;
    Recommender recommender(movieMgr, ratingMgr);

    // 프로그램 시작 시 파일에서 데이터 로드 [cite: 697, 785]
    movieMgr.loadFromFile("data/movies.csv");
    userMgr.loadFromFile("data/users.csv");
    ratingMgr.loadFromFile("data/ratings.csv");

    int choice;
    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "잘못된 입력입니다. 번호를 다시 입력해주세요." << std::endl;
            continue;
        }

        if (choice == 0) {
            std::cout << "프로그램을 종료합니다. 데이터를 저장하는 중..." << std::endl;
            movieMgr.saveToFile("data/movies.csv");
            userMgr.saveToFile("data/users.csv");
            ratingMgr.saveToFile("data/ratings.csv");
            break;
        }

        switch (choice) {
            case 1: {
                int id, year;
                std::string title;
                double rating;

                std::cout << "영화 ID (숫자): ";
                while (!(std::cin >> id)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "올바른 숫자가 아닙니다. 영화 ID를 숫자로 입력해주세요: ";
                }
                std::cin.ignore(); // 버퍼에 남은 엔터키 제거

                std::cout << "영화 제목: ";
                std::getline(std::cin, title);

                std::cout << "개봉 연도 (숫자): ";
                while (!(std::cin >> year)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "올바른 숫자가 아닙니다. 개봉 연도를 숫자로 입력해주세요: ";
                }

                std::cout << "기본 평점 (숫자): ";
                while (!(std::cin >> rating)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "올바른 숫자가 아닙니다. 평점을 숫자로 입력해주세요: ";
                }

                movieMgr.addMovie(Movie(id, title, year, rating));
                std::cout << "영화가 추가되었습니다." << std::endl;
                break;
            }
            case 2: {
                std::string title;
                std::cin.ignore();
                std::cout << "검색할 제목 입력: ";
                std::getline(std::cin, title);
                movieMgr.searchByTitle(title);
                break;
            }
            case 3:
                std::cout << "\n--- 전체 영화 목록 ---" << std::endl;
                movieMgr.printAll();
                break;
            case 4:
                std::cout << "\n--- 평점순 영화 목록 ---" << std::endl;
                movieMgr.sortByRating();
                movieMgr.printAll();
                break;
            case 5: {
                int id, age;
                std::string name;

                std::cout << "사용자 ID (숫자): ";
                while (!(std::cin >> id)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "올바른 숫자가 아닙니다. 사용자 ID를 숫자로 입력해주세요: ";
                }
                std::cin.ignore();

                std::cout << "이름: ";
                std::getline(std::cin, name);

                std::cout << "나이 (숫자): ";
                while (!(std::cin >> age)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "올바른 숫자가 아닙니다. 나이를 숫자로 입력해주세요: ";
                }

                userMgr.addUser(User(id, name, age));
                std::cout << "사용자가 추가되었습니다." << std::endl;
                break;
            }
            case 6:
                std::cout << "\n--- 전체 사용자 목록 ---" << std::endl;
                userMgr.printAll();
                break;
            case 7: {
                int uId, mId, score;

                std::cout << "유저 ID (숫자): ";
                while (!(std::cin >> uId)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "올바른 숫자가 아닙니다. 유저 ID를 숫자로 입력해주세요: ";
                }

                std::cout << "영화 ID (숫자): ";
                while (!(std::cin >> mId)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "올바른 숫자가 아닙니다. 영화 ID를 숫자로 입력해주세요: ";
                }

                while (true) {
                    std::cout << "평점 (1~5 사이 숫자): ";
                    if (std::cin >> score && score >= 1 && score <= 5) {
                        break;
                    }
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "잘못된 입력입니다. " << std::endl;
                }

                ratingMgr.addRating(Rating(uId, mId, score));
                std::cout << "평점이 등록되었습니다." << std::endl;
                break;
            }
            case 8: {
                int mId;
                std::cout << "영화 ID 입력: ";
                while (!(std::cin >> mId)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "올바른 숫자가 아닙니다. 영화 ID를 숫자로 입력해주세요: ";
                }
                std::cout << "\n--- 선택한 영화의 평점 목록 ---" << std::endl;
                ratingMgr.printRatingsForMovie(mId);
                break;
            }
            case 9: {
                int uId, k, n;

                std::cout << "추천 대상 유저 ID (숫자): ";
                while (!(std::cin >> uId)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "올바른 숫자가 아닙니다. 유저 ID를 숫자로 입력해주세요: ";
                }

                std::cout << "참고할 이웃 수(K) (숫자): ";
                while (!(std::cin >> k)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "올바른 숫자가 아닙니다. 이웃 수를 숫자로 입력해주세요: ";
                }

                std::cout << "추천받을 영화 수(N) (숫자): ";
                while (!(std::cin >> n)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "올바른 숫자가 아닙니다. 추천 영화 수를 숫자로 입력해주세요: ";
                }

                std::vector<Movie> recommendations = recommender.recommend(uId, k, n);

                if (recommendations.empty()) {
                    std::cout << "추천할 만한 영화가 없습니다." << std::endl;
                } else {
                    std::cout << "\n--- 유저 " << uId << "번 맞춤 추천 영화 TOP " << recommendations.size() << " ---" << std::endl;
                    for (size_t i = 0; i < recommendations.size(); ++i) {
                        std::cout << i + 1 << "위: " << recommendations[i] << std::endl;
                    }
                }
                break;
            }
            default:
                std::cout << "잘못된 선택입니다. 다시 선택해주세요." << std::endl;
                break;
        }
    }
    return 0;
}