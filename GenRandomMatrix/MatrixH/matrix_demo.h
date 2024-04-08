#ifndef MATRIX_DEMO_H
#define MATRIX_DEMO_H

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
                       const double &upLim, const std::string &matrixType);
  bool readFromFile(const std::string &filename);
  bool writeToFile(const std::string &filename);
  double maxElem();
  double minElem();

  Matrix_demo &operator=(const Matrix_demo &matrix);
  Matrix_demo operator*(const Matrix_demo &matrix) const;
  Matrix_demo operator*(const double &figure) const;
  Matrix_demo operator+(const Matrix_demo &matrix) const;
  Matrix_demo operator-(const Matrix_demo &matrix) const;

  void print();

private:
  int nrow_;
  int ncol_;
  std::vector<std::vector<double>> data_;
};

#endif
