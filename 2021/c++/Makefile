CXX = g++
CXXFLAGS = -g -Wall -Werror
LDFLAGS =
LDLIBS =


.PHONY : all clean

CXX_OPTIONS = -g -Wall
SOURCES = $(wildcard 2021Day??.cpp)
PROGRAMS = $(subst .cpp,.out,$(SOURCES))

all : $(PROGRAMS)

2021Day19.out : 2021Day19.cpp utilities.hpp Vector3.hpp Vector3.cpp Matrix3.hpp Matrix3.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) 2021Day19.cpp Vector3.cpp Matrix3.cpp $(LDLIBS) -o $@

%.out : %.cpp utilities.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean :
	@$(RM) *.out $~
