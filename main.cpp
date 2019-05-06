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
  int loop_count {};
  while (!found) {

    double total_fitness = 0;
    for (size_t i {}; i < pop; i++) {
      total_fitness += chromos[i]->cal_fitness();
    }

    vector<shared_ptr<NumericalChromo>> tchromos; 

    for (size_t ii {}; ii < pop; ii++) {
      double rwheel_pivot = Nature::random(total_fitness);
      for (size_t i {}; i < chromos.size(); i++) {
        if (chromos[i] != nullptr) { 
          if (chromos[i]->fitness > rwheel_pivot) {
              total_fitness -= chromos[i]->fitness;
              tchromos.push_back(chromos[i]);
              chromos[i] = nullptr;
              break;
          } else {
            rwheel_pivot -= chromos[i]->fitness;
          }
        }
      }
    }


  /*  cout << "test" << endl;
    for(size_t i {}; i < pop; i++) {
      tchromos[i]->print();
    }
    cout << endl; */

    for (size_t i {}; i < (pop/2); i++) {
      int p1 = Nature::random_int(pop);
      int p2 = Nature::random_int(pop);
      while (p2 == p1) {
        p2 = Nature::random_int(pop);
      }

      int p1a = tchromos[p1]->ch[2];
      int p1b = tchromos[p1]->ch[3];
      tchromos[p1]->ch[2] = tchromos[p2]->ch[0];
      tchromos[p1]->ch[3] = tchromos[p2]->ch[1];
      tchromos[p2]->ch[0] = p1a;
      tchromos[p2]->ch[1] = p1b;
    }

   /* cout << "test" << endl;
    for(size_t i {}; i < pop; i++) {
      tchromos[i]->print();
    }
    cout << endl; */

    int total_gene = pop * NumericalChromo::CHROMO_LEN;
    double mrate = 0.1;
    int num_of_m_gene = static_cast<int>(total_gene * mrate);
    vector<int> pos;
    if (pos.empty()) {
      pos.push_back(Nature::random_int(total_gene));
    }
    while (pos.size() < static_cast<size_t>(num_of_m_gene)) {
      int x = Nature::random_int(total_gene);
      bool exist = false;
      for (auto ele : pos) {
        if (ele == x) {
            exist = true;
        }
      }
      if (!exist) {
        pos.push_back(x);
      }
    }
    for (size_t i {}; i < pos.size(); i++) {
      size_t tch_pos = static_cast<size_t>(pos[i] / 4);
      int gene_pos = pos[i] % 4;
      tchromos[tch_pos]->ch[gene_pos] = Nature::random_int(30);
    }

    /*cout << "test" << endl;
    for(size_t i {}; i < pop; i++) {
      tchromos[i]->print();
    }
    cout << endl; */

    for(size_t i {}; i < pop; i++) {
      chromos[i] = tchromos[i];
    }

    for (size_t i {}; i < pop; i++) {
      if (chromos[i]->cal_fitness() == 1.0) {
        found = true;
        cout << "loop: "<< loop_count << endl;
        chromos[i]->print();
      }
    }

    loop_count++;
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

