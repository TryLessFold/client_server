
#include<sercl.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<string.h>
#include<malloc.h>
//#include<unistd.h>
void func(char* str)
{
	str[0] = 'O';
}
void *handler()
{
	key_t key;
	int que_in, que_out;
	sercl_t buff;
	key = ftok("server.c", 'H');
	que_in = msgget(key, 0);
	key = ftok("server.c", 'O');
	que_out = msgget(key, 0);
	while(1)
	{
		msgrcv(que_in, &buff, SERCL, 0, 0);
		func(buff.str);
		msgsnd(que_out, &buff, SERCL, 0);
	}
}
long log_2(unsigned int alf)
{
	long i = 0;
	while(alf>=2)
	{
		alf/=2;
		i++;
	}
	return i;
}
long get_user(int **mass)
{
	unsigned long tmp = 1;
	long i = 1;
	while((tmp&(*mass)[i])!=0)
	{
		tmp<<=1;
		if (tmp == 0x00000000)
		{
			i++;
			tmp = 1;
			if (i>(*mass)[0])
			{
				(*mass)[0]++;
				(*mass) = realloc((*mass), (i+1)*sizeof(int));
			}
		}
	}
	(*mass)[i] = (*mass)[i]|tmp;
	return ((32*(i-1))+log_2(tmp));
}
void clear_user(int *mass, int pos)
{
	unsigned int tmp = 1;
	tmp = tmp << (pos%32);
	tmp = ~tmp;
	mass[(pos/32)+1] &= tmp;
}
int main()
{
	key_t key;
	sercl_t buff;
	struct msqid_ds bb;
	int que_in, que_hand, que_out;
	int num_cl;
	int *users = malloc(sizeof(int)*2);
	int val = 1;
	users[0] = 1;
	key = ftok("server.c", 'I');
	que_in = msgget(key, IPC_CREAT|0666);
	key = ftok("server.c", 'H');
	que_hand = msgget(key, IPC_CREAT|0666);
	key = ftok("server.c", 'O');
	que_out = msgget(key, IPC_CREAT|0666);
	pthread_t tid[5];
	for(int i = 0; i<5; i++)
	{
		pthread_create(&tid[i], NULL, handler, NULL);
	}

	while(val)
	{
		msgrcv(que_in, &buff, SERCL, 0, 0);
		printf("%ld %s", buff.mtype, buff.str);
		if(!strcmp(buff.str,"shutdown\n"))
		{
			msgctl(que_in, IPC_RMID, NULL);
			msgctl(que_out, IPC_RMID, NULL);
			msgctl(que_hand, IPC_RMID, NULL);
			free(users);
			return 0;
		}
		if(buff.mtype == 1)
		{
			buff.mtype = get_user(&users)+2;
			msgsnd(que_out, &buff, SERCL, 0);
		}
		else
		{
			if(buff.str[0] == 0)
			{
				clear_user(users, buff.mtype-2);
			}
			else
			{
				msgsnd(que_hand, &buff, SERCL, 0);
			}
		}
	}
	return 0;
}
