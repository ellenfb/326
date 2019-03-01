#include <sys/types.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
using namespace std;

int main(){
	
	int op1 = -1;
	int op2 = -1;
	int op1_rd, op2_rd;
	string wr_opr = "";
	string opr_rd;
	char opr_wr[1];
	char rd_opr[1];

	int fd[2];
	pid_t pid;
	pipe( fd );
	pid = fork();
	if( pid > 0 ){	// in parent
		close( fd[0] );	// close unused Read End

		cout << "Enter an integer, a plus or minus, and an integer. Integers must be positive and < 100." << endl;
		while (op1 < 0 or op1 > 99){
			cout << "Enter operand 1." << endl;
			cin >> op1;
		}
		cout << "Operand 1: " << op1 << endl;

		while (wr_opr.compare("+") != 0 and wr_opr.compare("-") != 0){
			cout << "Enter operation." << endl;
			cin >> wr_opr;
		}
		cout << "Operation: " << wr_opr << endl;

		while (op2 < 0 or op2 > 99){
			cout << "Enter operand 2." << endl;
			cin >> op2;
		}
		cout << "Operand 2: " << op2 << endl;
		write(fd[1], &op1, sizeof(op1));

		unsigned int size = wr_opr.length();
		wr_opr.copy(opr_wr, wr_opr.length(), 0);
		write(fd[1], opr_wr, size);

		write(fd[1], &op2, sizeof(op2));

		close( fd[1] );	// all done, close the pipe!
	}
	else{
		close( fd [1] );	// close unused write end
		read(fd[0], &op1, sizeof(op1));
		cout << "In child: op1: " << op1 << endl;
		op1_rd = op1;

		read(fd[0], rd_opr, 1);
		cout << "In child: opr: " << rd_opr << endl;
		opr_rd = rd_opr;

		read(fd[0], &op2, sizeof(op2));
		cout << "In child: op2: " << op2 << endl;
		op2_rd = op2;

		if (opr_rd.compare("+") == 0){
			cout << op1_rd << " + " << op2_rd << " = " << op1_rd + op2_rd << endl;
		}
		else {
			cout << op1_rd << " - " << op2_rd << " = " << op1_rd - op2_rd << endl;
		}

		close( fd[0] );	// all done, close the pipe!
	}
	exit(0);
}

