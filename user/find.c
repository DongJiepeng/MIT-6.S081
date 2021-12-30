#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path, char* filename);

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("usage: fing <PATH> <FILENAME>\n");
	}else{
		find(argv[1],argv[2]);
	}
	exit(0);
}

void find(char* path, char* filename){
	char buf[512];
	char* p;
	int fd;
	struct dirent de;
	struct stat st;

	if((fd = open(path,0)) <0){
		printf("find : cannnot open %s\n", path);
		return ;
	}

	if(fstat(fd, &st) < 0){
		printf("find : cannot stat %s\n", path);
		close(fd);
		return ;
	}

	if(st.type != T_DIR){
		printf("find: %s is not a directory !\n",path);
		close(fd);
		return ;
	}

	if(strlen(path) + 1 + DIRSIZ +1 > sizeof(buf)) {
		printf("find: path too long\n");
	}
	
	strcpy(buf, path);
	p = buf+strlen(buf);
	*p++ = '/';
	while(read(fd, &de, sizeof(de)) == sizeof(de)) {
		if(de.inum == 0)	continue;
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;
		if(stat(buf,&st) < 0){
			printf("find : cannot stat %s\n",buf);
			continue;
		}

		if(strcmp(p,filename) == 0){
			printf("%s\n",buf);
		}else if(st.type == T_DIR && strcmp(p,".") !=0 && strcmp(p,"..") !=0){
			find(buf,filename);//recursion
		}
	}
	close(fd);
}


