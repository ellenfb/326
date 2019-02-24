#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
using namespace std;

int main(){
	pid_t pid, pid1, pid2, pid3;
	pid = fork(); //child 1
	if( pid < 0 ){
		cout << "ERROR: Fork failed!\n";
		exit( 1 );	// abort!
	}
	else if( pid == 0 ){	// true inside child

		//child 2
		pid1 = fork();
		if(pid1 < 0){
			cout << "ERROR: Fork 2 Failed!\n";
			exit(1);
		}
		else if (pid1 == 0){
			cout << "CHILD1: about fork and show long list of directory contents:\n";
			system("ls -l");
		}
		else{
//			cout << "\nCHILD1: Waiting for child 2 to exit.\n";
			wait( &pid1) ;
//			cout << "\nCHILD1: Child 2 finally exited\n";
			exit(1);
		}

		//child 3
		pid2 = fork();
		if( pid2 < 0){
			cout << "ERROR: Fork 3 Failed!\n";
			exit(1);
		}
		else if( pid2 == 0){
			cout << "CHILD1: about fork and show hello.cpp contents\n";
			ifstream fin;
			fin.open("hello.cpp", ios::in);
			char my_character ;
			while (!fin.eof() ) {
				fin.get(my_character);
				cout << my_character;
			}
		}
		else{
//			cout << "\nCHILD1: Waiting for child 3 to exit.\n";
			wait( &pid2) ;
//			cout << "\nCHILD1: Child 3 finally exited\n";
			exit(1);
		}

		//child 4
		pid3 = fork();
		if( pid3 < 0){
			cout << "ERROR: Fork 4 Failed!\n";
			exit(1);
		}
		else if( pid3 == 0){
			cout << "CHILD1: about fork and compile hello.cpp\n";
			system("g++ hello.cpp -o hello.out");
		}
		else{
//			cout << "\nCHILD1: Waiting for child 4 to exit.\n";
			wait( &pid3) ;
//			cout << "\nCHILD1: Child 4 finally exited\n";
			exit(1);
		}
		
		cout << "CHILD1: Doing ./hello.out 2\n";
		system("./hello.out 2");
	}
	else{	// positive pid means parent
		cout << "\nPARENT: Waiting for child to exit...\n";
		wait( &pid );	// only wait if pd > 0 i.e. inside parent
		cout << "\nPARENT: Child finally finally exited\n";
	}
	exit( 0 );
}
