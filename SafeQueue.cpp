#include "SafeQueue.h"

template<typename T>
bool SafeQueue<T>::push(T&& value){
    std::lock_guard<std::mutex> lock(mux);
    m_queue.push(value);
    
    return true;
}

template<typename T>
void SafeQueue<T>::front_pop(T& value){
    std::lock_guard<std::mutex> lock(mux);
    m_queue.front(value);
    m_queue.pop();
}

template<typename T>
bool SafeQueue<T>::empty(){
    std::lock_guard<std::mutex> lock(mux);
    return m_queue.empty();
}

template<typename T>
int SafeQueue<T>::size(){
    std::lock_guard<std::mutex> lock(mux);
    return m_queue.size();
}