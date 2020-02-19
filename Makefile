SHELL = /bin/bash
FLAGS = -O2 -Wall

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
	cc ${FLAGS} -o $@ $^ ${LIBS}

${OBJECTS}: ${SOURCES}
	cc ${FLAGS} -c $^ ${INCLUDE}

clean:
	rm -f *.o ${TARGET}
