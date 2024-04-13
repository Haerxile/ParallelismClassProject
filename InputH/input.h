#ifndef INPUT_H
#define INPUT_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>

typedef std::pair<std::string, std::string> SetItem;
typedef std::map<std::string, std::string> Configs;

class Input {
public:
  Input();
  Input(const std::string &filename);
  ~Input();

  void init(const std::string &filename);
  void set_maxlen(int target_len);

  void readData(std::fstream& fin);

  Configs targetConfigs_;

  void printConfigs();

private:
  std::string filename_;
  Configs readInConfigs_;
  std::ios::pos_type dataPos_;
  int MAXLEN_;

  bool readConfigsFromFile();
  bool putReadToConfigs();
};

#endif
