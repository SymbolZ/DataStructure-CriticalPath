#ifndef DIALOG_IN_AR_H
#define DIALOG_IN_AR_H

#include <QDialog>
#include <QAbstractButton>
#include <QPushButton>
#include <string>
#include <QMessageBox>

namespace Ui {
class Dialog_in_ar;
}

class Dialog_in_ar : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_in_ar(QWidget *parent = 0);
    ~Dialog_in_ar();
    void set_ver_num(int i);
private slots:

    //确认键响应函数
    void on_pushButton_clicked();

    //取消键响应函数
    void on_pushButton_2_clicked();

signals:
    void send_arc_data(int start,int end,int value);
private:
    Ui::Dialog_in_ar *ui;
    int ver_num;
};

#endif // DIALOG_IN_AR_H
