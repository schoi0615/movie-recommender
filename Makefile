CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I src

SRCS = src/main.cpp \
       src/movie.cpp \
       src/user.cpp \
       src/rating.cpp \
       src/MovieManager.cpp \
       src/UserManager.cpp \
       src/RatingManager.cpp \
       src/Recommender.cpp \
       src/SimilarityCalculator.cpp \
       src/StressRecommender.cpp

TARGET = movie_recommender

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: clean
