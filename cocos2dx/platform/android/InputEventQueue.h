#ifndef __INPUT_EVENT_QUEUE_H__
#define __INPUT_EVENT_QUEUE_H__

#include <android/sensor.h>
#include <android/log.h>

#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>

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
  std::atomic<Node *> divider, last;         // shared

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
  
  void put( const T& t ) {
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
  
  bool get( T& result ) {
    if( (divider.load()) != (last.load()) ) {         // if queue is nonempty
      count--;
      result = (divider.load())->next->value;  // C: copy it back
      divider = (divider.load())->next;   // D: publish that we took it
      return true;              // and report success
    }
    return false;               // else report empty
  }
};


extern LockFreeQueue<std::function<void ()> > *inputEventQ;
void runOnGLThread(std::function<void ()> func);

#endif
