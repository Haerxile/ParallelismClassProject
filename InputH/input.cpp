#include "input.h"
#include <algorithm>
#include <iostream>

#define defaultTarget {{"calculation", "Default"}, \
                    {"matrix_type", "Default"}, \
                    {"matrix_print", "Default"}, \
                    {"nrows", "Default"}, \
                    {"ncols", "Default"}, \
                    {"print_mpi_log", "Default"}}

using namespace std;

Input::Input() {
  targetConfigs_ = defaultTarget;
  filename_ = "emptyInput";
  readInConfigs_ = {};
  MAXLEN_ = 256;
}

Input::Input(const string &filename) {
  targetConfigs_ = defaultTarget;
  filename_ = "emptyInput";
  readInConfigs_ = {};
  MAXLEN_ = 256;
  filename_ = filename;
  readConfigsFromFile();
  putReadToConfigs();
}

Input::~Input() {
  targetConfigs_.clear();
  readInConfigs_.clear();
  filename_.clear();
  MAXLEN_ = 0;
}

void Input::init(const std::string &filename) {
  filename_ = filename;
  readConfigsFromFile();
  putReadToConfigs();
}

void Input::set_maxlen(int target_len) {
  MAXLEN_ = target_len > MAXLEN_ ? target_len : MAXLEN_;
}

void Input::printConfigs() {
  for (auto sub : targetConfigs_) {
    cout << sub.first << ": " << sub.second << endl;
  }
}

bool Input::readConfigsFromFile() {
  ifstream fin;
  fin.open(filename_);
  if (!fin.is_open()) {
    cerr << "Cannot open file <" << filename_ << ">!" << endl;
    return false;
  } // 文件是否正常打开

  int cntLine = 0;

  // 逐行读取
  while (!fin.eof()) {
    cntLine++;
    SetItem tempSI;
    tempSI.first.resize(MAXLEN_);
    tempSI.second.resize(MAXLEN_);
    char tempChar;
    int flag = 0;

    char auxIfNum;
    fin >> auxIfNum;
    fin.putback(auxIfNum);
    if(auxIfNum > '0' && auxIfNum < '9') {
      dataPos_ = fin.tellg();
      std::cout << "Data found at line <" << cntLine << ">\n";
      break;
    }
    // 逐字读取
    do {
      fin >> tempChar;
      // fin.get(&tempChar, MAXLEN, '\n');
      if (tempChar == ' ' || tempChar == '\t' ) {
        // do nothing
      } else if (tempChar == '\n' || tempChar == EOF) {
        break;
      } else if (tempChar == '#') {
        fin.ignore(MAXLEN_, '\n');
        tempChar = EOF;
        // fin.putback(EOF);
      } else if (flag == 0) {
        fin.unget();
        fin >> tempSI.first;
        // fin.getline(&tempSI.first[0], MAXLEN, ' ');
        flag++;
      } else if (flag == 1) {
        fin.unget();
        fin >> tempSI.second;
        // fin.unget();
        // fin.getline(&tempSI.second[0], MAXLEN, ' ');
        flag++;
        if(fin.peek() == '\n') {
          break;
        }
      } else if (flag > 1) {
        cerr << "More than 2 arguments at line <" << cntLine << ">!" << endl;
        fin.ignore(MAXLEN_, '\n');
        tempChar = EOF;
      } else {
        cerr << "Unkown error #0001 at line <" << cntLine << ">!" << endl;
        return false;
      }
    } while (tempChar != '\n' && tempChar != EOF);

    // 去掉空项目
    if (fin.eof() || fin.bad() || fin.fail())
      break;
    if (tempSI.first[0] == '\000')
      continue;

    // 写入
    readInConfigs_.emplace(tempSI);
  }
  return true;
}

bool Input::putReadToConfigs() {
  bool state = true;
  for (auto ReadSI : readInConfigs_) {

    // 忽略大小写
    string tempStr;
    tempStr.resize(ReadSI.first.length());
    transform(ReadSI.first.begin(), ReadSI.first.end(), tempStr.begin(),
              ::tolower);

    // 尝试写入配置，若不匹配则输出错误信息
    try {
      targetConfigs_.at(tempStr) = ReadSI.second;
    } catch (std::out_of_range &) {
      cerr << "Invalid name of argument: <" << tempStr << ">" << endl;
      state = false;
    }
  }
  return state;
}

void Input::readData(std::fstream &fin) {
  fin.open(filename_, ios::in);
  if (!fin.is_open()) {
    cerr << "Cannot open file <" << filename_ << ">!" << endl;
  } // 文件是否正常打开
  fin.seekg(dataPos_, ios::beg);
}