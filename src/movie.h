#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <iostream>

class Movie {
private:
    int id;
    std::string title;
    int releaseYear;
    double rating;

public:
    // 생성자 선언
    Movie();
    Movie(int id, const std::string& title, int releaseYear, double rating);

    // Getter 선언
    int getId() const;
    std::string getTitle() const;
    int getReleaseYear() const;
    double getRating() const;

    // 연산자 오버로딩 선언
    bool operator==(const Movie& other) const;
    bool operator<(const Movie& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Movie& m);
};

#endif