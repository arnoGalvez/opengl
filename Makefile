CC = g++
LIB = -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
CFLAGS = -Wall -Wextra
SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c,%.o,$(SRC_FILES))

all: compile

clean:
	rm -f $(OBJ_FILES)

compile: $(OBJ_FILES)
	${CC} ${CFLAGS} -o exe ${OBJ_FILES} ${LIB}

%:%.o glad.o
	${CC} ${CFLAGS} -o $@ glad.o $< ${LIB}

%.o:%.c 
	${CC} ${CFLAGS} -o $@ $< -c
