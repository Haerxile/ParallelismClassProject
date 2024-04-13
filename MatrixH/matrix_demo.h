#ifndef MATRIX_DEMO_H
#define MATRIX_DEMO_H

#include "../InputH/input.h"

#include <cblas-openblas.h>
#include <fstream>
#include <iostream>
#include <lapacke.h>
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
  void init(Input& inputConfig);
  void genRandomMatrix(const int &m, const int &n, const double &downLim,
                       const double &upLim, const std::string &matrixType);
  std::ios::pos_type readFromFile(const std::string &filename,
                                  const std::ios::pos_type &targetPos);
  bool writeToFile(const std::string &filename);
  double maxElem() const;
  double minElem() const;
  double *matrixToArray(CBLAS_LAYOUT layout, double *arrayPtr) const;
  void ArrayToMatrix(double *ptr, int m, int n,
                     CBLAS_LAYOUT layout = CblasRowMajor);
  void EigenToMatrix(double *ptr, int n);

  Matrix_demo &operator=(const Matrix_demo &matrix);
  Matrix_demo operator*(const Matrix_demo &matrix) const;
  Matrix_demo operator*(const double &figure) const;
  Matrix_demo operator+(const Matrix_demo &matrix) const;
  Matrix_demo operator-(const Matrix_demo &matrix) const;

  static void CBLAS_Mult(const Matrix_demo &A, const Matrix_demo &B,
                         Matrix_demo &C, CBLAS_LAYOUT layout = CblasRowMajor);
  // A * B = C they are all Matrix_demo type
  // layout equals that in CBLAS as CBLAS_LAYOUT
  // typedef enum CBLAS_ORDER     {CblasRowMajor=101, CblasColMajor=102}
  // CBLAS_ORDER; typedef CBLAS_ORDER CBLAS_LAYOUT;

  void LAPACK_Dsyev(Matrix_demo &eigenValue, Matrix_demo &eigenVector,
                    int layout = LAPACK_ROW_MAJOR);
  // origin matrix is the function caller
  // eigenValue is a Matrix_demo type, receive output of eigenValue as a n*n
  // matrix eigenVector is a Matrix_demo type, receive output of eigenVector the
  // function caller can be the output receiver at the same time layout is a int
  // type implying the order #define LAPACK_ROW_MAJOR               101 #define
  // LAPACK_COL_MAJOR               102

  void print() const;

private:
  int nrow_;
  int ncol_;
  std::vector<std::vector<double>> data_;
};

#endif
