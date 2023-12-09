//支持互斥访问的队列

#include <mutex>
#include <queue>

Template<typename T>
class SafaQueue{
    private:
        std::queue<T> m_queue;
        std::mutex mux;
    
    public:
        SafeQueue(); //构造函数
        ~SafeQueue(); 

        bool push(T value);

        void front_pop(T& value);

        bool empty();

        int size();

    
};