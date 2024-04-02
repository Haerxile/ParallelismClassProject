#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

int main() {
  chrono::steady_clock::time_point start = chrono::steady_clock::now();
  this_thread::sleep_for(chrono::seconds(2));
  chrono::steady_clock::time_point end = chrono::steady_clock::now();

  chrono::duration<double> duration =
      chrono::duration_cast<chrono::duration<double>>(end - start);

  cout << "time duration: " << duration.count() << " seconds" << endl;

  int cnt = 0;
  cout << "how many times?:" << endl;
  cin >> cnt;
  for (int i = 0; i < cnt; ++i) {
    cout << "the " << i << "th time." << endl;
    this_thread::sleep_for(chrono::seconds(1));
  }

  vector<int> nums = {5, 2, 8, 1, 9};

  chrono::time_point<chrono::steady_clock> start1 = chrono::steady_clock::now();
  sort(nums.begin(), nums.end());
  chrono::time_point<chrono::steady_clock> end1 = chrono::steady_clock::now();

  chrono::duration<double> diff = end1 - start1;
  cout << "SteadyClock: " << diff.count() << "seconds." << endl;

  chrono::time_point<chrono::high_resolution_clock> start2 =
      chrono::high_resolution_clock::now();
  sort(nums.begin(), nums.end());
  chrono::time_point<chrono::high_resolution_clock> end2 =
      chrono::high_resolution_clock::now();

  chrono::duration<double> diff2 = end2 - start2;
  cout << "HighResolutionClock: " << diff2.count() << "seconds." << endl;

  return 0;
}
