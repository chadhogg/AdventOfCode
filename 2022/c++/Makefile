CXX = g++
CXXFLAGS = -g -Wall -Werror
LDFLAGS =
LDLIBS =


.PHONY : all clean

CXX_OPTIONS = -g -Wall
SOURCES = $(wildcard 2022Day??.cpp)
PROGRAMS = $(subst .cpp,.out,$(SOURCES))

all : $(PROGRAMS)

%.out : %.cpp utilities.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean :
	@$(RM) *.out $~
