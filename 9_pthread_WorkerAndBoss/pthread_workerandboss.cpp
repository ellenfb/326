#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
using namespace std;

pthread_t
  *tids;            //array of worker thrd IDs
pthread_cond_t
  *tEnable,         //enable a worker of thrd to print
  tPrinted;         //worker thrd printing complete
pthread_mutex_t
  theMutex;         //mutex variable

static int cleanup_pop_arg = 0;
static void cleanupHandler(void *arg){
  cout << "Worker " << cleanup_pop_arg 
    << " cleaning up and exiting\n"
    "Worker ";
  if(pthread_mutex_unlock(&theMutex) != 0){
    cout << "UHOH!\n";
  }
}

short signify = 0; //thread launch delay variable

class WorkerInfo{
  public:
    WorkerInfo(){}
  
    WorkerInfo(WorkerInfo &wi)
      :workerID(wi.workerID),
       sizeOfDataSet(wi.sizeOfDataSet){
       dataSet = new short[sizeOfDataSet];
       for(int i = 0; i < sizeOfDataSet; i++){
        dataSet[i] = wi.dataSet[i];
        }
       }
  
    WorkerInfo(short wid, short sods)
      :workerID(wid), sizeOfDataSet(sods){}
  
    void putDataInSet(short sA[], short size){
      if(size != sizeOfDataSet){
        cout << "\tsizes not equal\n";
        sizeOfDataSet = size;
        if(dataSet != nullptr){
          delete [] dataSet;
        }
        dataSet = new short[size];
      }
      if(dataSet == nullptr){
        cout << "Allocating memory for data set!\n";
        dataSet = new short[size];
      }
      cout << "\tinitializing dataset\n";
      for(int i = 0; i < sizeOfDataSet; i++){
        dataSet[i] = sA[i];
      }
    }
  
    void showDataSet(){
      cout << "worker " << workerID << " showing data:\n";
      for(int i = 0; i < sizeOfDataSet; i++){
        cout << '\t' << dataSet[i] << endl;
      }
    }
  
    short getWorkerID(){
      return workerID;
    }
  
    ~WorkerInfo(){
      cout << workerID << " destroying data set\n\n";
      delete [ ] dataSet;
    }
  
  private:
    short workerID = 0;
    short sizeOfDataSet = 0;
    short *dataSet = nullptr;
};

//worker controller thread function
void *boss(void *param){
  
  short runningWorkers = *((short *)param);
  bool workerRunning[*(short *)param ];
  
  for(int i = 0; i < *(short *)param; i++){
    workerRunning[i] = true;
  }
  
  cout << "!!!! Boss Thread Running!\n"
       << "Managing "
       << *(short *)param
       << " worker threads\n";
  cout << "Enter a number of 1 to "
       << *(short *)param
       << " to make a thread print its data set\n"
       << "Enter the negated value of a worker to cancel that thread\n"
       << "Enter 0 to make all threads output their data sets\n";
  int input;
  
  while(runningWorkers){
    
    cin >> input;
    
    // If input = 0
    if(input == 0){
      
      for(int i = 0; i < *(short *)param; i++){
        
        if(workerRunning[i]){
          pthread_cond_signal(&tEnable[i]);
        }
      }//for loop exits
      
      //loop 1 continue check?
    }
    // If input != 0
    if(input > 0 && input <= *(short *)param){
      
      if(workerRunning[input - 1]){
        pthread_cond_signal(&tEnable[input - 1]);
      }
      else{
        cout << "Worker " << input << " already finished\n";
      }
      //loop 1 continue check
    }
    
    if(input < 0 && (-input) <= *(short *)param){
      
      input = -input;
      
      if(workerRunning[input - 1]){
        cout << "Cancelling worker " << input << endl;
        cleanup_pop_arg = input;
        pthread_cancel(tids[input - 1]);
        workerRunning[input - 1] = false;
        runningWorkers--;
      }
      else{
        cout << "Worker " << input << " already cancelled\n";
      }
      //loop one continue check
    }
    else if(input > *(short *)param || (-input) > *(short *)param){
      cout << "ERROR: Invalid Entry\n";
    }
    //loop one continue check
    
    input = 0;
    
    if(runningWorkers == false){
      cout << "BOSS exits!\n";
      pthread_exit(0);
    }
  }
}

//worker thread function
void *worker(void *param){
  
  WorkerInfo myInfo(*(WorkerInfo*)param);
  
  cout << "Worker Thread "
       << myInfo.getWorkerID()
       << " Running!\n";
  
  signify++;
  
  pthread_cleanup_push(cleanupHandler, NULL);
  
  while(1){
    
    pthread_mutex_lock(&theMutex);
    pthread_cond_wait(&tEnable[myInfo.getWorkerID() - 1], &theMutex);
    myInfo.showDataSet();
    pthread_mutex_unlock(&theMutex);
  }
  pthread_cleanup_pop(cleanup_pop_arg);
}

int main(int argc, char ** argv){
  
  short numThreads = atoi(argv[1]); //number of threads from arg 1
  
  tids = new pthread_t[numThreads];
  tEnable = new pthread_cond_t[numThreads];
  pthread_t bossTid;
  pthread_mutex_init(&theMutex, 0);
  short dataSetIndex = 2;
  
  for(int i = 0; i < numThreads; i++){ //creating the workers
    
    short sizeOfDataSet = atoi(argv[dataSetIndex]);
    WorkerInfo tempWorker(i + 1, sizeOfDataSet);
    cout << "Temp worker " 
         << tempWorker.getWorkerID()
         << " initializing!\n";
    
    short dSet[sizeOfDataSet];
    
    dataSetIndex++;
    
    for(int j = 0; j < sizeOfDataSet; j++){
      dSet[j] = atoi(argv[dataSetIndex]);
      cout << "dSet[ " << j << " ] = " << dSet[j] << endl;
      dataSetIndex++;
    }
    
    cout << "Size of dataset: "
         << (sizeof(dSet) / sizeof(short))
         << endl;
    
    tempWorker.putDataInSet(dSet, sizeOfDataSet);
    pthread_cond_init(&tEnable[i], 0);
    cout << "Creating worker thread\n";
    short tmp = signify;
    pthread_create(&tids[i], NULL, worker, &tempWorker);
    //wait for thread to launch / initialize
    while(signify == tmp) ;
    cout << "temp worker ";
  }//end of for loop
  
  pthread_create(&bossTid, NULL, boss, &numThreads);
  cout << "Main thread blocking until boss thread finished\n\n";  
 
  // end
  pthread_join(bossTid, NULL);
  cout << "\n\nMain thread unblocked\n\n";
  for(int i = 0; i < numThreads; i++){
    pthread_cond_destroy(&tEnable[i]);
  }
  pthread_mutex_destroy(&theMutex);
  delete [ ] tids;         //deallocate heap memory
  delete [ ] tEnable;      //deallocate heap memory
  
  return 0;
}