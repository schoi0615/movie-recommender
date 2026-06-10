#include "movie.h"

// 생성자 구현
Movie::Movie() : id(0), title(""), releaseYear(0), rating(0.0) {}

Movie::Movie(int id, const std::string& title, int releaseYear, double rating)
    : id(id), title(title), releaseYear(releaseYear), rating(rating) {}

// Getter 구현
int Movie::getId() const { return id; }
std::string Movie::getTitle() const { return title; }
int Movie::getReleaseYear() const { return releaseYear; }
double Movie::getRating() const { return rating; }

// 연산자 오버로딩 구현
bool Movie::operator==(const Movie& other) const { 
    return id == other.id; 
}

bool Movie::operator<(const Movie& other) const { 
    return rating < other.rating; 
}

std::ostream& operator<<(std::ostream& os, const Movie& m) {
    os << "[" << m.id << "] " << m.title << " (" << m.releaseYear << ") " << m.rating;
    return os;
}