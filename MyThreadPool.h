
#include "SafeQueue.cpp"
#include <mutex>
#include <future>
#include <thread>
#include <vector>
#include <functional>
#include <memory>
#include <condition_variable>



class MyThreadPool{
    private:
        //任务队列queue
        std::vector<std::thread> threadPool; //线程池
        std::condition_variable cv; //用于等待任务队列的条件变量
        bool is_shutdown; //判断是不是关闭
        int thread_num;
        SafeQueue<std::function<void()> > task; //任务队列
        int task_size; //当前任务数量
        std::mutex cv_mux; //互斥锁

        class threadfunc{ 
            private:
                MyThreadPool* thispool;
                int m_id;
                std::function<void()> func;   //用function来封装函数
            
            public:
                threadfunc(MyThreadPool* pool,int id):thispool(pool),m_id(id){}
                void operator()(){//重载操作符，当作函数对象用
                    //从任务队列中取任务
                    //如果任务队列为空，在挂起，等待唤醒
                    //考虑如何实现这个功能；互斥锁，只能实现对资源的互斥访问
                    //信号量，标记有多少资源量，如果没有资源了，阻塞线程；等有任务了，再唤醒；可以
                    //条件变量，条件设为任务队列是不是空。如果为空，那么wait挂起等嗲唤醒
                    while(!thispool->is_shutdown){
                        {
                            std::unique_lock lock(thispool->cv_mux);
                            if(thispool->task.empty()){
                                //任务列表为空
                                thispool->cv.wait(lock);
                            }
                            //此处为获得了锁，且任务队列不为空
                            thispool->task.front_pop(func);
                        }
                        func();
                    }
                    

                }
        };
    
    public:
        MyThreadPool(int threadnum=10):threadPool(threadnum),thread_num(threadnum),is_shutdown(false),task_size(0){}   //构造函数

        void init(){ //初始化线程池
            for(int i=0;i<thread_num;i++){
                threadPool[i]=std::thread(threadfunc(this,i)); 
            }
        }

        //因为我们可能还想要获取函数的执行结果，那submit函数最好返回类型为
        //std::future<任务返回类型>  也即std::future<decltype(f(args...));
        //用尾类型推导来写。
        template<typename F,typename ...Args> //目前不是一个异步函数
        auto submit(F &&f,Args &&...args)->std::future<decltype(f(args...))> {
            auto func=std::bind(std::forward<F>(f),std::forward<Args>(args)...); //用bind函数，把函数与参数绑定
            auto funcPtr=std::make_shared<std::packaged_task<decltype(f(args...))()> >(func);  //获得指针，这一步目前是多余的，可以直接func()
            //可能要封装成异步任务，因为以后还要获取任务的结果
            
            std::function<void()> myfunc=[funcPtr](){
                (*funcPtr)();
            };

            task.push(move(myfunc));
            cv.notify_one(); //唤醒一个线程

            return (*funcPtr).get_future();
        }

        void shutdown(){
            is_shutdown=true;
        }
};
