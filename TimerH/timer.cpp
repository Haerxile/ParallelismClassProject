#include "timer.h"

#include <chrono>
#include <iostream>
#include <map>
#include <stdexcept>
#include <utility>
#include <fstream>
#include <sstream>

#include "../TabulateH/tabulate.hpp"

using namespace std;
using namespace tabulate;

double Timer_demo::TIME_ALL;
map<string, map<string, Timer_demo *>> Timer_demo::goToData;

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
    map<string, Timer_demo *> &tempMap = goToData.at(className);
    try {
      timerPtr = tempMap.at(funcName);
      is_it_exist = true;
    } catch (const out_of_range &e) {
      timerPtr = new Timer_demo;
      tempMap.insert(make_pair(funcName, timerPtr));
    }
  } catch (const out_of_range &e) {
    timerPtr = new Timer_demo;
    map<string, Timer_demo *> tempMap;
    tempMap.insert(make_pair(funcName, timerPtr));
    Timer_demo::goToData.insert(make_pair(className, tempMap));
  }
  if (is_it_exist) {
    if (timerPtr->stat) {
      timerPtr->cnt++;
      #ifdef __MPI__
      timerPtr->endTime = MPI_Wtime();
      #else
      timerPtr->endTime = chrono::steady_clock::now();
      #endif
      timerPtr->singleTime = timerPtr->endTime - timerPtr->startTime;
      #ifdef __MPI__
      timerPtr->addedTime += timerPtr->singleTime;
      #else
      timerPtr->addedTime += timerPtr->singleTime.count();
      #endif
      timerPtr->stat = false;
      #ifdef __MPI__
      Timer_demo::TIME_ALL += timerPtr->singleTime;
      #else
      Timer_demo::TIME_ALL += timerPtr->singleTime.count();
      #endif
    } else {
      timerPtr->stat = true;
      #ifdef __MPI__
      timerPtr->startTime = MPI_Wtime();
      #else
      timerPtr->startTime = chrono::steady_clock::now();
      #endif
    }
  } else {
    if (timerPtr->stat == true) {
      cerr << "Timer already working but just created!" << endl;
    } else {
      timerPtr->stat = true;
      #ifdef __MPI__
      timerPtr->startTime = MPI_Wtime();
      #else
      timerPtr->startTime = chrono::steady_clock::now();
      #endif
    }
  }
}
#ifdef __MPI__
void Timer_demo::printRes(int worldRank) {
#else
void Timer_demo::printRes() {
#endif
  Table output;
  #ifdef __MPI__
  output.add_row(
      RowStream{} << "WORLD_RANK" << worldRank << "" << ""
                  << "" << "");
  #endif
  output.add_row(
      {"CLASS_NAME", "FUNC_NAME", "TIME(sec)", "CALLS", "AVRG_TIME", "PER%"});
  for (auto &classPair : Timer_demo::goToData) {
    double classTotalTime = 0;
    double classPerc = 0;
    for (auto &funcPair : classPair.second) {
      Timer_demo *tempPtr = funcPair.second;
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

#ifdef __MPI__
  void Timer_demo::writeResToFile(int worldRank, int flag) {
  #else
  void Timer_demo::writeResToFile() {
  #endif
    const string baseFilename = "TimerOutput";
    string filename;
    #ifdef __MPI__
    if(flag) {
      filename = baseFilename + to_string(worldRank) + ".dat";
    } else {
      filename = baseFilename + ".dat";
    }
    #else
    filename = baseFilename + ".dat";
    #endif

    const string outputDir = "./resOutput";

    Table output;

    stringstream ss;
    ss << "test -d " << outputDir << " || mkdir -p " << outputDir;
    if(system(ss.str().c_str())) {
      cerr << "Cannot create directory <" << outputDir << ">!" << endl;
      exit(EXIT_FAILURE);
    }

    const string fileDir = outputDir + "/" + filename;
    ofstream fout(fileDir, ios::app);
    if(!fout.is_open()) {
      cerr << "Cannot open file <" << filename << ">!" << endl;
      return;
    }

    #ifdef __MPI__
    output.add_row(
        RowStream{} << "WORLD_RANK" << worldRank << "" << ""
                    << "" << "");
    #endif
    output.add_row(
        {"CLASS_NAME", "FUNC_NAME", "TIME(sec)", "CALLS", "AVRG_TIME", "PER%"});
    for (auto &classPair : Timer_demo::goToData) {
      double classTotalTime = 0;
      double classPerc = 0;
      for (auto &funcPair : classPair.second) {
        Timer_demo *tempPtr = funcPair.second;
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
    fout << output << endl;
  }

void Timer_demo::clearRes() {
  for (auto &classPair : Timer_demo::goToData) {
    for (auto &funcPair : classPair.second) {
      delete funcPair.second;
      classPair.second.erase(funcPair.first);
    }
    Timer_demo::goToData.erase(classPair.first);
  }
  cout << "Cleared!" << endl;
}
