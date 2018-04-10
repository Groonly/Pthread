#include <pthread.h>
#include <memory>
#include <iostream>
#include <cstdint>
#include <map>


struct DeferredCaller {
  int id[2] = {-1,-1};
  std::function<void()> f[2];
};


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


class IThread{
public:
  IThread(){}
  virtual ~IThread(){}
  virtual  Run() = 0;
};


class Thread {
private:
  pthread_t ti;
  static void*ThreadRun(void *f) {return reinterpret_cast<void*>(static_cast<IThread*>(f)->Run());}
public:

  inline void Fork(IThread *t){ pthread_create(&ti, NULL, ThreadRun, t);}
  long Join(){void *result; pthread_join(ti, &result); return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));}
};

void OnThreadComplete(void){std::cout << "MATH" << std::endl;}
void OnThreadText(void){std::cout << "TEXT" << std::endl;}

class Math : public IThread {
private:
  double latestValue = 0;
  DeferredCaller& DFC;
  Caller& CLS;
public:
  Math(DeferredCaller& deferredCaller, Caller& caller) : DFC(deferredCaller), CLS(caller) {}
  template<typename T>
  void add(T value1, T value2){this->latestValue = value1 + value2;}
  int Run(){add(2,3); DFC.f[0] = std::bind(OnThreadComplete);  DFC.id[0] = 1; CLS.SetInt(7); return 1;}
  double GetlatestValue(void){return latestValue;}
};

class Text : public IThread{
private:
  DeferredCaller& DFC;
  Caller& CLS;
  int i;
public:
  Text(DeferredCaller& deferredCaller, Caller& caller) : DFC(deferredCaller), CLS(caller) {}
  int Run(){DFC.f[1] = std::bind(OnThreadText); DFC.id[1] = 1; CLS.GetInt(i); return 1;}
};


int main(){
std::map<std::string, double> OperationResults;


DeferredCaller deferredCalller;
Caller caller;
Math m(deferredCalller, caller);
Text te(deferredCalller, caller);
Thread t;
t.Fork(&m);
t.Fork(&te);

while(1){
  if(deferredCalller.id[0] > 0){
    deferredCalller.f[0]();
    deferredCalller.id[0] = -1;
  }
  if(deferredCalller.id[1] > 0){
    deferredCalller.f[1]();
    std::cout << caller.GetInt() << std::endl;
    deferredCalller.id[1] = -1;
  }
}

std::cout << t.Join() << std::endl;
std::cout << m.GetlatestValue() << std::endl;
return 0;
}
