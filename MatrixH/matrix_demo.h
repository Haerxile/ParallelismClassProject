#ifndef MATRIX_DEMO_H
#define MATRIX_DEMO_H

#include <cblas-openblas.h>
#include <fstream>
#include <iostream>
#include <vector>

class Matrix_demo {
public:
  Matrix_demo();
  Matrix_demo(const int &nrow, const int &ncol, const double &initNum = 0);
  ~Matrix_demo();

  int rowNum() const;
  int colNum() const;
  void init(const int &nrow, const int &ncol, const double &initNum = 0);
  void init(const int &n);
  void genRandomMatrix(const int &m, const int &n, const double &downLim,
                       const double &upLim);
  std::ios::pos_type readFromFile(const std::string &filename,
                                  const std::ios::pos_type &targetPos);
  bool writeToFile(const std::string &filename);
  double maxElem() const;
  double minElem() const;
  double* matrixToArray(CBLAS_LAYOUT layout, double* arrayPtr) const;
  void ArrayToMatrix(double *ptr, int m, int n, CBLAS_LAYOUT layout);

  Matrix_demo &operator=(const Matrix_demo &matrix);
  Matrix_demo operator*(const Matrix_demo &matrix) const;
  Matrix_demo operator*(const double &figure) const;
  Matrix_demo operator+(const Matrix_demo &matrix) const;
  Matrix_demo operator-(const Matrix_demo &matrix) const;

  static void CBLAS_Mult(const Matrix_demo &A, const Matrix_demo &B,
                         Matrix_demo &C, CBLAS_LAYOUT layout = CblasRowMajor);
  void print() const;

private:
  int nrow_;
  int ncol_;
  std::vector<std::vector<double>> data_;
};

#endif
