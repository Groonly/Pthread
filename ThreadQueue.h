#include <pthread.h>
#include <iostream>
#include <list>
#include <memory>

class ThreadedQueue {
private:
  pthread_t ti;
  pthread_mutex_t mutex;
  bool WorkerThreadRunning = true;
  std::list<std::function<void()>> work;

  void DoWork();
  WorkThread();
  static void*ThreadRun(void *f);

public:
  ThreadedQueue();
  virtual ~ThreadedQueue();
  bool PushWork(std::function<void()> func);
  bool CancelAllJobs();
  int PendingJobs();
};
