CXX = g++
CXXFLAGS = -std=c++17 -Wall -Isrc

TARGET = movie_recommender
SRCS = src/main.cpp src/MovieManager.cpp src/UserManager.cpp src/RatingManager.cpp src/Recommender.cpp src/SimilarityCalculator.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f src/*.o $(TARGET)