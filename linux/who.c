#define __LIBRARY__
#include <unistd.h>
#include <stdarg.h>


_syscall0(void, whoami);
/* like modern linux "DEFINE_SYSTEMCALL_**** "*/


int main(int argc, char * argv[] ) {
	

	whoami(); /* boo! */


	return 0;
}

