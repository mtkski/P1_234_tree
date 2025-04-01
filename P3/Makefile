all : P3

P3: P3_code.c
	gcc -o P3 P3_code.c
	gcc -D NDEBUG -O3 -ansi -pedantic P3_code.c  

T01 : all
	./P3 < ./P3-tests/T01/input

T02 : all
	./P3 < ./P3-tests/T02/input

T03 : all
	./P3 < ./P3-tests/T03/input

T04 : all
	./P3 < ./P3-tests/T04/input

clean :
	rm P3
	rm *.out