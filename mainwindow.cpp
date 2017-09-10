#include "mainwindow.h"
#include "topocontrol.h"
MainWindow::MainWindow()
{
    //设置窗口大小
    this->resize(QSize(1300,600));

    sortFlag = -1;//表示该图未开始排序

    //建立绘图区域
    area = new RenderArea;

    //初始化按钮和文字
    result = new QLabel;
    QPushButton *bt_add = new QPushButton;
    QPushButton *bt_link = new QPushButton;
    QPushButton *bt_topo = new QPushButton;
    QPushButton *bt_kr = new QPushButton;
    QPushButton *bt_example = new QPushButton;

    result->setText(" ");

    bt_link->setText("添加弧");
    connect(bt_link,SIGNAL(clicked()),
                          this, SLOT(on_bt_link_clicked())
                          );

    bt_kr->setText("获得关键路径");
    connect(bt_kr,SIGNAL(clicked()),
                          this, SLOT(on_bt_kr_clicked() )
                          );


    bt_add->setText("添加顶点");
    connect(bt_add,SIGNAL(clicked()),
                          this, SLOT(on_bt_add_clicked())
                          );

    bt_example->setText("添加示例图");
    connect(bt_example,SIGNAL(clicked()),
                          this, SLOT(on_bt_example_clicked())
                          );

    bt_topo->setText("拓扑排序");
    connect(bt_topo,SIGNAL(clicked()),
                          this, SLOT(on_bt_topo_clicked())
                          );
    //获得关键路径的信号
    connect(this,SIGNAL(gene_key_route()),
                          area, SLOT( gene_key_route() )
            );

    //初始化菜单
    m_bar = new QMenuBar;

    QMenu *menu1= new QMenu("图的添加");
    QMenu *menu2= new QMenu("图的操作");

    menu1->addAction("添加顶点",this,MainWindow::on_bt_add_clicked);
    menu1->addAction("添加弧",this,MainWindow::on_bt_link_clicked);

    menu2->addAction("拓扑排序",this,MainWindow::on_bt_topo_clicked);
    menu2->addAction("关键路径",this,MainWindow::on_bt_kr_clicked);

    m_bar->addMenu(menu1);
    m_bar->addMenu(menu2);
    m_bar->setStyleSheet("background-color: QColor(160,0,225);");

    //布局代码
    QGridLayout *layout = new QGridLayout;
    setLayout(layout);
    layout->addWidget(m_bar,0,0,1,9);
    layout->addWidget(area,1,0,14,9);

    layout->addWidget(bt_add,15,2,1,1);
    layout->addWidget(bt_link,16,2,1,1);

    layout->addWidget(bt_example,15,4,1,1);

    layout->addWidget(bt_topo,15,6,1,1);
    layout->addWidget(bt_kr,16,6,1,1);

    layout->addWidget(result,17,2,1,7);

    setWindowTitle("数据结构课程设计-算法实现3");

}


MainWindow::~MainWindow()
{
    //父对象的子对象会被自动回收
    //因此只有一个delete
}
void MainWindow::on_bt_link_clicked()
{


    Dialog_in_ar *d = new Dialog_in_ar;
    d->set_ver_num(area->get_length());
    connect(d,SIGNAL(send_arc_data(int,int,int) ),
            this,SLOT(add_arc(int,int,int)) );

    d->show();

}

void MainWindow::on_bt_add_clicked()
{
    area->add_vertex();
}
void MainWindow::on_bt_kr_clicked()
{

    if(sortFlag==-1)
    {
        QMessageBox::warning(this, tr("错误"),
                               QString(tr("获得关键路径前需要先进行拓扑排序")),
                               QMessageBox::Yes);
        return;
    }
    if(sortFlag==0)
    {
        QMessageBox::warning(this, tr("错误"),
                               QString(tr("该图拓无序，无法获得关键路径")),
                               QMessageBox::Yes);
        return;
    }

    emit(gene_key_route());
    string str = area->GetKeyRouteStr();

    result->setText(QString::fromStdString(
                        str.substr(0,str.length() - 4) ) );


    on_bt_vel_clicked();


}

void MainWindow::on_bt_vel_clicked()
{
    //创建表格，设置标题
    QTableWidget  *table = new QTableWidget(area->ll->ListLength(),4);
    table->setWindowTitle("关键路径信息");

    //设置为自动适应表格行列数
    table->setSizeAdjustPolicy(QTableWidget::AdjustToContents);


    QStringList headLabels;
    headLabels<<"顶点"<<"是否关键"<<"Ve[i]"<<"Vl[i]";
    table->setHorizontalHeaderLabels(headLabels);

    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);


    //获得邻接链表
    LinkList<LinkList<edge>* > *ptr = this->area->ll;
    LinkList<edge>* list;

    QTableWidgetItem *item0, *item1,*item2,*item3;
    char c[10];
    for (int row = 0; row < ptr->ListLength(); ++row)
    {
        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;
        item2 = new QTableWidgetItem;
        item3 = new QTableWidgetItem;

        ptr->GetElem(row,&list);

        itoa(row,c,10);
        QString txt = QString("V")+QString(c);
        item0->setText(txt);
        table->setItem(row, 0, item0);


        txt = QString("%1").arg(list->ve);
        item2->setText(txt);
        table->setItem(row, 2, item2);

        txt = QString("%1").arg(list->vl);
        item3->setText(txt);
        table->setItem(row, 3, item3);

        if(list->ve == list->vl)
            txt = QString("x");
        else
            txt = QString(" ");
        item1->setText(txt);
        table->setItem(row, 1, item1);
    }
    table->show();
}

void MainWindow::add_arc(int start,int end ,int value)
{
    //错误输入已经在输入界面处理
    //此处不再处理错误输入
    area->add_arc(start,end,value);
}

void MainWindow::on_bt_example_clicked()
{
    area->add_exp();
}
void MainWindow::on_bt_topo_clicked()
{
    //创建拓扑排序控制界面
    TopoControl *tc = new TopoControl;

    area->topo_start();
    connect(tc,SIGNAL(send_topo_once()),
                          area, SLOT( topo_once() )
            );
    connect(tc,SIGNAL(send_topo_all()),
                          area, SLOT( topo_all() )
            );
    connect(area,SIGNAL(send_topo_res()),
                          tc, SLOT( rec_topo_res() )
            );
    //主界面与画图部分
    connect(area,SIGNAL(send_topo_res(int,string)),
                          this, SLOT( rec_topo_res(int,string) )
            );

    //显示拓扑排序控制界面
    tc->show();

}
void MainWindow::rec_topo_res(int i,string topo_order)
{
    sortFlag = i;
    if(sortFlag == 0)
    {
        result->setText("该图拓扑无序");
    }
    else if(sortFlag == 1)
    {
        result->setText(QString::fromStdString("该图拓扑有序,序列为" + topo_order) );
    }
}
