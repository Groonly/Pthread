#include <pthread.h>
#include <memory>
#include <iostream>
#include <cstdint>
#include <map>
#include <vector>
#include <list>
#include "ThreadQueue.h"
#include "ThreadSafeDeferredCaller.h"
#include "Thread.h"

/*
class IThread{
public:
  IThread(){}
  virtual ~IThread(){}
  virtual  Run() = 0;
};
*/
/*
class ThreadedQueue {
private:
  pthread_t ti;
  pthread_mutex_t mutex;
  bool WorkerThreadRunning = true;

  std::list<std::function<void()>> work;
  void DoWork(){pthread_mutex_lock(&mutex); if(work.empty()) return; auto f=work.front(); work.pop_front(); pthread_mutex_unlock(&mutex); f();}
  WorkThread(){ while(WorkerThreadRunning){ DoWork();} }
  static void*ThreadRun(void *f){return reinterpret_cast<void*>(static_cast<ThreadedQueue*>(f)->WorkThread());}
public:
  ThreadedQueue(){pthread_mutex_init(&mutex, NULL); pthread_create(&ti, NULL, ThreadRun, this);}
  virtual ~ThreadedQueue(){ pthread_join(ti, NULL); }

  bool PushWork(std::function<void()> func){pthread_mutex_lock(&mutex); work.push_back(func); pthread_mutex_unlock(&mutex);}
  bool CancelAllJobs() { work.clear(); }
  int PendingJobs(){pthread_mutex_lock(&mutex); int i = work.size(); pthread_mutex_unlock(&mutex); return i; }
};

*/
/*
class Thread {
private:
  pthread_t ti;
  static void*ThreadRun(void *f) {return reinterpret_cast<void*>(static_cast<IThread*>(f)->Run());}
public:

  inline void Fork(IThread *t){ pthread_create(&ti, NULL, ThreadRun, t);}
  long Join(){void *result; pthread_join(ti, &result); return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));}
};
*/

/*class ThreadSafeDeferredCaller {

typedef std::function<void()> DeferredMethod;

public:
  ThreadSafeDeferredCaller(){pthread_mutex_init(&mutex, NULL);}
  void SetFunc(DeferredMethod func){
    pthread_mutex_lock(&mutex);
    f.push_back(func);
    pthread_mutex_unlock(&mutex);
  }
  void RunFunc(){
    pthread_mutex_lock(&mutex);
    if(f.empty()){
      pthread_mutex_unlock(&mutex);
      return;
    }
    else{
      DeferredMethod func = f.back();
      func();
      f.pop_back();
      pthread_mutex_unlock(&mutex);
    }
  }
private:
  pthread_mutex_t mutex;
  std::vector<std::function<void()>> f;
};*/

class Caller {
public:
  Caller(){pthread_mutex_init(&mutex, NULL); }
  void SetInt(int i){pthread_mutex_lock(&mutex); this->i = i; pthread_mutex_unlock(&mutex);}
  void GetInt(int& i){pthread_mutex_lock(&mutex); i = this->i; pthread_mutex_unlock(&mutex);}
  int GetInt(void){return i;}
private:
  int i = 0;
  pthread_mutex_t mutex;

};

void OnThreadComplete(void){std::cout << "MATH" << std::endl;}
void OnThreadText(void){std::cout << "TEXT" << std::endl;}
void WorkQueueJob(){for(int i = 0; i < 1000000; i++); std::cout << "Done" << "\n"; }


class Math : public IThread {
private:
  double latestValue = 0;
  ThreadSafeDeferredCaller& DFC;
  Caller& CLS;
public:
  Math(ThreadSafeDeferredCaller& deferredCaller, Caller& caller) : DFC(deferredCaller), CLS(caller) {}
  template<typename T>
  void add(T value1, T value2){this->latestValue = value1 + value2;}
  int Run(){add(2,3); DFC.SetFunc(std::bind(OnThreadComplete)); CLS.SetInt(latestValue); return 1;}
  int Run(int i){ Thread te; te.Fork(this);}
  double GetlatestValue(void){return latestValue;}
};

class Text : public IThread{
private:
  ThreadSafeDeferredCaller& DFC;
  Caller& CLS;
  int i;
public:
  Text(ThreadSafeDeferredCaller& deferredCaller, Caller& caller) : DFC(deferredCaller), CLS(caller) {}
  int Run(){DFC.SetFunc(std::bind(OnThreadText)); return 1;}
  int GetInt(void){return i;}
};


int main(){
std::map<std::string, double> OperationResults;
ThreadSafeDeferredCaller threadSafeDeferredCaller;
Caller caller;
ThreadedQueue tq;

for(int i = 0; i < 100; i++){
  tq.PushWork(WorkQueueJob);
  std::cout << "PendingJobs: " << tq.PendingJobs() << "\n";
}

Math m(threadSafeDeferredCaller, caller);
Text te(threadSafeDeferredCaller, caller);
Thread t;
//t.Fork(&m);
m.Run(1);
t.Fork(&te);

while(1){
  threadSafeDeferredCaller.RunFunc();
}

std::cout << t.Join() << std::endl;
std::cout << m.GetlatestValue() << std::endl;
return 0;
}
