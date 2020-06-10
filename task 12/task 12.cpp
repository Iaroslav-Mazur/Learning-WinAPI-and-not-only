#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>


class ReferenceCounter {
public:
    ReferenceCounter()
    {
        counter = 0;
    }
    void increase()
    {
        counter++;
    }
    void decrease()
    {
        counter--;
    }
    unsigned int getCount()
    {
        return counter;
    }

private:
    std::atomic<unsigned int> counter;
};

class SharedPointer {
    int* data{ };
    ReferenceCounter* rc;
public:
    SharedPointer(int* data)
    {
        int* tmp{ new int{ } };
        if (data != nullptr)
        {
            *tmp = *data;
            this->data = tmp;
        }
        rc = new ReferenceCounter();
        rc->increase();
        std::cout << "The number of references to the int is now " << rc->getCount() << std::endl;
    }
    SharedPointer(SharedPointer& sharedPointer) : data{ sharedPointer.data },
                                                  rc{ sharedPointer.rc }
    {
        rc->increase();
        std::cout << "The number of references to the int is now " << rc->getCount() << std::endl;
    }
    SharedPointer(SharedPointer&& sharedPointer) = delete;
    ~SharedPointer()
    {
        rc->decrease();
        std::cout << "The number of references to the int is now " << rc->getCount() << std::endl;
        if (rc->getCount() == 0)
        {
            delete rc;
            if (data != nullptr)
            {
                delete data;
            }
        }
    }

    int getData()
    {
        return *data;
    }

    unsigned int getCount()
    {
        return rc->getCount();
    }
};

void myThread(SharedPointer* shared_ptr_)
{
    SharedPointer p(*shared_ptr_);
    std::cout << "The value referenced by the shared pointer inside the child thread: " 
                << p.getData() << std::endl;
}

int main()
{
    int a{ 44 };
    SharedPointer sp(&a);
    SharedPointer* sp2{ new SharedPointer(sp)};
    SharedPointer sp3(*sp2);
    delete sp2;

    std::vector<std::thread> threads{ };
    for (int i=0; i<10; i++)
    {
        std::cout << "Spawning a new thread" << std::endl;
        threads.emplace_back(myThread, &sp);
    }

    std::cout << "Waiting for the thread to finish working" << std::endl;
    for (auto& thread : threads)
    {
        thread.join();
    }

    std::cout << "Quitting..." << std::endl;

    return 0;    
}