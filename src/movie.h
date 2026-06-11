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
    std::string genre;  // M4: 장르 필드 추가

public:
    Movie() : id(0), title(""), releaseYear(0), rating(0.0), genre("Unknown") {}
    Movie(int id, const std::string& title, int releaseYear, double rating,
          const std::string& genre = "Unknown")
        : id(id), title(title), releaseYear(releaseYear), rating(rating), genre(genre) {}

    int getId() const { return id; }
    const std::string& getTitle() const { return title; }       // const& : 불필요한 복사 방지
    int getReleaseYear() const { return releaseYear; }
    double getRating() const { return rating; }
    const std::string& getGenre() const { return genre; }       // const& : 불필요한 복사 방지

    bool operator==(const Movie& other) const { return id == other.id; }
    bool operator<(const Movie& other) const { return rating < other.rating; }

    friend std::ostream& operator<<(std::ostream& os, const Movie& m) {
        os << "[" << m.id << "] " << m.title
           << " (" << m.releaseYear << ") "
           << "[" << m.genre << "] "
           << "★ " << m.rating;
        return os;
    }
};

#endif
