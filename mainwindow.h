#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>

#include "ui_mainwindow.h"
#include "renderarea.h"
#include "dialog_in_ar.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();
private slots:
    void on_bt_link_clicked();//添加弧响应函数
    void on_bt_add_clicked();//添加按钮响应函数
    void on_bt_example_clicked();//添加示例图响应函数
    void on_bt_topo_clicked();//拓扑排序响应函数
    void on_bt_kr_clicked();//关键路径响应函数
    void add_arc(int,int,int);//接收添加弧窗口传回数据
    void rec_topo_res(int,string);//接收拓扑排序结果
private:
    RenderArea *area;
    int sortFlag;//-1为未排序，0为拓扑无序，1为拓扑有序
    QLabel *result;//显示排序结果

    QMenuBar *m_bar;//菜单栏
    QAction *bar_add_point;//菜单按钮，添加顶点
    QAction *bar_add_arc;//菜单按钮，添加弧

    void on_bt_vel_clicked();//求ve vl的函数

signals:
    void gene_key_route();//获得关键路径信号

};

#endif // MAINWINDOW_H
