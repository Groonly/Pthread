#include "ThreadQueue.h"

void ThreadedQueue::DoWork()
{
  pthread_mutex_lock(&mutex);
  if(work.empty()) return;
  auto f=work.front();
  work.pop_front();
  pthread_mutex_unlock(&mutex);
  f();
}

ThreadedQueue::WorkThread()
{
  while(WorkerThreadRunning){
     DoWork();
   }
 }
void* ThreadedQueue::ThreadRun(void *f)
{
  return reinterpret_cast<void*>(static_cast<ThreadedQueue*>(f)->WorkThread());
}

ThreadedQueue::ThreadedQueue()
{
  pthread_mutex_init(&mutex, NULL);
  pthread_create(&ti, NULL, ThreadRun, this);
}
ThreadedQueue::~ThreadedQueue()
{
  pthread_join(ti, NULL);
}

bool ThreadedQueue::PushWork(std::function<void()> func)
{
  pthread_mutex_lock(&mutex);
  work.push_back(func);
  pthread_mutex_unlock(&mutex);
}
bool ThreadedQueue::CancelAllJobs()
{
  work.clear();
}
int ThreadedQueue::PendingJobs()
{
  pthread_mutex_lock(&mutex);
  int size = work.size();
  pthread_mutex_unlock(&mutex);
  return size;
}
