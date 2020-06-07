#ifndef LOAD_H
#define LOAD_H

#include <CL/cl.h>

extern cl_int status;

#define check(str) {						\
	if (status != CL_SUCCESS) {				\
		printf("ERROR: ret=(%d): %s\n", status, str);	\
		goto exit;					\
	}							\
	printf("OK: %s\n", str);				\
}

#endif //LOAD_H

