#include "../InputH/input.h"
#include "../MatrixH/matrix_demo.h"
#include "../TimerH/timer.h"
#include <cblas-openblas.h>
#include <cstdlib>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "Wrong number <" << argc << "> of arguments passed into <"
         << argv[0] << ">!" << endl;
    exit(EXIT_FAILURE);
  }
  const string inputFilename = argv[1];
  const string outputFilename = argv[2];

  Matrix_demo A;
  Matrix_demo B;

  Timer_demo::tick("Matrix_demo", "readFromFile");
  int lastPos = A.readFromFile(inputFilename, ios::beg);
  Timer_demo::tick("Matrix_demo", "readFromFile");

  Timer_demo::tick("Matrix_demo", "readFromFile");
  B.readFromFile(inputFilename, lastPos);
  Timer_demo::tick("Matrix_demo", "readFromFile");

  Matrix_demo C;
  Timer_demo::tick("Matrix_demo", "operator*");
  C = A * B;
  Timer_demo::tick("Matrix_demo", "operator*");

  Matrix_demo D;
  Timer_demo::tick("Matrix_demo", "CBLAS_Mult");
  Matrix_demo::CBLAS_Mult(A, B, D);
  Timer_demo::tick("Matrix_demo", "CBLAS_Mult");

  Timer_demo::tick("Matrix_demo", "writeToFile");
  A.writeToFile(outputFilename);
  B.writeToFile(outputFilename);
  C.writeToFile(outputFilename);
  Timer_demo::tick("Matrix_demo", "writeToFile");

  Timer_demo::tick("Matrix_demo", "writeToFile");
  D.writeToFile(outputFilename);
  Timer_demo::tick("Matrix_demo", "writeToFile");

  Timer_demo::printRes();

  return 0;
}
