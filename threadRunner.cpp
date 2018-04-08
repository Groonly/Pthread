#include <pthread.h>
#include <memory>
#include <iostream>
#include <cstdint>
#include <map>


struct DeferredCaller {
  int id[2] = {-1,-1};
  std::function<void()> f[2];
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

void OnThreadComplete(void){std::cout << "COMPLETED" << std::endl;}

class Math : public IThread {
private:
  double latestValue = 0;
  DeferredCaller& DFC;
public:
  Math(DeferredCaller& deferredCaller) : DFC(deferredCaller) {}
  template<typename T>
  void add(T value1, T value2){this->latestValue = value1 + value2;}
  int Run(){add(2,3); DFC.f[0] = std::bind(OnThreadComplete);  DFC.id[0] = 1; return 1;}
  double GetlatestValue(void){return latestValue;}
};




int main(){
std::map<std::string, double> OperationResults;


DeferredCaller deferredCalller;

Math m(deferredCalller);
Thread t;
t.Fork(&m);

while(1){
  if(deferredCalller.id[0] > 0){
    deferredCalller.f[0]();
    break;
  }
  if(deferredCalller.id[1] > 0){
    deferredCalller.f[1]();
  }
}

std::cout << t.Join() << std::endl;
std::cout << m.GetlatestValue() << std::endl;
return 0;
}
