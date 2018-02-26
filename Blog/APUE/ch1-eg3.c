#include "apue.h"

/*
 *功能类似于前一个例子，将标准输入复制到标准输出
 * */

int main(void) {
	int c;
	while((c = getc(stdin)) != EOF) {
		//从标准输出中一个个读取出来
		if(putc(c, stdout) == EOF) {
			//将一个个字符复制到标准输出中
			err_sys("output error");
		}
	}

	if(ferror(stdin)) {
		err_sys("input_error");
	}

	exit(0);
}
