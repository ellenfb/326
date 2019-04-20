#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
using namespace std;

/** Variables */
int value = 100;
char input = ' ';
//integral speed setting
bool enabled = true;
bool countingDown = true; //true is down


void *inputReader( void *param );
void *countDownThread( void *param );

int main( int argc, char ** argv ){
	// cout << "Initially, value = " << value << endl;
	pthread_t tid_reader, tid_countDownThread;
	pthread_attr_t attr;
	pthread_attr_init( &attr );
  
	pthread_create( &tid_reader, &attr, inputReader, NULL );	// create the thread
	pthread_create( &tid_countDownThread, &attr, countDownThread, NULL );	// create the thread
  
	cout << "Main thread blocking until worker threads finish\n";
	pthread_join( tid_reader, NULL );
	pthread_join( tid_countDownThread, NULL );
  
	cout << "Main thread unblocked and outta here";
}

void *inputReader( void *param ){
	cout << "!!! Reader Thread Running!\n";
  
	cout << "Type +, -, s, d, or a to exit: ";
  
	while( input != 'a' ){
		cin >> input; //reads user input
  
    switch(input){
      
      case '+' :
        cout << "Attempting to increase speed." << endl;
        break;
      
      case '-' :
        cout << "Attempting to decrease speed." << endl;
        break;
      
      case 's' :
        if(enabled){
          cout << "Pausing." << endl;
          enabled = false;
        }
        else{
          cout << "Resuming." << endl;
          enabled = true;
        }
        break;
      
      case 'd' :
        cout << "Changing counting direction." << endl;
        if(countingDown){
          cout << "Going up!" << endl;
          countingDown = false;
        }
        else{
          cout << "Going down!" << endl;
          countingDown = true;
        }
        break;
        
      case 'a' :
        cout << "Ending input segment..." << endl;
        break;
      
      default :
        break;
    }
  }
  
	cout << "inputReader thread exited\n";
	pthread_exit( 0 );
}

void *countDownThread( void *param ){
  
	cout << "!!! Count Down Thread Running!\n";
  
	struct timespec timing;
	timing.tv_sec = 0;
	timing.tv_nsec = 500000000L;	// sleep time 500million nanoseconds
  
	while( value == 0 );	// BLOCK while value equals 0
  
	while( input != 'a' ){
    
    /** Counting down */
    if( countingDown ){
      /** Counter */
      cout << --value << flush;	// flush to make the value display
		  // sleep( 1 );							// wait 1 second
		  nanosleep( &timing, NULL );
		  // cout << "\r                       \r";	// overwrite previous number with spaces
		  cout << "\b\b\b\b\b\b       \b\b\b\b\b\b";	// overwrite previous number with spaces
		  // cout << "\b\b\b\b\b\b";	// backspace over previously  displayed characters
    }
    /** Starts counting up in here */
    else{
      /** Counter */
      cout << ++value << flush;	// flush to make the value display
		  // sleep( 1 );							// wait 1 second
		  nanosleep( &timing, NULL );
		  // cout << "\r                       \r";	// overwrite previous number with spaces
		  cout << "\b\b\b\b\b\b       \b\b\b\b\b\b";	// overwrite previous number with spaces
		  // cout << "\b\b\b\b\b\b";	// backspace over previously  displayed characters
    }
    
    /** Blocks while enabled is false */
    while( enabled == false ){
      
      if( input == 'a'){
        break;
      }
    }
    
    /** Increasing speed */
    if( input == '+'){
      
      if( timing.tv_nsec == 500000000L/8){
        cout << "Error: Already at maximum speed." << endl;
      }
      else{
        timing.tv_nsec = timing.tv_nsec/2;
      }
    }
    
    /** Decreasing speed */
    else if( input == '-'){
      
      if( timing.tv_nsec == 500000000L){
        cout << "Error: Already at minimum speed." << endl;
      }
      else{
        timing.tv_nsec = timing.tv_nsec*2;
      }
    }
    
    if(input != 'a'){
      input = ' ';
    }
    	
	}
	cout << "countDownThread thread exited\n";
	pthread_exit( 0 );
}
