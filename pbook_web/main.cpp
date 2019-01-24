#include "mainwindow.h"
#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>


#ifdef _WIN32
    #include <Windows.h>
#endif

int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);
    MainWindow w;

    QSystemSemaphore global_semaphore("semaphore for k&k phonebook", 1);
    global_semaphore.acquire();
    QSharedMemory shared_memory("shared memory for k&k phonebook");
    if (shared_memory.attach())
    {
        #ifdef _WIN32
            if(shared_memory.lock())
            {
                HWND hwnd = (HWND) QString((char*) shared_memory.data()).toInt();
                ShowWindow( hwnd, SW_SHOW);
            }
            shared_memory.unlock();
        #endif
        global_semaphore.release();
        return -1;
    }
    else
    {
        shared_memory.create(sizeof(WId));
        if(shared_memory.lock())
        {
            char *shared_data = (char*) shared_memory.data();
            strcpy_s(shared_data, 16, QString::number(w.winId()).toStdString().data());
        }
        shared_memory.unlock();
        global_semaphore.release();
        w.show();
        return a.exec();
    }
}
