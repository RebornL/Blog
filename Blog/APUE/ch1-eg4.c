#include "apue.h"
#include <errno.h>

/*
 *该程序用来展示strerror和perror的用法
 * */

int main(void) {
	fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
	errno = ENOENT;
	perror(argv[0]);
	exit(0);
}
