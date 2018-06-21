#include <vector>
#include <pthread.h>
#include <memory>
#include <functional>

class ThreadSafeDeferredCaller {

typedef std::function<void()> DeferredMethod;

public:
  ThreadSafeDeferredCaller();
  void SetFunc(DeferredMethod func);
  void RunFunc();

private:
  pthread_mutex_t mutex;
  std::vector<std::function<void()>> f;
};
