# @Group No.:    27
# @Authors:
#    Aditya Agrawal    2022A7PS0160P
#    Samarth           2022A7PS0096P
#    Shashank Jha      2022A7PS1186P
#    Shikhar Singh     2022A7PS1170P

# make CC=gcc-11.4.0

# test: test_exe
# 	./test_exe
# test_exe: test.o lexer.o
# 	gcc -o test_exe test.o lexer.o
# test.o: test.c
# 	gcc -c test.c
# lexer.o: lexer.c
# 	gcc -c lexer.c
# clean:
# 	rm *.o
# 	rm *_exe

driver: driver_exe
	./driver_exe test_t1.txt
driver_exe: driver.c lexer.o parser.o associativeArray.o set.o
	gcc -o driver_exe driver.c lexer.o parser.o associativeArray.o set.o

set.o: set.c
	gcc -c set.c

lexer.o: lexer.c
	gcc -c lexer.c

parser.o: parser.c
	gcc -c parser.c

associativeArray.o: associativeArray.c
	gcc -c associativeArray.c
	
clean:
	rm *.o
	rm *_exe
