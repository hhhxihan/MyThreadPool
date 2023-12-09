#include "SafeQueue.h"

SafeQueue::SafeQueue(); //构造函数
SafeQueue::~SafeQueue(); 

bool SafeQueue::push(T&& value){
    std::lock_guard<std::mutex> lock(mux);
    m_queue.push(value);
    
}

void SafeQueue::front_pop(T& value){
    std::lock_guard<std::mutex> lock(mux);
    m_queue.front(value);
    m_queue.pop();
}

bool SafeQueue::empty(){
    std::lock_guard<std::mutex> lock(mux);
    m_queue.empty();
}

int SafeQueue::size(){
    std::lock_guard<std::mutex> lock(mux);
    m_queue.size();
}