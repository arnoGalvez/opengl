CC = g++
LIB = -lglfw3 -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl #-lassimp #-lGL
CFLAGS = -Wall -Wextra -std=c++11 -g
DIR = -I src -I src/drawing -I src/window
DRW_FILES = $(wildcard src/drawing/*.cpp)
WIN_FILES = $(wildcard src/window/*.cpp)
EXE_FILES = $(wildcard src/exercice/*.cpp)
GLD_FILE = src/glad/glad.cpp
OBJ_FILES = $(patsubst %.cpp, %.o, ${DRW_FILES} ${WIN_FILES} ${GLD_FILE})
EXE_OBJ = $(patsubst %.cpp, %.o, ${EXE_FILES})

all: compile

exe:
	echo ${EXE_OBJ} ${OBJ_FILES} 

clean:
	rm -f ${OBJ_FILES} ${EXE_OBJ} exo1 exo2

compile: ${OBJ_FILES} ${EXE_OBJ}

%.o: %.cpp
	${CC} ${CFLAGS} ${DIR} -o $@ $< -c

exo1 : ${OBJ_FILES} ${EXE_OBJ}
	${CC} ${CFLAGS} -o $@ ${OBJ_FILES} src/exercice/$@.o ${LIB} 

exo2 : ${OBJ_FILES} ${EXE_OBJ}
	${CC} ${CFLAGS} -o $@ ${OBJ_FILES} src/exercice/$@.o ${LIB} 

phong: ${OBJ_FILES} src/exercice/phong.o
	${CC} ${CFLAGS} -o $@ ${OBJ_FILES} src/exercice/phong.o  ${LIB}
water.exe: ${OBJ_FILES} src/exercice/water.o
	${CC} ${CFLAGS} -o $@ ${OBJ_FILES} src/exercice/water.o ${LIB}
raytracer.exe: ${OBJ_FILES} src/exercice/raytracer.o
	${CC} ${CFLAGS} -o $@ ${OBJ_FILES} src/exercice/raytracer.o ${LIB}
noise_cubes: ${OBJ_FILES} src/exercice/noise_cubes.o
	${CC} ${CFLAGS} -o $@.exe ${OBJ_FILES} src/exercice/noise_cubes.o ${LIB}
