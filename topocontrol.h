#ifndef TOPOCONTROL_H
#define TOPOCONTROL_H

#include <QDialog>
#include <QCloseEvent>
namespace Ui {
class TopoControl;
}

class TopoControl : public QDialog
{
    Q_OBJECT

public:
    explicit TopoControl(QWidget *parent = 0);
    ~TopoControl();
    void closeEvent(QCloseEvent *);

private slots:
    void on_pushButton_clicked();//进行一次拓扑排序步骤
    void on_pushButton_2_clicked();//进行拓扑排序

signals:
    void send_topo_once();
    void send_topo_all();
private:
    Ui::TopoControl *ui;
    int topo_finish;//排序完成标记
public slots:
    void rec_topo_res();//从MainWindow获得拓扑排序标记
};

#endif // TOPOCONTROL_H
