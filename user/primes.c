#include "kernel/types.h"
#include "user/user.h"

void prime(int* p);

int main(int argc, char* argv[]){
	int p[2];//0 for read , 1 for write
	pipe(p);
	
	for(int i = 2;i <= 35;i++){
		write(p[1],&i,sizeof(i));
	}

	int pid = fork();
	if(pid < 0){
		printf("fork error !\n");
		close(p[0]);
		close(p[1]);
		exit(1);
	}else if(pid > 0){
		close(p[0]);
		close(p[1]);
		wait((int*)0);
		exit(0);
	}else {//childern process
		close(p[1]);//no need to write
		prime(p);
		close(p[0]);
		exit(0);
	}	
}

void prime(int *p){
	int num_prime;
	//read first number
	int len = read(p[0],&num_prime,sizeof(num_prime));
	if(len==0){//no data
		close(p[0]);
		exit(0);
	}else{//print the first number
		printf("prime %d\n",num_prime);
	}
	int num;
	int p_next[2];
	pipe(p_next);
	int pid = fork();
	if(pid < 0){
		printf("fork error !\n");
		close(p[0]);
		close(p_next[0]);
		close(p_next[1]);
		exit(1);
	}else if( pid > 0){//parent process
		close(p_next[0]);//no need to read this pipe
		while(read(p[0],&num,sizeof(num))){
			if(num % num_prime != 0){
				write(p_next[1],&num,sizeof(num));	
			}
		}
		close(p[0]);
		close(p_next[1]);
		wait((int*)0);
		exit(0);
	}else {
		close(p[0]);
		close(p_next[1]);
		prime(p_next);
		close(p_next[0]);
		exit(0);
	}
}
