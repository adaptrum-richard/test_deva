EXEC=test
all: $(EXEC)
$(EXEC): clean
	gcc -g -o test test.c -llua -lm -ldl -lpthread
clean:
	rm -rf test
