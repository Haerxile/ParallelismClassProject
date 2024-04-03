#include "matrix_demo.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
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

bool Matrix_demo::readFromFile(const string &filename) {
  ifstream fileIn(filename);
  if (!fileIn.is_open()) {
    cerr << "Failed to open <" << filename << ">!" << endl;
    return false;
  }
  fileIn >> nrow_ >> ncol_;
  data_.clear();
  data_.resize(nrow_, std::vector<double>(ncol_));
  for (int i = 0; i < nrow_; ++i) {
    for (int j = 0; j < ncol_; ++j) {
      fileIn >> data_[i][j];
    }
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

void Matrix_demo::print() {
  for (int i = 0; i < nrow_; ++i) {
    for (int j = 0; j < ncol_; ++j) {
      cout << setw(12) << data_[i][j];
    }
    cout << endl;
  }
}
