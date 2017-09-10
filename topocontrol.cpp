#include "topocontrol.h"
#include "ui_topocontrol.h"

TopoControl::TopoControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopoControl)
{
    ui->setupUi(this);
    this->setWindowTitle("拓扑排序控制面板");
    topo_finish = 0;
}

TopoControl::~TopoControl()
{
    delete ui;
}

void TopoControl::on_pushButton_clicked()
{
    //单步拓扑
    emit(send_topo_once());
}

void TopoControl::on_pushButton_2_clicked()
{
    //拓扑排序
    emit(send_topo_all());
}

void TopoControl::rec_topo_res()
{
    topo_finish =1;
    close();
}

void TopoControl::closeEvent(QCloseEvent *qc)
{
    if(!topo_finish)
        qc->ignore();//忽略推出请求
    close();
}
