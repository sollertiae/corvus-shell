CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17
SRCS = main.cpp parser.cpp builtins.cpp executor.cpp

corvus: $(SRCS)
	$(CXX) $(CXXFLAGS) -o corvus $(SRCS)

clean:
	rm -f corvus