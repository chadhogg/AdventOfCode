CXX = g++
CXXFLAGS = --std=c++20 -g -Wall -Werror
LDFLAGS =
LDLIBS =


.PHONY : all clean

CXX_OPTIONS = -g -Wall
SOURCES = $(wildcard 2015Day??.cpp)
PROGRAMS = $(subst .cpp,.out,$(SOURCES))

all : $(PROGRAMS)

%.out : %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean :
	@$(RM) *.out $~
