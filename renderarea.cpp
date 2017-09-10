#include "renderarea.h"
#include <QDebug>
const QPoint QPOINT = QPoint(12,20);

QString iToQString(int i)
{
    char c[6];
    itoa(i,c,10);
    return QString(c);
}
string iToString(int i)
{
    char c[6];
    itoa(i,c,10);
    return string(c);
}
RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderArea)
{
    route = "关键路径为:";

    ll = new  LinkList<LinkList<edge>* >;

    //设置背景颜色和窗口大小
    QPalette p;
    p.setBrush(this->backgroundRole(),QBrush(QColor(255,255,255)));
    this->setPalette(p);
    this->setAutoFillBackground(true);
    ui->setupUi(this);
    this->resize(QSize(400,300));

    //设置绘图参数
    this->row = 0;

    stk = new LinkedStack<LinkList<edge>* >;
    stk_KR = new LinkedStack<LinkList<edge>* >;
    out_stack_num = 0;
}

RenderArea::~RenderArea()
{
    delete ui;
}
void RenderArea::paintEvent(QPaintEvent *)
{

    QPainter QP(this);
    bool arrow = true;

    //获得当前顶点数量
    this->row =ll->ListLength();

    QP.drawText(QPoint(OFFSET_X,OFFSET_Y-20),"顶点");
    QP.drawText(QPoint(OFFSET_X,OFFSET_Y-3),"编号");
    QP.drawText(QPoint(OFFSET_X+32,OFFSET_Y-3),"入度");
    QP.drawText( QPoint(OFFSET_X + 1*INTERVAL_X , OFFSET_Y -3),"目标" );
    QP.drawText( QPoint(OFFSET_X + 1*INTERVAL_X + 34 , OFFSET_Y -3),"权值" );

    //绘制邻接链表
    for(int i = 0;i<this->row;++i)
    {
        ll->GetElem(i,&t);
        this->col = t->ListLength();

        for(int j = 0;j<this->col + 1;++j)
        {
            arrow = j == this->col? false : true;
            paintRect(&QP,j,i,arrow);
        }
    }

    //绘制图
    paintgraph(&QP);

}

void RenderArea::paintRect(QPainter *QP,int x,int y,bool arrow)
{
    //让第一个框比其它框大
    //若为第一个框，该值为0，绘制时宽减去该值
    int is_first = x == 0? 0:1;
    is_first = is_first*7;

    QP->save();

    QP->translate( OFFSET_X + x*INTERVAL_X , OFFSET_Y + y*INTERVAL_Y );
    //绘制箭头
    if(arrow)
        paintArrow(QP);

    //变换坐标系，并绘制矩形的最左部份
    QP->drawRect(QRect( QPoint(0,0),QPoint(LENGTH/3,WIDTH-is_first) ) );

    t->GetElem(x-1,&ed );

    if(x)
        QP->drawText(QPOINT,iToQString(ed.dest) );
    else
        //若为第一个结点，打印序号
        QP->drawText(QPOINT,iToQString(y));

    //变换坐标系，并绘制矩形的中间部份
    QP->translate( LENGTH/3 , 0 );
    QP->drawRect(QRect( QPoint(0,0),QPoint(LENGTH/3,WIDTH-is_first) ) );
    if(x)
        QP->drawText(QPOINT,iToQString(ed.dur) );
    else
        //若为第一个结点，打印入度
        QP->drawText(QPOINT,iToQString(t->indegree));


    //变换坐标系，并绘制矩形的最右部份


    QP->translate( LENGTH/3 , 0 );
    QP->drawRect(QRect( QPoint(0,0),QPoint(LENGTH/3,WIDTH-is_first) ) );
    if(!arrow)
    {
        QP->drawText(QPOINT,QString("Λ"));
    }
    QP->restore();

}

void RenderArea::paintArrow(QPainter *QP)
{
    QP->save();

    //变换坐标系，并绘制直线
    QP->translate( LENGTH*0.83 , WIDTH*0.5 );

    //INTERVAL_X - LENGTH 为矩形最近两边间隙
    QP->drawLine(QPoint(0,0),QPoint( LENGTH*0.17+ INTERVAL_X - LENGTH,0));

    //绘制箭头尖端
    QP->translate(  LENGTH*0.17+ INTERVAL_X - LENGTH ,0);
    QP->drawLine(QPoint(-5,-5),QPoint(0, 0));
    QP->drawLine(QPoint(-5,5),QPoint(0, 0));


    QP->restore();
}

void RenderArea::paintgraph(QPainter *QP)
{

    //坐标变换
    QP->save();
    QP->translate( GRAPH_OFFSET_X , GRAPH_OFFSET_Y );
    QP->setBrush(Qt::SolidPattern);


    int number = ll->ListLength();

    double deg_gap;
    if(number)
        deg_gap= 360/number;
    else
        deg_gap = 0;

    double deg = 0;
    QPoint qpoint;
    QPoint qp_arrow_l;
    QPoint qp_arrow_r;
    QPoint qpoint_end;


    //设置画刷，用于圆内部
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor::fromRgb(255,255,255) );


    //绘制直线与箭头
    int arrow_order = 0;
    double edge1,edge2,edge3;
    double v_tan;
    double v_sin;
    double v_cos;
    double dtemp;

    for(int i = 0;i<this->row;++i)
    {
        ll->GetElem(i,&t);
        this->col = t->ListLength();

        for(int j = 0;j<this->col ;++j)
        {
            t->GetElem(j,&ed);
            arrow_order  = ed.dest;
            //绘制直线
            qpoint.setX(cos( (deg+ i*deg_gap) *PI/180 ) * DISTANCE);
            qpoint.setY(sin( (deg+ i*deg_gap) *PI/180 ) * DISTANCE);

            qpoint_end.setX(cos( (deg+ arrow_order*deg_gap) *PI/180 ) * DISTANCE);
            qpoint_end.setY(sin( (deg+ arrow_order*deg_gap) *PI/180 ) * DISTANCE);

            QP->drawLine(qpoint,qpoint_end);

            //计算三角函数值，用于画箭头
            edge1 =qpoint_end.y() - qpoint.y();
            edge2 =qpoint_end.x() - qpoint.x();
            edge3 = sqrt(edge1*edge1+edge2*edge2);
            v_tan = edge1 / edge2;
            v_cos = edge2 / edge3;
            v_sin = edge1/edge3;

            qpoint.setX( qpoint_end.x() - v_cos*RADIUS*1.5);
            qpoint.setY( qpoint_end.y() - v_sin*RADIUS*1.5);

            qpoint_end.setX( qpoint.x() + v_cos*RADIUS*0.5);
            qpoint_end.setY( qpoint.y() + v_sin*RADIUS*0.5);
            //计算垂直线三角函数值，用于画箭头
            v_tan = -1/v_tan;
            dtemp = v_cos;
            v_cos = -v_sin;
            v_sin = dtemp;

            //获得箭头两翼坐标
            qp_arrow_l.setX(qpoint.x() - v_cos*5);
            qp_arrow_l.setY(qpoint.y() - v_sin*5);

            qp_arrow_r.setX(qpoint.x() + v_cos*5);
            qp_arrow_r.setY(qpoint.y() + v_sin*5);

            QP->drawLine(qp_arrow_l,qpoint_end);
            QP->drawLine(qp_arrow_r,qpoint_end);
        }
    }

    for(int i = 0;i<number;++i)
    {
        //外部黑色实心圆
        qpoint.setX(cos( (deg+ i*deg_gap) *PI/180 ) * DISTANCE);
        qpoint.setY(sin( (deg+ i*deg_gap) *PI/180 ) * DISTANCE);
        QP->drawEllipse(qpoint,RADIUS,RADIUS);
        //内部白色实心圆，用于擦除线条
        QP->setBrush(brush);
        QP->drawEllipse(qpoint,RADIUS-1,RADIUS-1);
        //QP->set
        QP->drawText(qpoint,iToQString(i));
    }


    QP->restore();

}
int RenderArea::get_length()
{

    return ll->ListLength();
}

void RenderArea::add_arc(int start,int end,int value)
{
    if(ll->GetElem(start,&t))
    {
        if(t->ListLength()>6)
            QMessageBox::critical(this,
                           "显示将出现错误" ,
                           "邻接链表过长",
                           QMessageBox::Ok | QMessageBox::Default ,
                           0,0 );
        ed ={end,value,0,INT_MAX};
        t->Append_elem(ed);
        ll->GetElem(end,&t2);
        t2->IncreaseIndegree();
    }
}

void RenderArea::add_vertex()
{
    if(ll->ListLength()>=10)
    {
        QMessageBox::information(this, tr("提示"),
                               QString(tr("顶点数超过十个时，右侧显示部分会出现错误")),
                               QMessageBox::Yes);
    }
    LinkList<edge> *temp;
    temp = new LinkList<edge>;//析构函数带有delete代码
    temp->n = ll->ListLength();
    temp->ve = 0;
    temp->vl = INT_MAX;

    ll->Append_elem(temp);
    update();
}

void RenderArea::topo_start()
{
    //拓扑排序准备，将入度为0的顶点入栈
    for(int i = 0;i<ll->ListLength();++i)
    {
        ll->GetElem(i,&t);
        if(!t->GetIndegree())
            stk->Push(t);
    }

}
void RenderArea::topo_once()
{
    int position;
    if(!stk->StackEmpty())
    {
        stk->Pop(&t);
        topo_order += iToString(t->n);
        topo_order +=" ";
        stk_KR->Push(t);

        //删除对应的边
        if(!t->GetIndegree())
        {
            ++out_stack_num;//统计入栈结点数量
            for(int j = 0;j<t->ListLength();++j)
            {
                t->GetElem(j,&ed);
                position = ed.dest;//得到指向结点位置position
                ll->GetElem(position,&t2);
                t2->DecreaseIndegree();
                if(t2->indegree == 0)
                    stk->Push(t2);
                if(t->ve + ed.dur > t2->ve)
                    t2->ve = t->ve + ed.dur;
            }
            t->DecreaseIndegree();//设置标记-1
        }
    }
    else
    {
        emit(send_topo_res());
        if(out_stack_num < ll->ListLength())
        {
            QMessageBox::about(this,
                           "拓扑排序结果",
                           "该图拓扑无序");
            emit(send_topo_res(0, topo_order) );//拓扑排序结果
        }
        else
        {
            QMessageBox::about(this,
                           "拓扑排序结果",
                           "该图拓扑有序");
            emit(send_topo_res(1,topo_order) );
        }
    }

    update();

}

void RenderArea::topo_all()
{
    int position;
    while(!stk->StackEmpty())
    {
        stk->Pop(&t);
        topo_order += iToString(t->n);
        topo_order += " ";
        stk_KR->Push(t);

        //删除对应的边
        if(!t->GetIndegree())
        {
            ++out_stack_num;//统计入栈结点数量
            for(int j = 0;j<t->ListLength();++j)
            {
                t->GetElem(j,&ed);
                position = ed.dest;//得到指向结点位置position
                ll->GetElem(position,&t2);
                t2->DecreaseIndegree();
                if(t2->indegree == 0)
                    stk->Push(t2);
                if(t->ve + ed.dur > t2->ve)
                    t2->ve = t->ve + ed.dur;
            }
            t->DecreaseIndegree();//设置标记-1
        }
     }

    if(out_stack_num < ll->ListLength())
    {
        QMessageBox::about(this,
                       "拓扑排序结果",
                       "该图拓扑无序");
        emit(send_topo_res(0,topo_order));

    }
    else
    {
        QMessageBox::about(this,
                       "拓扑排序结果",
                       "该图拓扑有序");
        emit(send_topo_res(1,topo_order));
    }

    update();
    emit(send_topo_res());
}
void RenderArea::add_exp()
{
    //添加示例图
    static int flag = 0;
    if(flag)
    {
        QMessageBox::warning(this, tr("错误"),
                               QString(tr("只能添加一次示例")),
                               QMessageBox::Yes);
        return;
    }
    flag = 1;

    LinkList<edge> *temp;//用于创建

    for (int i = 0;i<9;++i)
    {
        temp = new LinkList<edge>;//析构函数带有delete代码
        temp->n = i;
        temp->ve = 0;
        temp->vl = INT_MAX;
        ll->Append_elem(temp);

    }


    if(ll->GetElem(0,&t))
    {
        ed ={1,6,0,INT_MAX};
        t->Append_elem(ed);
        ll->GetElem(1,&t2);
        t2->IncreaseIndegree();

        ed = {2,4,0,INT_MAX};
        t->Append_elem(ed);
        ll->GetElem(2,&t2);
        t2->IncreaseIndegree();

        ed = {3,5,0,INT_MAX};
        t->Append_elem(ed);
        ll->GetElem(3,&t2);
        t2->IncreaseIndegree();
    }
    if(ll->GetElem(1,&t))
    {
        ed = {4,1,0,INT_MAX};
        t->Append_elem(ed);
        ll->GetElem(4,&t2);
        t2->IncreaseIndegree();
    }

    if(ll->GetElem(2,&t))
    {
        ed = {4,1,0,INT_MAX};
        t->Append_elem(ed);
        ll->GetElem(4,&t2);
        t2->IncreaseIndegree();
    }

    if(ll->GetElem(3,&t))
    {
        ed = {5,2,0,INT_MAX};
        t->Append_elem(ed);
        ll->GetElem(5,&t2);
        t2->IncreaseIndegree();
    }


    if(ll->GetElem(4,&t))
    {
        ed = {6,9,0,INT_MAX};
        t->Append_elem(ed);
        ll->GetElem(6,&t2);
        t2->IncreaseIndegree();

        ed = {7,7,0,INT_MAX};
        t->Append_elem(ed);
        ll->GetElem(7,&t2);
        t2->IncreaseIndegree();
    }

    if(ll->GetElem(5,&t))
    {
        ed = {7,4,0,INT_MAX};
        t->Append_elem(ed);
        ll->GetElem(7,&t2);
        t2->IncreaseIndegree();
    }

    if(ll->GetElem(6,&t))
    {
        ed = {8,2,0,INT_MAX};
        t->Append_elem(ed);
        ll->GetElem(8,&t2);
        t2->IncreaseIndegree();
    }

    if(ll->GetElem(7,&t))
    {
        ed = {8,4,0,INT_MAX};
        t->Append_elem(ed);
        ll->GetElem(8,&t2);
        t2->IncreaseIndegree();
    }
    this->row = ll->ListLength();
    update();
}
void RenderArea::gene_key_route()
{
    int last_point = 1;
    int position;
    while(!stk_KR->StackEmpty())
    {
        stk_KR->Pop(&t);
        //stk->Push(&t);
        if(last_point)
        {
            last_point = 0;
            t->vl=t->ve;
        }
        else
        {
            for(int i = 0;i<t->ListLength();++i)
            {
                t->GetElem(i,&ed);
                position = ed.dest;//得到指向结点位置position

                ll->GetElem(position,&t2);
                if(t2->ve-ed.dur < t->vl)
                    t->vl = t2->vl-ed.dur;

            }

        }
    }

}

std::string RenderArea::GetKeyRouteStr()
{
    //获得起始结点
    qDebug("%d",ll->ListLength());
    for(int i = 0;i<ll->ListLength();++i)
    {
        ll->GetElem(i,&t);
        //qDebug("%s,%d\n",t->c,t->GetIndegreeBack());
        qDebug("%d\n",t->GetIndegreeBack());
        if(!t->GetIndegreeBack())
            stk->Push(t);
    }

    //DFS搜索关键路径
    for(int i = 0;i<stk->StackLength();++i)
    {
        ll->GetElem(i,&t);
        VisitNode("",t);
    }

    return route;
}
void RenderArea::VisitNode(string str,LinkList<edge> *Node)
{
    qDebug("%s\n",str.c_str() );
    //DFS访问结点函数
    if(Node->vl == Node->ve)
        str = str + " " + iToString(Node->n);
    else
        return;


    //到达中终点，加入路径
    if(Node->ListLength() == 0)
    {
        route = route + str + " 或 ";
        return;
    }

    int position;
    for(int i = 0;i<Node->ListLength();++i)
    {
        if(Node->GetElem(i,&ed))
        {
            position = ed.dest;
            ll->GetElem(position,&t2);
            VisitNode(str,t2);
        }

    }

}
