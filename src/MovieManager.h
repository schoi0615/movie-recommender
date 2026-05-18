#ifndef MOVIE_MANAGER_H
#define MOVIE_MANAGER_H

#include "BaseManager.h"
#include "movie.h"
#include <vector>
#include <string>

class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;

public:
    void addMovie(const Movie& movie);
    void printAll() const;
    void sortByRating();
    void searchByTitle(const std::string& title) const;
    Movie findById(int id) const;

    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
    int size() const override;
};

#endif