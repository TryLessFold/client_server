#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sercl.h>
#include <string.h>
int main()
{
	sercl_t buff;
	buff.mtype = 1;
	strcpy(buff.str, "Send");
	int ds;
	key_t key;
	key = ftok("server.c", 't');
	ds = msgget(key, IPC_CREAT|0666);
	msgsnd(ds, &buff, sizeof(sercl_t)-sizeof(long), 0);
	msgrcv(ds, &buff, sizeof(sercl_t)-sizeof(long),2, 0);
	printf("%s", buff.str);
	msgctl(ds, IPC_RMID, 0);
	return 0;
}
