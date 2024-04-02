#include "bar.h"
#include <iostream>
using namespace std;
int main() {
  cout << "MakefileTest.main:" << endl << "Hello!" << endl;
  Print_Progress_Bar(10.0f / 100.0f);
  return 0;
}
