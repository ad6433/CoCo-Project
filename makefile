# ONLY FOR LEXICAL ANALYZER TESTING PURPOSES. THIS IS NOT THE FINAL MAKEFILE.

test: test_exe
	./test_exe
test_exe: test.o lexer.o
	gcc -o test_exe test.o lexer.o
test.o: test.c
	gcc -c test.c
lexer.o: lexer.c
	gcc -c lexer.c
clean:
	rm *.o
	rm *_exe