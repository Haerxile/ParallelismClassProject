#include "./MatrixH/matrix_demo.h"

#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 7) {
    cerr << "Invalid number<" << argc
         << "> of arguments when launching: " << argv[0] << ">!" << endl;
    return -1;
  }
  const int nrow = atoi(argv[1]);
  const int ncol = atoi(argv[2]);
  const int downLim = atoi(argv[3]);
  const int upLim = atoi(argv[4]);
  const string filename = argv[5];
  const string matrixType = argv[6];

  random_device seed;
  mt19937_64 branch(seed());
  uniform_real_distribution<double> fruit(downLim, upLim);

  Matrix_demo res(nrow, ncol);
  res.genRandomMatrix(nrow, ncol, downLim, upLim, matrixType);
  res.writeToFile(filename);
  return 0;
}
