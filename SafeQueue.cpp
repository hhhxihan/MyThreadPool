
#include <SafeQueue.h>



SafeQueue::SafeQueue(); //构造函数
SafeQueue::~SafeQueue(); 

bool SafeQueue::push(T value);

void SafeQueue::front_pop(T& value);

bool SafeQueue::empty();

int SafeQueue::size();