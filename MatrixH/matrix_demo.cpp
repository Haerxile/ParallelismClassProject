#include "matrix_demo.h"

#include <algorithm>
#include <fstream>
#include <iomanip>

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
  for (auto sub : data_) {
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
  data_.resize(nrow_, std::vector<double>(ncol_));
  for (auto sub : data_) {
    fill(sub.begin(), sub.end(), initNum);
  }
}

void Matrix_demo::init(const int &n) {
  nrow_ = n;
  ncol_ = n;
  data_.resize(nrow_, std::vector<double>(ncol_));
  for (auto sub : data_) {
    fill(sub.begin(), sub.end(), 0);
  }
  for (int i = 0; i < n; ++i) {
    data_[i][i] = 1;
  }
}

bool Matrix_demo::readFromFile(const string &filename) {
  ifstream fileIn(filename);
  if (!fileIn.is_open()) {
    cerr << "Failed to open <" << filename << ">!" << endl;
    return false;
  }
  fileIn >> nrow_ >> ncol_;
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
    this->data_.resize(nrow_, vector<double>(ncol_));
    for (int i = 0; i < nrow_; ++i) {
      for (int j = 0; j < ncol_; ++j) {
        this->data_[i][j] = matrix.data_[i][j];
      }
    }
  }
  return *this;
}

Matrix_demo &Matrix_demo::operator*(const Matrix_demo &matrix) {
  if (this->ncol_ != matrix.nrow_) {
    cerr << "ncol and nrow not matched when multipling!" << endl;
    return *this;
  }
  Matrix_demo tempMatrix(this->nrow_, matrix.ncol_, 0);
  for (int i = 0; i < this->nrow_; ++i) {
    for (int k = 0; k < this->ncol_; ++k) {
      double s = this->data_[i][k];
      for (int j = 0; j < matrix.ncol_; ++j) {
        tempMatrix.data_[i][j] += s * matrix.data_[k][j];
      }
    }
  }
  *this = tempMatrix;
  return *this;
}

Matrix_demo &Matrix_demo::operator*(const double &figure) {
  for (int i = 0; i < this->nrow_; ++i) {
    for (int j = 0; j < this->ncol_; ++j) {
      this->data_[i][j] *= figure;
    }
  }
  return *this;
}

Matrix_demo &Matrix_demo::operator+(const Matrix_demo &matrix) {
  if (this->nrow_ != matrix.nrow_ || this->ncol_ != matrix.ncol_) {
    cerr << "nrow and ncol unmatched when adding!" << endl;
    return *this;
  }
  for (int i = 0; i < this->nrow_; ++i) {
    for (int j = 0; j < this->ncol_; ++j) {
      this->data_[i][j] += matrix.data_[i][j];
    }
  }
  return *this;
}

Matrix_demo &Matrix_demo::operator-(const Matrix_demo &matrix) {
  if (this->nrow_ != matrix.nrow_ || this->ncol_ != matrix.ncol_) {
    cerr << "nrow and ncol unmatched when adding!" << endl;
    return *this;
  }
  for (int i = 0; i < this->nrow_; ++i) {
    for (int j = 0; j < this->ncol_; ++j) {
      this->data_[i][j] -= matrix.data_[i][j];
    }
  }
  return *this;
}

void Matrix_demo::print() {
  for (int i = 0; i < nrow_; ++i) {
    for (int j = 0; j < ncol_; ++j) {
      cout << setw(12) << data_[i][j];
    }
    cout << endl;
  }
}
