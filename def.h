#ifndef DEF
#define DEF

enum Status{MY_OVERFLOW = -1,ERROR,OK};

#include <iostream>

template <typename elem_type>
//链表结点
struct Node
{
    Node *next;
    elem_type elem;
};
//用来表示边的结构
struct edge
{
    int dest;
    int dur;
    int e;
    int l;
};


#endif
