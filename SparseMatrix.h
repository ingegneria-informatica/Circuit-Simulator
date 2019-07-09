//
// Created by Sierra on 2019-07-06.
//

#ifndef FIRSTSIMULATORTEST_SPARSEMATRIX_H
#define FIRSTSIMULATORTEST_SPARSEMATRIX_H

#include "MyMatrix.h"
#include "Node.h"

class SparseMatrix {
public:
    SparseMatrix();
    void add(Node* n);
    void add(Component* c,std::list<Node*> v);
    void remove(Node* n,std::list<Node*> v);
    void remove(Component* c,std::list<Component*> v);
    void update(Component* c,std::list<Component*> v,std::list<Node*> n);

private:
    int CompsIndex;
    MyMatrix* m;
    MyMatrix* terms;
};


#endif //FIRSTSIMULATORTEST_SPARSEMATRIX_H
