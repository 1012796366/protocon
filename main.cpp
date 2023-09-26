#include "mainwindow.h"

#include <QApplication>
#include <thread>
#include <mutex>
#include <functional>
#include <chrono>

int taskCount = 0;
std::recursive_mutex queueLock;

void producer(MainWindow& w, int id)
{
    while (true)
    {
        queueLock.lock();
        ++taskCount;
        w.pushTask(taskCount);
        w.updateText(id, taskCount);
        queueLock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }
}

void consumer(MainWindow& w, int id)
{
    while (true)
    {
        queueLock.lock();
        int res = w.popTask();
        if (res != -1)
        {
            w.updateText(id + 1, res);
        }
        queueLock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("生产者——消费者程序模拟解决方案");
    std::thread pro1(producer, std::ref(w), 0);
    std::thread con1(consumer, std::ref(w), 0);
    std::thread con2(consumer, std::ref(w), 1);
    std::thread con3(consumer, std::ref(w), 2);
    pro1.detach();
    con1.detach();
    con2.detach();
    con3.detach();
    w.show();
    return a.exec();
}
