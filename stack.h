#pragma once
#include <iostream>
#include <conio.h>
using namespace std;

#include "def.h"



template <typename elem_type>
class LinkedStack
{
private:
    Node<elem_type>* base;
    Node<elem_type>* top;
public:
    LinkedStack();//栈的初始化

    ~LinkedStack();

    int StackLength();//返回栈长度
    int StackEmpty();//返回栈是否为空

    Status Push(elem_type target);//在栈顶加入元素
    Status Push(elem_type *target);//在栈顶加入元素

    //Status Pop(elem_type &target);//在栈顶删除元素
    Status Pop(elem_type *target);//在栈顶删除元素

    Status GetTop(elem_type &e);//获取栈顶元素,放入e中

    Status StackTraverse();//遍历输出栈

    Status ClearStack();//清空栈


};
template <typename elem_type>
void visit_elem(elem_type &e)
{
    cout << e->c<<" ";
}

template<typename elem_type>
LinkedStack<elem_type>::LinkedStack()
{
    base = new Node<elem_type>;
    base->next = NULL;
    if (!base)
        exit(MY_OVERFLOW);
    base->next = NULL;
    base = top;
}

template<typename elem_type>
LinkedStack<elem_type>::~LinkedStack()
{
    cout<<"栈析构调用"<<endl;
    Node<elem_type>* p;
    while (base != top)
    {
        p=top;
        top = top->next;
        if(p)
            delete p;
    }

    //delete base;
    //单独使用不会出错
    //放在代码中出错
}

template<typename elem_type>
int LinkedStack<elem_type>::StackLength()
{
    int length = 0;
    Node<elem_type>* p = top;
    while (p != base)
    {
        p = p->next;
        length++;
    }
    return length;
}

template<typename elem_type>
int LinkedStack<elem_type>::StackEmpty()
{
    return top == base;
}

template<typename elem_type>
Status LinkedStack<elem_type>::Push(elem_type target)
{
    Node<elem_type>* p = new Node<elem_type>;
    if (!p)
        return ERROR;
    p->elem = target;
    p->next = top;
    top = p;
    return OK;
}

template<typename elem_type>
Status LinkedStack<elem_type>::Push(elem_type *target)
{
    Node<elem_type>* p = new Node<elem_type>;
    if (!p)
        return ERROR;
    p->elem = *target;
    p->next = top;
    top = p;
    return OK;
}

template<typename elem_type>
Status LinkedStack<elem_type>::Pop(elem_type * target)
{
    if (top == base)
        return ERROR;
    *target = top->elem;
    Node<elem_type>* p = top;
    top = top->next;
    delete p;
    return OK;

}
template<typename elem_type>
Status LinkedStack<elem_type>::GetTop(elem_type & e)
{
    if (top == base)
        return ERROR;
    e = top->elem;
    return OK;
}

template<typename elem_type>
Status LinkedStack<elem_type>::StackTraverse()
{
    Node<elem_type> *p = top;
    while (p!=base)
    {
        visit_elem(p->elem);
        p = p->next;
    }
    cout << endl;
    return OK;
}

template<typename elem_type>
Status LinkedStack<elem_type>::ClearStack()
{
    Node<elem_type> *p;
    while (base != top)
    {
        p = top;
        top = top->next;
        delete p;
    }
    return OK;
}
