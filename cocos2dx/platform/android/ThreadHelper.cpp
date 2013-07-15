#include "ThreadHelper.h"

using namespace std;

// void ConcurrentQueue::put(function<void ()> handler_fun) {
//   {
//     unique_lock<mutex> lock(this->_qMutex);
//     _q.push_back(handler_func);
//   }
//   _qCondition.notify_one();
// }

// function<void ()> ConcurrentQueue::get() {
  
//   function<void ()> tmp;
//   unique_lock<mutex> lock(this->_qMutex);
  
//   _qMutex.lock();
//   if(_q.empty()) {
//     return NULL;
//   }
//   tmp = _q.front();
//   _q.pop();
//   _qMutex.unlock();
//   return tmp;

// }

LockFreeQueue<function<void ()> > *lockQ = new LockFreeQueue<function<void ()> >();

void runOnGLThread(function<void ()> func) {
  lockQ->Produce(func);
}
