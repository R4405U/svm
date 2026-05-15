SRCS = $(shell find src -name "*.c" -type f)
FLAGS = -Wall -Wextra -Wpedantic -std=gnu99
sm: $(OBJS)
	gcc -o svm main.c $(SRCS) -Iinclude $(FLAGS)

%.o: %.c
	gcc -c $< -Iinclude -o $@

clean:
	rm -rf src/*.o *.o svm
