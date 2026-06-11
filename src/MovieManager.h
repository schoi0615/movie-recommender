#ifndef MOVIE_MANAGER_H
#define MOVIE_MANAGER_H

#include "BaseManager.h"
#include "movie.h"
#include <vector>
#include <string>
#include <map>

class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;

public:
    void addMovie(const Movie& movie);
    void printAll() const;
    void sortByRating();                          // 원본 정렬 (메뉴 4용)
    std::vector<Movie> getSortedByRating() const; // 원본 보호 정렬 복사본 반환
    void searchByTitle(const std::string& title) const;
    Movie findById(int id) const;

    // M4: 장르 필터
    std::vector<Movie> filterByGenre(const std::string& genre) const;
    void printByGenre(const std::string& genre) const;
    std::vector<std::string> getAvailableGenres() const;

    // M4: 통계
    double getAverageRating() const;
    std::map<std::string, double> getAverageRatingByGenre() const;
    std::vector<Movie> getTopN(int n) const;
    void printStatistics() const;
    void showStatisticsMenu() const;  

    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
    int size() const override;
};

#endif
