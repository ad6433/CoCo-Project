# ONLY FOR LEXICAL ANALYZER TESTING PURPOSES. THIS IS NOT THE FINAL MAKEFILE.

test: test_exe
	./test_exe
test_exe: test.o lexer2.o
	gcc -o test_exe test.o lexer2.o
test.o: test.c
	gcc -c test.c
lexer2.o: lexer2.c
	gcc -c lexer2.c
clean:
	rm *.o
	rm *_exe