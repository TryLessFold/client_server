#include <sercl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
int main()
{
	sercl_t buff;
	key_t key;
	int ds;
	key = ftok("server.c", 't');
	while((ds = msgget(key, 0)) ==-1);
	msgrcv(ds,&buff,sizeof(sercl_t)-sizeof(long),1,0);
	printf("%s nudes", buff.str);
	buff.mtype = 2;
	strcpy(buff.str, "nudes\n");
	msgsnd(ds,&buff,sizeof(sercl_t)-sizeof(long),0);
	return 0;
}
