#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <QTranslator>
#include "linkedList.h"
#include "stack.h"
#include "def.h"
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
