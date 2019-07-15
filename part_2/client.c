#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <sercl.h>
#include <string.h>
int main()
{
	sercl_t buff;
	buff.mtype = 1;
	strcpy(buff.str,"a");
	key_t key;
	int que_in, que_out;
	key = ftok("server.c", 'O');
	que_in = msgget(key, 0);
	key = ftok("server.c", 'I');
	que_out = msgget(key, 0);
	if((que_in!=-1)&&(que_out!=-1))
	{
		msgsnd(que_out, &buff, SERCL, 0);
		msgrcv(que_in, &buff, SERCL, 0, 0);
		printf("Input: ");
		fgets(buff.str, 256, stdin);
		while(strcmp(buff.str, "/exit\n"))
		{
			msgsnd(que_out, &buff, SERCL, 0);
			msgrcv(que_in, &buff, SERCL, buff.mtype, 0);
			printf("%s", buff.str);
			printf("Input: ");
			fgets(buff.str, 256, stdin);
		}
		buff.str[0] = 0;
		msgsnd(que_out, &buff, SERCL, 0);
	}
	return 0;
}
