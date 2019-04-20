#include<iostream>
#include <stdlib.h>

using namespace std;

int main( int argc, char ** argv ){
  if(argc == 0){
    cout << "ERROR: No arguments received.\n";
  }
  else if(argc < 4){
    cout << "ERROR: Enter three or more arguments.\n";
  }
  else if(argc > 11){
    cout << "ERROR: Enter ten or fewer arguments.\n";
  }
  else{
    int argInt = 0;
    int sum = 0;
	  cout << "The argument count = " << argc << endl;
	  cout << "The argument tokens are: " << endl;
	  for( int i = 0; i < argc-1; i++ ){
	  	cout << "argv[ " << i << " ] = " << argv[ i+1 ] << endl;
      argInt = atoi( argv[ i+1 ] );
      sum += argInt;
	  }
    cout << "The sum is " << sum << endl;
    double avg = sum;
    cout << "The average is " << (avg/(argc-1)) << endl;
    cout << "The range is " << atoi(argv[argc-1]) - atoi(argv[1]) << endl;
  }
  return 0;
}