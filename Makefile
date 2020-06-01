PROJECT=s8nchip
TYPE=c

CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -Wall -Wextra -Wshadow -pedantic -O3 -Wunreachable-code -Wmain #flags for C and C++
LDFLAGS=-g -s
LDLIBS= -lSDL2 # insert here all libraries needed


SRCS=src/main.c src/timer.c src/state.c\
     src/display.c src/s8nchip.c src/game.c
OBJS=$(subst .$(TYPE),.o,$(SRCS))

all : $(PROJECT)

$(PROJECT): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(PROJECT) $(OBJS) $(LDLIBS)

$(PROJECT).o: $(PROJECT).$(TYPE)
	
clean:
	$(RM) $(OBJS)
		
disclean: clean
	$(RM) $(PROJECT)
