#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <mutex>
#include <thread>
#include <functional>
#include <chrono>

extern int taskCount;
extern std::recursive_mutex queueLock;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    queueLock.lock();
    QObject::connect(ui->start_button, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, [=](bool check){
        ui->start_button->setCheckable(!check);
        if (check)
        {
            queueLock.unlock();
            ui->start_button->setText("暂停");
        }
        else
        {
            queueLock.lock();
            ui->start_button->setText("继续");
        }

    });
    QObject::connect(ui->stop_button, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, [=](bool check){
        queueLock.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
        ui->pro1_text->clear();
        ui->con1_text->clear();
        ui->con2_text->clear();
        ui->con3_text->clear();
        {
            std::queue<int> empty;
            queueTask.swap(empty);
        }
        taskCount = 0;
        queueLock.unlock();
        ui->stop_button->setEnabled(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
        ui->stop_button->setEnabled(true);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pushTask(int id)
{
    queueTask.push(id);
}

int MainWindow::popTask()
{
    if (queueTask.empty())
    {
        return -1;
    }
    int idTask = queueTask.front();
    queueTask.pop();
    return idTask;
}

void MainWindow::updateText(int id, int taskID)
{
    switch (id)
    {
    case 0:
        ui->pro1_text->insertPlainText("生产者生产了商品#");
        ui->pro1_text->insertPlainText(QString::number(taskID));
        ui->pro1_text->insertPlainText("\n");
        break;
    case 1:
        ui->con1_text->insertPlainText("消费者#1消费了商品#");
        ui->con1_text->insertPlainText(QString::number(taskID));
        ui->con1_text->insertPlainText("\n");
        break;
    case 2:
        ui->con2_text->insertPlainText("消费者#2消费了商品#");
        ui->con2_text->insertPlainText(QString::number(taskID));
        ui->con2_text->insertPlainText("\n");
        break;
    case 3:
        ui->con3_text->insertPlainText("消费者#3消费了商品#");
        ui->con3_text->insertPlainText(QString::number(taskID));
        ui->con3_text->insertPlainText("\n");
        break;
    default:
        break;
    }
}
