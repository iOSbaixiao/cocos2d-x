#include "InputEventQueue.h"

using namespace std;

LockFreeQueue<std::function<void ()> > *inputEventQ = new LockFreeQueue<std::function<void ()> >();

void runOnGLThread(function<void ()> func) {
  inputEventQ->put(func);
}
