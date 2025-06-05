#!/bin/bash

SRC_FILES=$(ls *.cpp 2>/dev/null | tr '\n' ' ')
HEADER_FILES=$(ls *.hpp 2>/dev/null | tr '\n' ' ')

cat << EOF > Makefile
CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = ${SRC_FILES}
HEADER = ${HEADER_FILES}

OBJ = \$(SRC:.cpp=.o)

NAME = a.out

all: \$(NAME)
	@echo "Compiled :)"

\$(NAME): \$(OBJ)
	\$(CC) \$(CFLAGS) \$(OBJ) -o \$(NAME)

%.o: %.cpp \$(HEADER) 
	\$(CC) \$(CFLAGS) -c \$< -o \$@

clean:
	rm -f \$(OBJ)

fclean: clean
	rm -f \$(NAME)

re: fclean all
EOF
