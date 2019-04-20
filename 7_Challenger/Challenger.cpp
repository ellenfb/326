#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main(){

	const int SIZE = 128; //DONT KNOW SIZE
	const char *name = "Challenge";
	int shm_fd;
	void *ptr;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	ptr = mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
	if(ptr == MAP_FAILED){
		cout << "ERROR: Map failed.\n";
		exit(-1);
	}

	short newValue = 0;
	short value = 0;

	do{
		// Reading from memory
		bool oldData = true;
		while(oldData){

			newValue = *((short *)ptr);
			if(newValue != value){
				oldData = false;
			}

		}
		value = newValue;
		cout << "Value recieved: " << value << endl;

		if(value == 2){
			cout << "VICTORY!" << endl;
			*((short *)ptr) = 1;
			break;
			/*cout << "Attempting to close memory." << endl;
			if(shm_unlink(name) == -1){
				cout << "Error removing memory region.\n";
				exit(-1);
			}*/
		}
		else if(value == 1){
			cout << "Defeat." << endl;
			break;
			/*cout << "Attempting to close memory." << endl;
			if(shm_unlink(name) == -1){
				cout << "Error removing memory region.\n";
				exit(-1);
			}*/
		}
		else{

			// Calculating N+1
			if(value % 2 == 1){
				value = value * 3 + 1;
			}
			else{
				value = value / 2;
			}

			cout << "Writing into shared memory: " << value << endl;
			*((short *)ptr) = value;
		}
		cout << "Awaiting new data." << endl;

	}while(newValue > 0);

	cout << "Attempting to close memory." << endl;
	if(shm_unlink(name) == -1){
		cout << "Error removing memory region.\n";
		exit(-1);
	}
	cout << "Memory region closed." << endl;

	return 0;
}
