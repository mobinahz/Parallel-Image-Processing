CXX = g++
CXXFLAGS = -std=c++11

OUT_MAIN = ImageFilters.out

all: $(OUT_MAIN)

$(OUT_MAIN): main.o filters.o bmp.o
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cpp bmp.hpp filters.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

bmp.o: bmp.cpp bmp.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

filters.o: filters.cpp filters.hpp bmp.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY: all clean

clean:
	rm -f *.o ./*.out