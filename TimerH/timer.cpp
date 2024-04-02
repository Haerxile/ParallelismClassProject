#include "./timer.h"
#include "tabulate.h"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <utility>

using namespace std;
using namespace tabulate;

Timer_demo::Timer_demo() {
  stat = false;
  cnt = 0;
  addedTime = 0;
  perc = 0;
}
Timer_demo::~Timer_demo() {}
void Timer_demo::reset() {
  stat = false;
  cnt = 0;
  addedTime = 0;
  perc = 0;
}

void Timer_demo::tick(const std::string &className,
                      const std::string &funcName) {
  Timer_demo *timerPtr = nullptr;
  bool is_it_exist = false;
  try {
    map<string, Timer_demo> &tempMap = goToData.at(className);
    try {
      timerPtr = &tempMap.at(funcName);
      is_it_exist = true;
    } catch (const out_of_range &e) {
      timerPtr = &tempMap[funcName];
    }
  } catch (const out_of_range &e) {
    timerPtr = &goToData[className][funcName];
  }
  if (is_it_exist) {
    if (timerPtr->stat) {
      timerPtr->cnt++;
      timerPtr->endTime = chrono::steady_clock::now();
      timerPtr->singleTime = timerPtr->endTime - timerPtr->startTime;
      timerPtr->addedTime += timerPtr->singleTime.count();
      timerPtr->stat = false;
      Timer_demo::TIME_ALL += timerPtr->singleTime.count();
    } else {
      timerPtr->stat = true;
      timerPtr->startTime = chrono::steady_clock::now();
    }
  } else {
    if (timerPtr->stat == true) {
      cerr << "Timer already working but just created!" << endl;
    } else {
      timerPtr->stat = true;
      timerPtr->startTime = chrono::steady_clock::now();
    }
  }
}

void Timer_demo::printRes() {
  Table output;
  output.add_row(
      {"CLASS_NAME", "FUNC_NAME", "TIME(sec)", "CALLS", "AVRG_TIME", "PER%"});
  for (auto &classPair : Timer_demo::goToData) {
    double classTotalTime = 0;
    double classPerc = 0;
    for (auto &funcPair : classPair.second) {
      Timer_demo *tempPtr = &funcPair.second;
      tempPtr->avrgTime = tempPtr->addedTime / tempPtr->cnt;
      tempPtr->perc = 100 * tempPtr->addedTime / Timer_demo::TIME_ALL;
      output.add_row(RowStream{} << classPair.first << funcPair.first
                                 << tempPtr->addedTime << tempPtr->cnt
                                 << tempPtr->avrgTime << tempPtr->perc);
      classTotalTime += tempPtr->addedTime;
    }
    classPerc = 100 * classTotalTime / Timer_demo::TIME_ALL;
    output.add_row(RowStream{} << classPair.first << "**total**"
                               << classTotalTime << " "
                               << " " << classPerc);
  }
  output.add_row(RowStream{} << "**total**"
                             << " " << Timer_demo::TIME_ALL << " "
                             << " "
                             << "100");
  cout << output << endl;
}
