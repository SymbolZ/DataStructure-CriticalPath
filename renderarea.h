#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPainter>
#include <cmath>
#include <QMessageBox>

#include "linkedlist.h"
#include "stack.h"
#include "ui_renderarea.h"

const int OFFSET_X = 70;
const int OFFSET_Y = 40;
//矩形长宽
const int LENGTH = 100;
const int WIDTH = 35;
//矩形左上角顶点的间距
const int INTERVAL_X = LENGTH + 15;
const int INTERVAL_Y = WIDTH + 0;

//图的半径、偏移量、距离中心长度
const int RADIUS = 30;
const int GRAPH_OFFSET_X = 1080;
const int GRAPH_OFFSET_Y = 200;
const int DISTANCE = 150;
const double PI = 3.1415926;

namespace Ui {
class RenderArea;
}

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(QWidget *parent = 0);
    ~RenderArea();
    //得到顶点数量，验证输入是否正确
    int get_length();
    void add_arc(int start,int end,int value);
    void add_exp();
    void add_vertex();

    LinkList<LinkList<edge>* > *ll;

    std::string GetKeyRouteStr();
private:
    Ui::RenderArea *ui;
    int row;//绘图行数
    int col;//绘图列数
    int out_stack_num;//用于统计出栈顶点数量
    LinkList<edge> *t;//用于显示
    LinkList<edge> *t2;//用于获取的临时指针
    struct edge ed;//得到边
    LinkedStack<LinkList<edge>* > *stk;//用于拓扑排序
    LinkedStack<LinkList<edge>* > *stk_KR;//用于关键路径
    void VisitNode(std::string str,LinkList<edge> *Node);//用于DFS获得关键路径

    string topo_order;//拓扑排序结果

    std::string route;//存储关键路径

protected:
    void paintEvent(QPaintEvent *)Q_DECL_OVERRIDE;
    void paintRect(QPainter *,int x,int y,bool arrow);//绘图函数，绘制一个长方形
    void paintArrow(QPainter *);//绘图函数，绘制一个箭头
    void paintgraph(QPainter *);//绘图函数，绘制图
public slots:
    void topo_start();//拓扑排序入栈步骤
    void topo_once();//单步拓扑排序
    void topo_all();//一次完成的拓扑排序
    void gene_key_route();//获得关键路径

signals:
    void send_topo_res();//只标记是否完成
    void send_topo_res(int,string topo_order);//标记拓扑完成结果
};



#endif // RENDERAREA_H
