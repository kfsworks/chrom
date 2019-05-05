#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <bitset>

using namespace std;

class Chromo {
public:
  static inline const size_t CHROMO_LEN {200};
  static inline const size_t GENE_LEN {4};

private:
  bitset<CHROMO_LEN> bit;
  double fitness {};
  long decoded_value {};

  inline void chop_bit(vector<bitset<CHROMO_LEN>>& chopped) {
    for (size_t i {}; i < (CHROMO_LEN / GENE_LEN); i++) {
      size_t remove = CHROMO_LEN - GENE_LEN;
      bitset<CHROMO_LEN> tbit {((bit >> (i * GENE_LEN)) << remove) >> remove};
      chopped.push_back(tbit);
    }
  }

public:
  Chromo(string b) : bit {b} {
    //cout << "origi: " <<  bit << endl;
    decoded_value = decode_bit();
  }

  inline bitset<CHROMO_LEN> get_bit() {
    return bit;
  }

  inline long get_decoded_value() {
    return decoded_value;
  }

  inline void set_fitness(double f) {
    fitness = f;
    //cout << "fitness: " <<  fitness << endl;
  }

  inline double get_fitness() {
    return fitness;
  }

  inline string to_string() {
    return bit.to_string();
  }

  inline void mutate(bitset<CHROMO_LEN>& b) {
    for (size_t i {}; i < b.size(); ++i) {
      double rate = Chromo::random(0.999);
      if (rate < 0.001) {
        b.flip(i);
      }
    }
  }

  inline Chromo mating(shared_ptr<Chromo> c) {
    double rate = Chromo::random(1);
    if (rate < 0.7) {
    long pos = static_cast<long>(Chromo::random(static_cast<double>(Chromo::CHROMO_LEN)));
    bitset<CHROMO_LEN> t1 = (bit >> pos) << pos;
    bitset<CHROMO_LEN> t2 = c->get_bit();
    t2 = (t2 << (CHROMO_LEN - pos)) >> (CHROMO_LEN - pos);
    //cout << "t1 = " << t1 <<endl;
    //cout << "t2 = " << t2 <<endl;
    bitset<CHROMO_LEN> kid_chromo = (t1 | t2);
    //cout << "before m: " << kid_chromo << endl;
    mutate(kid_chromo);
    //cout << "after  m: " << kid_chromo << endl;
    return Chromo(kid_chromo.to_string());
    } else {
    bitset<CHROMO_LEN> kid_chromo = bit;
    return Chromo(kid_chromo.to_string());
    }
  }

  inline static double random(double biased_factor) {
    double biased = (RAND_MAX + 1u) / biased_factor;
    return static_cast<double>(rand() / biased);
  }

  inline long decode_bit(bool print=false) {
    vector<bitset<CHROMO_LEN>> chopped;
    chop_bit(chopped);

    bool isOperRound = false;
    long total {0L};
    size_t operIdx {};
    for (size_t i {}; i < chopped.size(); i++) {
      long current {static_cast<long>(chopped[i].to_ulong())}; // no worry for overflow since design it should be 4 bit value
      if (isOperRound) {
        if (current >=10 && current <=13) {
          operIdx = i;
          isOperRound = false;
          if (print) {
            cout << "found operator - " << current << endl;
          }
        }
      } else {
        if (current < 10) {
          if (print) {
            cout << "found number - " << current << endl;
          }
          if (total <= 0) {
            total = current;
          }
          else {
              switch (chopped[operIdx].to_ulong()) {
                case 10:
                  total += current;
                  break;
                case 11:
                  total -= current;
                  break;
                case 12:
                  total *= current;
                  break;
                case 13:
                  if (current > 0) {
                    total /= current;  // ignore truncation
                  } else {
                    total += 0;
                  }
                  break;
              }
          }
          isOperRound = true;
          //cout << "total become - " << total << endl;
        }
      }
    }

          if (print) {
            cout << "total : " << total<< endl;
          }
    return total;
  }


};


class Nature {
public:
  inline static const size_t POPULATION {100};

  inline static void seed_rand() {
    srand(static_cast<unsigned>(time(nullptr)));
  }

  inline static unsigned random_bit() {
    return static_cast<unsigned>(rand() / ((RAND_MAX + 1.0f)/ 2) );
  }

  inline static int random_int(int biased_factor) {
    return static_cast<int>(rand() / ((RAND_MAX + 1u) / biased_factor));
  }

  inline static double random(double biased_factor) {
    double biased = (RAND_MAX + 1u) / biased_factor;
    return static_cast<double>(rand() / biased);
  }

  inline static double roulette_wheel_pivot() {
    int count {random_int(6)+1};

    double v {1.0};
    for (int c {}; c < count; c++) {
      v = Nature::random(v);
    }
    return v;
  }

  inline static string first_born_chromo_bits(size_t length) {
    string s {};
    for (size_t y {}; y < length; ++y) {
      s = s + (Nature::random_bit() ? "1" : "0");
    }
    return s;
  }

  inline static double cal_fitness(long target, long chromo_value) {
    if (chromo_value == target) {
      return 999.0;
    } else {
      return 1/fabs(target - chromo_value);
    }
  }

  inline static shared_ptr<Chromo> selection(double total_fitness, vector<shared_ptr<Chromo>>& chromos) {
    double selection_point = Nature::random(total_fitness);
    for(size_t x {}; x < Nature::POPULATION; x++) {
      if (chromos[x]->get_fitness() >= selection_point) {
        return chromos[x];
      } else {
        selection_point -= chromos[x]->get_fitness();
      }
    }

    cout << "should not get to here, but if it is, throw exception" << endl;
    throw string("should not get to here, but if it is, throw exception");
  }

};

class NumericalChromo {
  private:
    inline int cal_numerical_result() {
    // f(x) = ((a + 2b + 3c + 4d) - 30)
      return fabs( (ch[0] + (ch[1] * 2) + (ch[2] * 3) + (ch[3] * 4))  - 30 ) ;
    }

  public:
    static inline const int CHROMO_LEN {4};
    vector<int> ch;
    double fitness {0};

    NumericalChromo() {
      for(size_t i {}; i < CHROMO_LEN; i++) {
        ch.push_back(Nature::random_int(30));
      }

      cout << "initial ...";
      print();
      cout << endl;
    }

    inline double cal_fitness() {
      int v = cal_numerical_result();
      fitness = 1.0 / (1 + v);
      return fitness;
    }

    inline void print() {
      for(size_t i {}; i < CHROMO_LEN; i++) {
        cout << ch[i] << " ";
      }
      cout << " : f = " << fitness << endl;
    }
};

int main(){
  Nature::seed_rand();
  size_t pop {6};

  //vector<unique_ptr<NumericalChromo>> chromos; 
  vector<shared_ptr<NumericalChromo>> chromos; 
  for(size_t i {}; i < pop; i++) {
    //chromos.push_back(make_unique<NumericalChromo>());
    chromos.push_back(make_shared<NumericalChromo>());
  }

  bool found {false};
  while (!found) {

    double total_fitness = 0;
    for (size_t i {}; i < pop; i++) {
      total_fitness += chromos[i]->cal_fitness();
    }

    cout << "t: " << total_fitness << endl;
    cout << "p: " << Nature::random(total_fitness) << endl;
    /*
    vector<shared_ptr<NumericalChromo>> tchromos; 
    for (size_t i {}; i < pop; i++) {
      double rwp = Nature::roulette_wheel_pivot();
      for (size_t ii {}; ii < pop; ii++) {
        if (chromos[ii]->fitness > rwp) {
        } else {
          rwp -= chromos[ii]->fitness;
        }
      }
    }*/

    /*
    for(size_t i {}; i < pop; i++) {
      chromos[i]->print();
    }
    cout << "test" << endl;
    auto x = chromos[1];
    chromos[1] = chromos[0];
    chromos[0] = x;
    for(size_t i {}; i < pop; i++) {
      chromos[i]->print();
    }
 */

    found = true;
  }

    /*
  bool found = false;
  long target = 23; // assume target is 23

  vector<shared_ptr<Chromo>> chromos;
  for(size_t x {}; x < Nature::POPULATION; x++) {
    Chromo c {Nature::first_born_chromo_bits(Chromo::CHROMO_LEN)};
    c.set_fitness(Nature::cal_fitness(target, c.get_decoded_value()));
    auto pC { make_shared<Chromo>(c) };
    chromos.push_back(pC);
  }

  long count {};
  while (!found) {
    cout << ++count << endl;
    double total_fitness {};
    for(size_t x {}; x < Nature::POPULATION; x++) {
      total_fitness += chromos[x]->get_fitness();
      if (chromos[x]->get_fitness() > 900) {
        cout << endl;
        cout << "found it: " << chromos[x]->get_bit() << endl;
        chromos[x]->decode_bit(true);
        found = true;
      }
    }

    vector<shared_ptr<Chromo>> tchromos;
    while (tchromos.size() < Nature::POPULATION) {
      shared_ptr<Chromo> c1 = Nature::selection(total_fitness, chromos);
      shared_ptr<Chromo> c2 = Nature::selection(total_fitness, chromos);
      Chromo x = c1->mating(c2);
      tchromos.push_back( make_shared<Chromo>(x) );
    }

    chromos.clear();
    for(size_t x {}; x < Nature::POPULATION; x++) {
      chromos.push_back( tchromos[x] );
    }
  } */


  return 0;
}

