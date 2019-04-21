#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

class Chromo{
public:
  static inline const size_t CHROMO_LEN {16};

  string bits;
  float fitness;

  Chromo() : Chromo("", 0) {}
  Chromo(string b, float f) : bits {b}, fitness {f} {}

};

class Nature {
public:
  inline static void seed_rand() {
    srand(static_cast<unsigned>(time(nullptr)));
  }

  inline static unsigned random() {
    return static_cast<unsigned>(rand() / ((RAND_MAX + 1.0f)/ 2) );
  }

  inline static string first_born_chromo_bits() {
    string s {};
    for (size_t y {}; y < Chromo::CHROMO_LEN; ++y) {
      s = s + (Nature::random() ? "1" : "0");
    }
    return s;
  }
};

int main(){
  Nature::seed_rand();

  vector<Chromo> chromos;
  for(size_t x {}; x < 10; x++) {
    Chromo c {Nature::first_born_chromo_bits(), 0.0f};
    chromos.push_back(c);
    cout << chromos[x].bits << endl;
  }

 // float target {23.0f};

  return 0;
}
