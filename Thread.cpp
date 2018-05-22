#include "Thread.h"


static void* ThreadRun(void *f)
{
  return reinterpret_cast<void*>(static_cast<IThread*>(f)->Run());
}

void Thread::Fork(IThread *t)
{
  pthread_create(&ti, NULL, ThreadRun, t);
}

long  Thread::Join()
{
  void *result;
  pthread_join(ti, &result);
  return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
 }
