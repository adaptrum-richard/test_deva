#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <pthread.h>
#include <sys/select.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <string.h>
#include<errno.h>
#include <stdlib.h>
static pid_t gettid() 
{
	return syscall(SYS_gettid);
}

static void my_sleep(long sec,long usec)
{
	struct timeval tv;
	tv.tv_sec=sec;
	tv.tv_usec = usec;
	select(0, NULL, NULL, NULL, &tv);
}

static int load_lua(const char *lua_code)
{
	int result = -1;
	if(!lua_code)
		return -1;
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	if(luaL_loadfile(L, lua_code) || lua_pcall(L, 0,0,0)){
   		printf("error %s\n", lua_tostring(L,-1));
    		return -1;
  	}
	lua_getglobal(L,"result");
	result = lua_tointeger(L,-1);
	lua_close(L);
	return result;
}

void *pthread_t1(void *id)
{
	char *t1 = "test1.lua";
	printf("\n%s running, pid = %d\n", t1, gettid());
	int result = 0;
	while(1)
	{
		my_sleep(0, 200);
		result = load_lua(t1);
		if(!result)
			printf("%s result = %d error\n", t1, result);
	}
	return 0;
}

void *pthread_t2(void *id)
{
	char *t2 = "test2.lua";
	int result = 0;
	printf("\n%s running, pid = %d\n", t2, gettid());
	while(1)
	{
		my_sleep(0, 300);
		result = load_lua(t2);
		if(!result)
			printf("%s result = %d error\n", t2, result);
	}
	return 0;
}


void *pthread_t3(void *id)
{
#define FILE_NAME "/tmp/temp"
	printf("\n t3 running, pid = %d", gettid());
	int count = 0;
	FILE*  fd = NULL;
	char b[1024] = {0};
	int w_len = 0;
	while(1)
	{	
		fd = fopen(FILE_NAME, "w+");
		if(!fd)
		{
			printf("error is %s\n",strerror(errno));
			my_sleep(1,0);
			continue;
		}
		snprintf(b,sizeof(b) ,"%daaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbccccccccccccccccccccccc\n", count++);
		w_len = fwrite(b, 1, strlen(b), fd);
		//printf("w_len = %d, b len = %d\n", w_len, strlen(b));
		my_sleep(0,1000*200);
		//fflush(fd);
		fclose(fd);
		//fflush(NULL);
	}
}

void *pthread_t4(void *id)
{
	pid_t fpid;
	while(1){
		my_sleep(1,0);
		fpid =fork();
		if(fpid < 0)
		{
			printf("fork error\n");
			break;
		}
		else if(fpid == 0)
		{
			my_sleep(0,11110);
			printf("child\n");
			abort();
		}
		else
		{
			printf("parent\n");
			waitpid(fpid,NULL,0);
		}

	}
}

#define NUMBER_OF_THREADS 4
void* (*threads[NUMBER_OF_THREADS]) (void*) = { pthread_t1, pthread_t2, pthread_t3, pthread_t4};
pthread_t tid[NUMBER_OF_THREADS];
int main() 
{
	int i = 0;
	printf("main pid = %d\n", getpid());
	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		pthread_create(&tid[i], NULL, threads[i], NULL);
	}

	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		pthread_join(tid[i], NULL);
	}
	return 0;
}
