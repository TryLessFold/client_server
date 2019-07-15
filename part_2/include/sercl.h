#ifndef SERCL
#define SERCL sizeof(sercl_t) - sizeof(long)
	typedef struct sercl
	{
		long mtype;
		char str[256];
	} sercl_t;
#endif
