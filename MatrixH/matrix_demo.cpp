#include "matrix_demo.h"

#include <algorithm>
#include <cblas-openblas.h>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iomanip>
#include <ios>
#include <random>

using namespace std;

Matrix_demo::Matrix_demo() {
  nrow_ = 0;
  ncol_ = 0;
  data_.resize(nrow_, std::vector<double>(ncol_));
}

Matrix_demo::Matrix_demo(const int &nrow, const int &ncol,
                         const double &initNum) {
  nrow_ = nrow;
  ncol_ = ncol;
  data_.resize(nrow_, std::vector<double>(ncol_));
  for (auto &sub : data_) {
    fill(sub.begin(), sub.end(), initNum);
  }
}

Matrix_demo::~Matrix_demo() {
  nrow_ = 0;
  ncol_ = 0;
  vector<vector<double>>().swap(data_);
}

int Matrix_demo::rowNum() const { return nrow_; }

int Matrix_demo::colNum() const { return ncol_; }

void Matrix_demo::init(const int &nrow, const int &ncol,
                       const double &initNum) {
  nrow_ = nrow;
  ncol_ = ncol;
  data_.clear();
  data_.resize(nrow_, std::vector<double>(ncol_));
  for (auto &sub : data_) {
    fill(sub.begin(), sub.end(), initNum);
  }
}

void Matrix_demo::init(const int &n) {
  nrow_ = n;
  ncol_ = n;
  data_.clear();
  data_.resize(nrow_, std::vector<double>(ncol_));
  for (auto &sub : data_) {
    fill(sub.begin(), sub.end(), 0);
  }
  for (int i = 0; i < n; ++i) {
    data_[i][i] = 1;
  }
}

void Matrix_demo::genRandomMatrix(const int &m, const int &n,
                                  const double &downLim, const double &upLim) {
  nrow_ = m;
  ncol_ = n;
  data_.clear();
  data_.resize(m, vector<double>(n));

  random_device seed;
  mt19937_64 branch(seed());
  uniform_real_distribution<double> fruit(downLim, upLim);
  for (int i = 0; i < nrow_; ++i) {
    for (int j = 0; j < ncol_; ++j) {
      data_[i][j] = fruit(branch);
    }
  }
}

ios::pos_type Matrix_demo::readFromFile(const string &filename,
                                        const ios::pos_type &targetPos) {
  ifstream fileIn(filename);
  if (!fileIn.is_open()) {
    cerr << "Failed to open <" << filename << ">!" << endl;
    return false;
  }

  fileIn.seekg(targetPos, ios::beg);
  if (!fileIn.good()) {
    cerr << "failed to seek targetPos in fstream <" << filename << ">!" << endl;
    exit(EXIT_FAILURE);
  }

  fileIn >> nrow_ >> ncol_;
  data_.clear();
  data_.resize(nrow_, std::vector<double>(ncol_));
  for (int i = 0; i < nrow_; ++i) {
    for (int j = 0; j < ncol_; ++j) {
      fileIn >> data_[i][j];
    }
  }
  return fileIn.tellg();
}

bool Matrix_demo::writeToFile(const string &filename) {
  ofstream fileOut(filename, ios_base::app);
  if (!fileOut.is_open()) {
    cerr << "Failed to open <" << filename << ">!" << endl;
    return false;
  }
  fileOut << endl << nrow_ << " " << ncol_ << endl;
  for (int i = 0; i < nrow_; ++i) {
    for (int j = 0; j < ncol_; ++j) {
      fileOut << data_[i][j] << " ";
    }
    fileOut << endl;
  }
  return true;
}

double Matrix_demo::maxElem() {
  double maxElement = data_[0][0];
  for (int i = 0; i < nrow_; i++) {
    for (int j = 0; j < ncol_; ++j) {
      maxElement = data_[i][j] > maxElement ? data_[i][j] : maxElement;
    }
  }
  return maxElement;
}

double Matrix_demo::minElem() {
  double minElement = data_[0][0];
  for (int i = 0; i < nrow_; i++) {
    for (int j = 0; j < ncol_; ++j) {
      minElement = data_[i][j] < minElement ? data_[i][j] : minElement;
    }
  }
  return minElement;
}

/*
typedef enum CBLAS_ORDER     {CblasRowMajor=101, CblasColMajor=102} CBLAS_ORDER;
typedef enum CBLAS_TRANSPOSE {CblasNoTrans=111, CblasTrans=112,
CblasConjTrans=113, CblasConjNoTrans=114} CBLAS_TRANSPOSE; typedef enum
CBLAS_UPLO      {CblasUpper=121, CblasLower=122} CBLAS_UPLO; typedef enum
CBLAS_DIAG      {CblasNonUnit=131, CblasUnit=132} CBLAS_DIAG; typedef enum
CBLAS_SIDE      {CblasLeft=141, CblasRight=142} CBLAS_SIDE; typedef CBLAS_ORDER
CBLAS_LAYOUT;
*/

double *Matrix_demo::matrixToArray(CBLAS_LAYOUT layout) const {
  double *ptr = new double(nrow_ * ncol_);
  cout << "New at <" << ptr << ">." << endl;
  double *tempPtr = ptr;
  if (layout == CblasRowMajor) {
    for (int i = 0; i < nrow_; ++i) {
      for (int j = 0; j < ncol_; ++j) {
        *tempPtr = data_[i][j];
        cout << "Write " << *tempPtr << " at " << tempPtr << endl;
        tempPtr++;
      }
    }
  } else if (layout == CblasColMajor) {
    for (int i = 0; i < ncol_; ++i) {
      for (int j = 0; j < nrow_; ++j) {
        *tempPtr = data_[j][i];
        cout << "Write " << *tempPtr << " at " << tempPtr << endl;
        tempPtr++;
      }
    }
  }
  return ptr;
}

void Matrix_demo::ArrayToMatrix(double *ptr, int m, int n,
                                CBLAS_LAYOUT layout) {
  this->init(m, n, 0.0);
  if (layout == CblasRowMajor) {
    for (int i = 0; i < nrow_; ++i) {
      for (int j = 0; j < ncol_; ++j) {
        cout << "read " << *ptr << " from " << ptr << endl;
        data_[i][j] = *ptr;
        ptr++;
      }
    }
  } else if (layout == CblasColMajor) {
    for (int i = 0; i < ncol_; ++i) {
      for (int j = 0; j < nrow_; ++j) {
        cout << "read " << *ptr << " from " << ptr << endl;
        data_[j][i] = *ptr;
        ptr++;
      }
    }
  }
}

Matrix_demo &Matrix_demo::operator=(const Matrix_demo &matrix) {
  if (&matrix != this) {
    this->nrow_ = matrix.nrow_;
    this->ncol_ = matrix.ncol_;
    this->data_.clear();
    this->data_.resize(this->nrow_, vector<double>(this->ncol_));
    for (int i = 0; i < this->nrow_; ++i) {
      for (int j = 0; j < this->ncol_; ++j) {
        this->data_[i][j] = matrix.data_[i][j];
      }
    }
  }
  return *this;
}

Matrix_demo Matrix_demo::operator*(const Matrix_demo &matrix) const {
  int matrixK;
  if (this->ncol_ != matrix.nrow_) {
    cerr << "ncol and nrow not matched when multipling!" << endl;
    return *this;
  } else {
    matrixK = this->ncol_;
  }
  Matrix_demo tempMatrix(this->nrow_, matrix.ncol_, 0);
  for (int i = 0; i < tempMatrix.nrow_; ++i) {
    for (int k = 0; k < matrixK; ++k) {
      double s = this->data_[i][k];
      for (int j = 0; j < tempMatrix.ncol_; ++j) {
        tempMatrix.data_[i][j] += s * matrix.data_[k][j];
      }
    }
  }
  return tempMatrix;
}

Matrix_demo Matrix_demo::operator*(const double &figure) const {
  Matrix_demo temp;
  temp.init(this->nrow_, this->ncol_, 0);
  for (int i = 0; i < temp.nrow_; ++i) {
    for (int j = 0; j < temp.ncol_; ++j) {
      temp.data_[i][j] = figure * this->data_[i][j];
    }
  }
  return temp;
}

Matrix_demo Matrix_demo::operator+(const Matrix_demo &matrix) const {
  if (this->nrow_ != matrix.nrow_ || this->ncol_ != matrix.ncol_) {
    cerr << "nrow and ncol unmatched when adding!" << endl;
    return *this;
  }
  Matrix_demo temp;
  temp.init(this->nrow_, this->ncol_, 0);
  for (int i = 0; i < temp.nrow_; ++i) {
    for (int j = 0; j < temp.ncol_; ++j) {
      temp.data_[i][j] = this->data_[i][j] + matrix.data_[i][j];
    }
  }
  return temp;
}

Matrix_demo Matrix_demo::operator-(const Matrix_demo &matrix) const {
  if (this->nrow_ != matrix.nrow_ || this->ncol_ != matrix.ncol_) {
    cerr << "nrow and ncol unmatched when minusing!" << endl;
    return *this;
  }
  Matrix_demo temp;
  temp.init(this->nrow_, this->ncol_, 0);
  for (int i = 0; i < temp.nrow_; ++i) {
    for (int j = 0; j < temp.ncol_; ++j) {
      temp.data_[i][j] = this->data_[i][j] - matrix.data_[i][j];
    }
  }
  return temp;
}

void Matrix_demo::CBLAS_Mult(const Matrix_demo &A, const Matrix_demo &B,
                             Matrix_demo &C, CBLAS_LAYOUT layout) {
  int m = A.nrow_;
  int k = 0;
  int k_A = A.ncol_;
  int k_B = B.nrow_;
  int n = B.ncol_;
  if (k_A != k_B) {
    cerr << "Unmached A_ncol and B_nrow when multifying!" << endl;
    exit(EXIT_FAILURE);
  } else {
    k = k_A;
  }
  double *auxPtrA = nullptr;
  double *auxPtrB = nullptr;
  double *auxPtrC = nullptr;
  auxPtrA = A.matrixToArray(layout);
  auxPtrB = B.matrixToArray(layout);
  cblas_dgemm(layout, CblasNoTrans, CblasNoTrans, m, n, k, 1.0, auxPtrA, k_A,
              auxPtrB, n, 0.0, auxPtrC, n);
  C.ArrayToMatrix(auxPtrC, m, n, layout);
}

void Matrix_demo::print() {
  for (int i = 0; i < nrow_; ++i) {
    for (int j = 0; j < ncol_; ++j) {
      cout << setw(12) << data_[i][j];
    }
    cout << endl;
  }
}
