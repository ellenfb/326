#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include <sys/mman.h>

#include <iostream>
using namespace std;

int main(){

	const int SIZE = 128;
	const char *name = "Challenge";
	char messageBuffer[ 5 ];
	int shm_fd;
	void *ptr;
	bool anotherRound = true;
	shm_fd = shm_open(name, O_RDWR, 0666);
	if(shm_fd == -1){
		cout << "CLEANER: ERROR: Opening shared memory failed\n";
		exit(-1);
	}
	
	if(shm_unlink(name) == -1){
		cout << "CLEANER: ERROR: Error removing shared memory region" << name << endl;
		exit(-1);
	}
	cout << "CLEANER: Successfully closed shared memory region" << endl;
	return 0;
}

