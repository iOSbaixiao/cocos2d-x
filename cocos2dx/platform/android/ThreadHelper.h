#ifndef __THREAD_HELPER_H__
#define __THREAD_HELPER_H__

#include <android/sensor.h>
#include <android/log.h>

#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>
/* class ConcurrentQueue { */
/*  public: */
/*   void put(std::function<void ()> handler_func); */
/*   std::function<void ()> get(); */
/*  private: */
/*   std::mutex _qMutex; */
/*   std::condition_variable _qContidion; */
/*   std::deque<std::function<void ()> > _q; */
/* }; */
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "cocos2dx/nativeactivity.cpp", __VA_ARGS__))

using namespace std;
template <typename T>
class LockFreeQueue {

 private:

  struct Node {
  Node(T val) : value(val), next(nullptr) { }
    T value;
    Node* next;
  };

  Node* first;             // for producer only
  int count;
  atomic<Node *> divider, last;         // shared

 public:

  LockFreeQueue() {
    first = divider = last =
      new Node( T() );           // add dummy separator
    count = 0;
  }

  ~LockFreeQueue() {
    while( first != nullptr ) {   // release the list
      Node* tmp = first;
      first = tmp->next;
      delete tmp;
    }
  }
  
  void Produce( const T& t ) {
    (last.load())->next = new Node(t);    // add the new item
    last = (last.load())->next;      // publish it

    while( first != (divider.load()) ) { // trim unused nodes
      Node* tmp = first;
      first = first->next;
      delete tmp;
    }
    count++;
  }

  int getCount() {
    return count;
  }
  
  bool Consume( T& result ) {
    if( (divider.load()) != (last.load()) ) {         // if queue is nonempty
      count--;
      result = (divider.load())->next->value;  // C: copy it back
      divider = (divider.load())->next;   // D: publish that we took it
      return true;              // and report success
    }
    return false;               // else report empty
  }
};


extern LockFreeQueue<function<void ()> > *lockQ;
void runOnGLThread(function<void ()> func);

#endif
