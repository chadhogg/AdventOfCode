CXX = g++
CXXFLAGS = --std=c++23 -g -Wall
LDFLAGS =
LDLIBS = -lcrypto


.PHONY : all clean

CXX_OPTIONS = -g -Wall
SOURCES = $(wildcard 2015Day??.cpp)
PROGRAMS = $(subst .cpp,.out,$(SOURCES))

all : $(PROGRAMS)

%.out : %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean :
	@$(RM) *.out $~
