CXX = g++
CXXFLAGS = --std=c++23 -g -Wall -O3
LDFLAGS =
LDLIBS = -lcrypto


.PHONY : all clean

CXX_OPTIONS = -g -Wall
SOURCES = $(wildcard 2016Day??.cpp)
PROGRAMS = $(subst .cpp,.out,$(SOURCES))

all : $(PROGRAMS)

%.out : %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean :
	@$(RM) *.out $~
