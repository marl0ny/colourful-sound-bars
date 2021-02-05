SHELL = /bin/bash

# COMPILE = /usr/bin/clang
# FLAGS = -O2 -Wall -g
COMPILE = /usr/bin/gcc
FLAGS = -O2 -Wall -g

# Put your own include and lib path for sdl2 here:
INCLUDE = -I${HOME}/include
LIBS = -L${HOME}/lib -lSDL2 -lm

TARGETDIR = .
# TARGETDIR = ${HOME}
TARGET = ${TARGETDIR}/sound_view
SOURCES = sound_view.c wave.c fft.c
OBJECTS = sound_view.o wave.o fft.o

.PHONY = all clean

all: ${TARGET}

${TARGET}: ${OBJECTS}
	${COMPILE} ${FLAGS} -o $@ $^ ${LIBS}

${OBJECTS}: ${SOURCES}
	${COMPILE} ${FLAGS} -c $^ ${INCLUDE}

clean:
	rm -f *.o ${TARGET}
