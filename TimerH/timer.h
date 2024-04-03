#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <map>
#include <stack>
#include <string>
#include <vector>

class Timer_demo {
public:
  Timer_demo();
  ~Timer_demo();
  void reset();

  static void printRes();
  static void clearRes();
  static void tick(const std::string &className, const std::string &funcName);

private:
  bool stat;
  long int cnt;
  static double TIME_ALL;
  std::chrono::steady_clock::time_point startTime;
  std::chrono::steady_clock::time_point endTime;
  std::chrono::duration<double> singleTime;
  double addedTime;
  double avrgTime;
  double perc;
  static std::map<std::string, std::map<std::string, Timer_demo *>> goToData;
};

#endif
