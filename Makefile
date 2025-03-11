all :
	gcc -g -o P1 P1_code.c
	lldb ./P1

	process launch --stdout /dev/ttys032 --stderr /dev/ttys032
