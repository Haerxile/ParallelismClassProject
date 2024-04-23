#include "../InputH/input.h"
#include "../MatrixH/matrix_demo.h"

#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 5) {
    cerr << "Invalid number<" << argc
         << "> of arguments when launching: " << argv[0] << ">!" << endl;
    return -1;
  }
  const int downLim = atoi(argv[1]);
  const int upLim = atoi(argv[2]);
  const string filename = argv[3];
  const string matrixType = argv[4];

  Input randomInput(filename);
  randomInput.printConfigs();
  int nrow = stoi(randomInput.targetConfigs_.at("nrows"));
  int ncol = stoi(randomInput.targetConfigs_.at("ncols"));

  random_device seed;
  mt19937_64 branch(seed());
  uniform_real_distribution<double> fruit(downLim, upLim);

  Matrix_demo res(nrow, ncol);
  res.genRandomMatrix(nrow, ncol, downLim, upLim, matrixType);
  res.writeToFile(filename);
  return 0;
}
