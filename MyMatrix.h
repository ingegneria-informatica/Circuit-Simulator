//
// Created by cipher on 01/07/19.
//

#ifndef FIRSTSIMULATORTEST_IMATRIX_H
#define FIRSTSIMULATORTEST_IMATRIX_H

#include <Eigen/Dense>

using namespace Eigen;

typedef Matrix<int,Dynamic,Dynamic> DynamicMatrix;

class MyMatrix: public DynamicMatrix {

public:
    MyMatrix();
    void addRow();
    void ROWinsertBetweenNthAndNthplusOne(int x);
    void COLinsertBetweenNthAndNthplusOne(int x);
    void addCol();
    void addCol(int x);
    int end();
    int bottom();
    void removeColumn(int index);
    void removeRow(int index);
};


#endif //FIRSTSIMULATORTEST_IMATRIX_H
