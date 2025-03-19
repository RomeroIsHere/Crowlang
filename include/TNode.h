#ifndef TNODE_H
#define TNODE_H
#include <vector.h>


template <class T>
struct TNode
{
    std::vector<*tnode> children;
    T data;
    TNode(const T& newData):data(newData);
    ~TNode(); //deletes all of the children nodes
    TNode* addChild(const T& newData);
};


#endif // TNODE_H
