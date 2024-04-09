#include "../InputH/input.h"
#include "../MatrixH/matrix_demo.h"
#include "../TimerH/timer.h"
#include <cblas-openblas.h>
#include <cstdlib>
#include <fstream>
#include <lapacke.h>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "Wrong number <" << argc << "> of arguments passed into <"
         << argv[0] << ">!" << endl;
    exit(EXIT_FAILURE);
  }
  const string inputFilename = argv[1];
  const string outputFilename = argv[2];

  Matrix_demo matrix;
  Matrix_demo eigenVector;

  Timer_demo::tick("Matrix_demo", "readFromFile");
  matrix.readFromFile(inputFilename, ios::beg);
  Timer_demo::tick("Matrix_demo", "readFromFile");

  Timer_demo::tick("Matrix_demo", "LAPACK_Dsyev");
  matrix.LAPACK_Dsyev(matrix, eigenVector, LAPACK_ROW_MAJOR);
  Timer_demo::tick("Matrix_demo", "LAPACK_Dsyev");

  Timer_demo::tick("Matrix_demo", "writeToFile");
  matrix.writeToFile(outputFilename);
  Timer_demo::tick("Matrix_demo", "writeToFile");

  Timer_demo::tick("Matrix_demo", "writeToFile");
  eigenVector.writeToFile(outputFilename);
  Timer_demo::tick("Matrix_demo", "writeToFile");

  Timer_demo::printRes();
  return 0;
}
