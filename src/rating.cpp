#include "rating.h"
#include <iostream>

Rating::Rating(int uId, int mId, double s) : userId(uId), movieId(mId) {
    if (s >= 0.0 && s <= 5.0) {
        score = s;
    } else {
        score = 0.0;
    }
}

int Rating::getUserId() const { return userId; }
int Rating::getMovieId() const { return movieId; }
double Rating::getScore() const { return score; }

void Rating::display() const {
    std::cout << *this << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Rating& r) {
    os << "유저ID: " << r.userId
       << " | 영화ID: " << r.movieId
       << " | 점수: " << r.score;
    return os;
}