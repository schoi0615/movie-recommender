#ifndef RATING_H
#define RATING_H

#include <iostream>

class Rating {
private:
    int userId;
    int movieId;
    double score;

public:
    Rating() : userId(0), movieId(0), score(0) {}
    Rating(int userId, int movieId, double score)
        : userId(userId), movieId(movieId), score(score) {}

    int getUserId() const { return userId; }
    int getMovieId() const { return movieId; }
    double getScore() const { return score; }

    friend std::ostream& operator<<(std::ostream& os, const Rating& r) {
        os << "유저: " << r.userId << ", 영화: " << r.movieId << ", 평점: " << r.score;
        return os;
    }
};

#endif