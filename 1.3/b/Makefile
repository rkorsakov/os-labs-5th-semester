TARGET = thread-example
SRCS = thread.c

CC=gcc
RM=rm
CFLAGS= -g -Wall -Wpedantic -Wpointer-arith -Wendif-labels -Wmissing-format-attribute \
        -Wimplicit-fallthrough=3 -Wcast-function-type -Wshadow=compatible-local -Wformat-security -fsanitize=address

LIBS=-lpthread
INCLUDE_DIR="."

all: ${TARGET}

${TARGET}: ${SRCS}
	${CC} ${CFLAGS} -I${INCLUDE_DIR} ${SRCS} ${LIBS} -o ${TARGET}

clean:
	${RM} -f *.o ${TARGET}
