#include <stdio.h>
#include <unistd.h>

void main(){
	static char msg[] = {'0','1','2','3','4','5','6','7','8','9'};
	//char* ptr = msg;
	int display(msg), i;

	for(i = 0; i < 10; i++)
		display(msg[i]);
	sleep(2);
	return 0;
}

int display(char m){
	char err_msg[25];
	switch(fork()){
		case 0:
			execlp("/bin/echo","echo",m,(char)NULL);
			sprintf(err_msg "%s Exec failure", m);
			perror(err_msg);
			return(1);
		case -1:
			perror("Fork Failure");
			return(2);
		default:
			return(0);
	}
}
