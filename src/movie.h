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
    Movie() : id(0), title(""), releaseYear(0), rating(0.0) {}
    Movie(int id, const std::string& title, int releaseYear, double rating)
        : id(id), title(title), releaseYear(releaseYear), rating(rating) {}

    int getId() const { return id; }
    std::string getTitle() const { return title; }
    int getReleaseYear() const { return releaseYear; }
    double getRating() const { return rating; }

    bool operator==(const Movie& other) const { return id == other.id; }
    bool operator<(const Movie& other) const { return rating < other.rating; }

    friend std::ostream& operator<<(std::ostream& os, const Movie& m) {
        os << "[" << m.id << "] " << m.title << " (" << m.releaseYear << ") " << m.rating;
        return os;
    }
};

#endif