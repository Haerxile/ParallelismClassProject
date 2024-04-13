#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <map>
#include <stack>
#include <string>
#include <vector>

#ifdef __MPI__
#include <mpi.h>
#endif

class Timer_demo {
public:
  Timer_demo();
  ~Timer_demo();
  void reset();

  #ifdef __MPI__
  static void printRes(int worldRank);
  #else
  static void printRes();
  #endif

  #ifdef __MPI__
  static void writeResToFile(int worldRank, int flag);
  #else
  static void writeResToFile();
  #endif

  static void clearRes();
  static void tick(const std::string &className, const std::string &funcName);

private:
  bool stat;
  long int cnt;
  static double TIME_ALL;
  #ifdef __MPI__
  double startTime;
  double endTime;
  double singleTime;
  #else
  std::chrono::steady_clock::time_point startTime;
  std::chrono::steady_clock::time_point endTime;
  std::chrono::duration<double> singleTime;
  #endif
  double addedTime;
  double avrgTime;
  double perc;
  static std::map<std::string, std::map<std::string, Timer_demo *>> goToData;
};

#endif
