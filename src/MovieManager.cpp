#include "MovieManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

void MovieManager::addMovie(const Movie& movie) {
    movies.push_back(movie);
}

void MovieManager::printAll() const {
    for (const auto& m : movies) {
        std::cout << m << std::endl;
    }
}

void MovieManager::sortByRating() {
    std::sort(movies.begin(), movies.end(), [](const Movie& a, const Movie& b) {
        return b < a;
    });
}

void MovieManager::searchByTitle(const std::string& title) const {
    bool found = false;
    for (const auto& m : movies) {
        if (m.getTitle().find(title) != std::string::npos) {
            std::cout << m << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "검색 결과가 없습니다." << std::endl;
    }
}

Movie MovieManager::findById(int id) const {
    for (const auto& m : movies) {
        if (m.getId() == id) return m;
    }
    return Movie(-1, "Unknown", 0, 0.0);
}

void MovieManager::loadFromFile(const std::string& filename) {
    movies.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 파일을 열 수 없습니다." << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line); // 헤더 라인 건너뛰기
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        
        std::getline(ss, token, ',');
        int id = std::stoi(token);
        
        std::getline(ss, token, ',');
        std::string title = token;
        
        std::getline(ss, token, ',');
        int year = std::stoi(token);
        
        std::getline(ss, token, ',');
        double rating = std::stod(token);
        
        movies.push_back(Movie(id, title, year, rating));
    }
    file.close();
    std::cout << filename << " 로드 완료: " << movies.size() << "건" << std::endl;
}

void MovieManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 저장 실패" << std::endl;
        return;
    }
    file << "id,title,year,rating\n";
    for (const auto& m : movies) {
        file << m.getId() << "," << m.getTitle() << "," << m.getReleaseYear() << "," << m.getRating() << "\n";
    }
    file.close();
    std::cout << filename << " 저장 완료: " << movies.size() << "건" << std::endl;
}

int MovieManager::size() const {
    return movies.size();
}