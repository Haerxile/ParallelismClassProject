#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

const int MAXLEN = 2048; // 每行的最大字符数

using namespace std;

// 将每行的参数和值配对成 pair，定义为 SetItem
// 将所有的 SetItem 放入 map，定义为 Configs
typedef pair<string, string> SetItem;
typedef map<string, string> Configs;

// 函数声明
bool readConfigsFromFile(const string &filename, Configs &readInConf);
bool putReadToConfigs(const Configs &ReadInfo, Configs &configsDef);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Invalid number<" << argc << "> of arguments when lanching: <"
         << argv[0] << ">!" << endl;
    return -1;
  } // 命令行传参是否正常
  string filename = argv[1];

  // 配置默认设置项
  Configs targetConfigs;
  targetConfigs["calculation"] = "Default";
  targetConfigs["matrix_type"] = "Default";
  targetConfigs["matrix_print"] = "Default";
  targetConfigs["nrows"] = "Default";
  targetConfigs["ncols"] = "Default";

  // 将文件内容读入 readInConfigs
  // 将 readInConfigs 写入配置项 targetConfigs
  Configs readInConfigs;
  readConfigsFromFile(filename, readInConfigs);
  putReadToConfigs(readInConfigs, targetConfigs);

  // 输出配置
  for (auto targetSi : targetConfigs) {
    cout << targetSi.first << ": " << targetSi.second << endl;
  }

  return 0;
}

// 从文件中读入
bool readConfigsFromFile(const string &filename, Configs &readInConf) {
  ifstream fin;
  fin.open(filename);
  if (!fin.is_open()) {
    cerr << "Cannot open file <" << filename << ">!" << endl;
    return false;
  } // 文件是否正常打开

  int cntLine = 0;

  // 逐行读取
  while (!fin.eof()) {
    cntLine++;
    SetItem tempSI;
    tempSI.first.resize(MAXLEN);
    tempSI.second.resize(MAXLEN);
    char tempChar;
    int flag = 0;

    // 逐字读取
    do {
      fin >> tempChar;
      // fin.get(&tempChar, MAXLEN, '\n');
      if (tempChar == ' ' || tempChar == '\t' || tempChar == EOF) {
        // do nothing
      } else if (tempChar == '#') {
        fin.ignore(MAXLEN, '\n');
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
        // fin.getline(&tempSI.second[0], MAXLEN, ' ');
        flag++;
      } else if (flag > 1) {
        cerr << "More than 2 arguments at line <" << cntLine << ">!" << endl;
        return false;
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
    readInConf.emplace(tempSI);
  }
  return true;
}

// 将读入值与目标配置匹配
bool putReadToConfigs(const Configs &ReadInfo, Configs &configsDef) {
  bool state = true;
  for (auto ReadSI : ReadInfo) {

    // 忽略大小写
    string tempStr;
    tempStr.resize(ReadSI.first.length());
    transform(ReadSI.first.begin(), ReadSI.first.end(), tempStr.begin(),
              ::tolower);

    // 尝试写入配置，若不匹配则输出错误信息
    try {
      configsDef.at(tempStr) = ReadSI.second;
    } catch (std::out_of_range &) {
      cerr << "Invalid name of argument: <" << tempStr << ">" << endl;
      state = false;
    }
  }
  return state;
}
