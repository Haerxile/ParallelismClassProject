#include "../InputH/input.h"
#include "../MatrixH/matrix_demo.h"
#include "../TimerH/timer.h"

using namespace std;

int main() {
  Timer_demo::tick("main", "main");
  cout << "yeys" << endl;
  Timer_demo::tick("main", "main");
  Timer_demo::printRes();
}
