#include "apue.h"

#define BUFFSIZE 4096
/*
 *从标准输入读，并向标准输出写
usage: ./a.out < infile > outfile
 * */
int main(void) {
	int n;
	char buf[BUFFSIZE];

	while((n = read(STDIN_FILENO, buf, BUFFSIEZE)) > 0) {
		if(write(STDOUT_FILENO, buf, n) != n) {
			err_sys("write error");
		}
	}
	if(n < 0) {
		err_sys("read error");
	}

	exit(0);
}
