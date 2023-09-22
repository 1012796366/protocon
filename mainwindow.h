#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <queue>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void pushTask(int id);
    int popTask();

    void updateText(int id, int taskID);


private:
    Ui::MainWindow *ui;
    std::queue<int> queueTask;

};
#endif // MAINWINDOW_H
