#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]){
	int p2c[2];//0 for read,1 for write
	int c2p[2];//0 for read,1 for write
	pipe(p2c);
	pipe(c2p);

	char buf_ping[4];
	char buf_pong[4];

	int pid = fork(); 

	if(pid > 0){//parent process
		close(p2c[0]);
		close(c2p[1]);
		write(p2c[1],"ping",4);
		wait((int*)0);//wait until child process returns
		if (read(c2p[0],buf_pong,sizeof(buf_pong))==4&&strcmp(buf_pong,"pong")==0){
			printf("%d: received pong\n", getpid());
		}
		close(p2c[1]);
		close(c2p[0]);
		exit(0);
	}else if(pid==0){//child process
		close(p2c[1]);
		close(c2p[0]);
		if(read(p2c[0],buf_ping,sizeof(buf_ping))==4&&strcmp(buf_ping,"ping")==0){
			printf("%d: received ping\n", getpid());
		}
		write(c2p[1],"pong",4);
		close(p2c[0]);
		close(c2p[1]);
		exit(0);
	}else{
		printf("fork error\n");
		close(p2c[0]);
		close(p2c[1]);
		close(c2p[0]);
		close(c2p[1]);
		exit(1);
	}
}

