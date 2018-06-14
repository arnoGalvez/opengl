CC = g++
LIB = -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
CFLAGS = -Wall -Wextra
DIR = -I src -I src/drawing -I src/window
DRW_FILES = $(wildcard src/drawing/*.c)
WIN_FILES = $(wildcard src/window/*.c)
EXE_FILES = $(wildcard src/exercice/*.c)
GLD_FILE = src/glad/glad.c
OBJ_FILES = $(patsubst %.c, %.o, ${DRW_FILES} ${WIN_FILES} ${GLD_FILE})
EXE_OBJ = $(patsubst %.c, %.o, ${EXE_FILES})

all: compile

exe:
	echo ${EXE_OBJ} ${OBJ_FILES} 

clean:
	rm -f ${OBJ_FILES} ${EXE_OBJ} exo1 exo2

compile: ${OBJ_FILES} ${EXE_OBJ}

%.o: %.c
	${CC} ${CFLAGS} ${DIR} -o $@ $< -c

exo1 : ${OBJ_FILES} ${EXE_OBJ}
	${CC} ${CFLAGS} -o $@ ${OBJ_FILES} src/exercice/$@.o -L/usr/include/GLFW ${LIB} 

exo2 : ${OBJ_FILES} ${EXE_OBJ}
	${CC} ${CFLAGS} -o $@ ${OBJ_FILES} src/exercice/$@.o -L/usr/include/GLFW ${LIB} 