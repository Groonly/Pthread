#include "ThreadSafeDeferredCaller.h"

ThreadSafeDeferredCaller::ThreadSafeDeferredCaller()
{
  pthread_mutex_init(&mutex, NULL);
}

void ThreadSafeDeferredCaller::SetFunc(DeferredMethod func)
{
  pthread_mutex_lock(&mutex);
  f.push_back(func);
  pthread_mutex_unlock(&mutex);
}
void ThreadSafeDeferredCaller::RunFunc(){
  pthread_mutex_lock(&mutex);
  if (f.empty()) {
    pthread_mutex_unlock(&mutex);
    return;
}
  else {
    DeferredMethod func = f.back();
    func();
    f.pop_back();
    pthread_mutex_unlock(&mutex);
  }
}
