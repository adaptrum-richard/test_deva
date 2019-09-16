EXEC=test
all: $(EXEC)
$(EXEC): clean
	gcc -o test test.c -llua -lm -ldl
clean:
	rm -rf test
