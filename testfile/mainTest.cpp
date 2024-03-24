#include "../InputH/input.h"
#include "../MatrixH/matrix_demo.h"

using namespace std;

int main() {
  Matrix_demo A;
  A.init(5);
  Matrix_demo B(5, 6, 1.0);
  Matrix_demo B1(5, 5, 2.0);
  Matrix_demo C = A + A;
  Matrix_demo D = (C + B1) * B;
  Matrix_demo E = A * 5;
  double maxD = D.maxElem();
  double minD = D.minElem();
  cout << "Matrix A" << endl;
  A.print();
  cout << "Matrix B" << endl;
  B.print();
  cout << "Matrix B1" << endl;
  B1.print();
  cout << "Matrix C" << endl;
  C.print();
  cout << "Matrix D" << endl;
  D.print();
  cout << "Matrix E" << endl;
  E.print();
  cout << "maxD: " << maxD << endl << "minD: " << minD << endl;

  Input Inn("./InputFile.dat");
  cout << "Configs: " << endl;
  Inn.printConfigs();
}
