#include <pthread.h>
#include <memory>
#include "IThread.h"

class Thread {
public:
  void Fork(IThread *t);
  long Join();
private:
  pthread_t ti;
};
