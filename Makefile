CC = g++
LIB = -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
CFLAGS = -Wall -Wextra
DIR = -I src -I src/drawing -I src/window
DRW_FILES = $(wildcard src/drawing/*.cpp)
WIN_FILES = $(wildcard src/window/*.cpp)
EXE_FILES = $(wildcard src/exercice/*.cpp)
GLD_FILE = src/glad/glad.c
OBJ_FILES = $(patsubst %.c[p]*,%.o, ${DRW_FILES} ${WIN_FILES} ${EXE_FILES} ${GLD_FILE})

all: compile

clean:
	rm -f $(OBJ_FILES)

compile: $(OBJ_FILES)

%.o: %.c[p]*
	${CC} ${CFLAGS} ${DIR} -o $@ $< -c

exo[1-9]*: ${OBJ_FILES}
	${CC} ${CFLAGS} -o $@ ${OBJ_FILES} -L/usr/include/GLFW ${LIB} 
