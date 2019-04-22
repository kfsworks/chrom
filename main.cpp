#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

class Chromo{
private:

public:
  static inline const size_t CHROMO_LEN {16};
  static inline const size_t GENE_LEN {4};

  string bits;
  float fitness;

  Chromo() : Chromo("", 0) {}
  Chromo(string b, float f) : bits {b}, fitness {f} {}

  inline string to_string() {
    vector<string> a;
    for (size_t no_of_parts {Chromo::CHROMO_LEN / Chromo::GENE_LEN}, i {}; i < no_of_parts; i++) {
      string g { bits.substr(i * Chromo::GENE_LEN, Chromo::GENE_LEN) };
      if (g == "0000") {
        a.push_back("0");
      } else if (g == "0001") {
        a.push_back("1");
      } else if (g == "0010") {
        a.push_back("2");
      } else if (g == "0011") {
        a.push_back("3");
      } else if (g == "0100") {
        a.push_back("4");
      } else if (g == "0101") {
        a.push_back("5");
      } else if (g == "0110") {
        a.push_back("6");
      } else if (g == "0111") {
        a.push_back("7");
      } else if (g == "1000") {
        a.push_back("8");
      } else if (g == "1001") {
        a.push_back("9");
      } else if (g == "1010") {
        a.push_back("+");
      } else if (g == "1011") {
        a.push_back("-");
      } else if (g == "1100") {
        a.push_back("*");
      } else if (g == "1101") {
        a.push_back("/");
      } else {
        a.push_back("na");
      }
    }

    string res {};
    for(auto q : a) {
      res += q + " ";
    }

    return res;
  }

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
    cout << chromos[x].bits << " " << chromos[x].to_string() << endl;
  }

 // float target {23.0f};

  return 0;
}
