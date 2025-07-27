#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>
/*
int main(){
	int pid;
	pid = fork();

	if(pid > 0){
		//parent
		wait(NULL);
		printf("parent: %d benim pid: %d\n",pid,getpid());
	}
	if(pid == 0){
		//child
		printf("child: %d benim pid: %d\n",pid,getpid());
	}
	if(pid < 0){
		//error
	}

}
*/
/*
int main(){
	char giris[100];
	//First open write folde mode
	FILE *ptrm;
	ptrm = fopen("/home/gokalp/Documents/Eclipse/ttt/a.txt", "w");
	if(ptrm == NULL){
		printf("Folder didn't open\n");
		return 1;
	}

	//get user enter
	printf("Write anythink");
	fgets(giris, sizeof(giris), stdin);

	//write to folder
	fprintf(ptrm, "%s", giris);
	fclose(ptrm);

	//open folder again for read
	ptrm = fopen("/home/gokalp/Documents/Eclipse/ttt/a.txt", "r");
	if(ptrm == NULL){
		printf("Folder didn't open\n");
		return 1;
	}

	char write;
	while((write = fgetc(ptrm)) != EOF){
		putchar(write);
	}

	fclose(ptrm);
}

*/
void process_leaf(void){
	srand(getpid());
	int r = rand() % 100;
	char filename[20];
	sprintf(filename, "%d.txt", getpid());
	printf("r random num crate %d from %d", r, getpid());
	fflush(stdout);

	FILE *ptr = fopen(filename, "w");
	fprintf(ptr, "%d", r);
	fclose(ptr);
}

int read_file(int pid){
	srand(getpid());
	int r = rand() % 100;
	char filename[20];
	sprintf(filename, "%d.txt", getpid());
	FILE *ptr = fopen(filename, "r");
	fscanf(ptr, "%d", &r);
	fclose(ptr);
	return r;
}

int parent_process(int pidleft, int pidright){
	int lvalue = read_file(pidleft);
	int rvalue = read_file(pidright);
	char filename[20];
	sprintf(filename, "%d.txt", getpid());
	FILE *ptr = fopen(filename, "w");
	fprintf(ptr, "%d", (lvalue + rvalue));
	printf("parent sum val: %d from %d\n", lvalue + rvalue, getpid());
	fclose(ptr);
	return rvalue+lvalue;
}

void tree(int height){
	if(height < 0){
		process_leaf();
		return;
	}

	int pid = fork();

	if(pid > 0){
		int pid2 = fork();
		if(pid2 > 0){
			printf("parent: %d, lchild: %d, rchild: %d, height:%d\n", getpid(), pid, pid2, height);
			wait(NULL);
			int result = parent_process(pid, pid2);
			if(height == 3){
				printf("RESULT: %d\n", result);
			}
		}
		if(pid2 == 0){
			tree(--height);
		}
	}
	if(pid == 0){
		tree(--height);
	}
}

int main(){
	tree(3);
}
