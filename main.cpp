#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <bitset>

using namespace std;

class Chromo {
public:
  static inline const size_t CHROMO_LEN {40};
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
    cout << "origi: " <<  bit << endl;
    decoded_value = decode_bit();
  }

  inline long get_decoded_value() {
    return decoded_value;
  }

  inline void set_fitness(double f) {
    fitness = f;
    cout << "fitness: " <<  fitness << endl;
  }

  inline double get_fitness() {
    return fitness;
  }

  inline long decode_bit() {
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
          cout << "found operator - " << current << endl;
        }
      } else {
        if (current < 10) {
          cout << "found number - " << current << endl;
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
          cout << "total become - " << total << endl;
        }
      }
    }

    return total;
  }


};


class Nature {
public:
  inline static const size_t POPULATION {1};

  inline static void seed_rand() {
    srand(static_cast<unsigned>(time(nullptr)));
  }

  inline static unsigned random() {
    return static_cast<unsigned>(rand() / ((RAND_MAX + 1.0f)/ 2) );
  }

  inline static string first_born_chromo_bits(size_t length) {
    string s {};
    for (size_t y {}; y < length; ++y) {
      s = s + (Nature::random() ? "1" : "0");
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
};

int main(){
  Nature::seed_rand();

  // assume target is 23
  long target = 23;

  vector<Chromo> chromos;
  for(size_t x {}; x < Nature::POPULATION; x++) {
    Chromo c {Nature::first_born_chromo_bits(Chromo::CHROMO_LEN)};
    c.set_fitness(Nature::cal_fitness(target, c.get_decoded_value()));
    chromos.push_back(c);
    //c.print();
  }


  return 0;
}

/*
 *
#define CROSSOVER_RATE            0.7
#define MUTATION_RATE             0.001
#define POP_SIZE                  100			//must be an even number
#define CHROMO_LENGTH             300
#define MAX_ALLOWABLE_GENERATIONS	400

  while (true)
  {
    //storage for our population of chromosomes.
    chromo_typ Population[POP_SIZE];

	  //get a target number from the user. (no error checking)
	  float Target;
	  cout << "\nInput a target number: ";
	  cin >> Target;
    cout << endl << endl;

	  //first create a random population, all with zero fitness.
	  for (int i=0; i<POP_SIZE; i++)
	  {
		  Population[i].bits	  = GetRandomBits(CHROMO_LENGTH);
		  Population[i].fitness = 0.0f;
	  }

	  int GenerationsRequiredToFindASolution = 0;

	  //we will set this flag if a solution has been found
	  bool bFound = false;

	  //enter the main GA loop
	  while(!bFound) {
		 //this is used during roulette wheel sampling
		 float TotalFitness = 0.0f;

         // test and update the fitness of every chromosome in the population
         for (int i=0; i<POP_SIZE; i++) {
             Population[i].fitness = AssignFitness(Population[i].bits, Target);
			 TotalFitness += Population[i].fitness;
		 }

		  // check to see if we have found any solutions (fitness will be 999)
		  for (i=0; i<POP_SIZE; i++) {
			  if (Population[i].fitness == 999.0f) {
	  cout << "\nSolution found in " << GenerationsRequiredToFindASolution << " generations!" << endl << endl;;
				  PrintChromo(Population[i].bits);
				  bFound = true;
	              break;
			  }
		  }

          // create a new population by selecting two parents at a time and creating offspring
          // by applying crossover and mutation. Do this until the desired number of offspring have been created.

		  //define some temporary storage for the new population we are about to create
		  chromo_typ temp[POP_SIZE];
		  int cPop = 0;

		  //loop until we have created POP_SIZE new chromosomes
		  while (cPop < POP_SIZE) {
			  // we are going to create the new population by grabbing members of the old population
			  // two at a time via roulette wheel selection.
			  string offspring1 = Roulette(TotalFitness, Population);
			  string offspring2 = Roulette(TotalFitness, Population);

              //add crossover dependent on the crossover rate
              Crossover(offspring1, offspring2);

			  //now mutate dependent on the mutation rate
			  Mutate(offspring1);
			  Mutate(offspring2);

              //add these offspring to the new population. (assigning zero as their fitness scores)
              temp[cPop++] = chromo_typ(offspring1, 0.0f);
              temp[cPop++] = chromo_typ(offspring2, 0.0f);
          }//end loop

		  //copy temp population into main population array
		  for (i=0; i<POP_SIZE; i++) {
              Population[i] = temp[i];
          }

          ++GenerationsRequiredToFindASolution;

		  // exit app if no solution found within the maximum allowable number of generations
		  if (GenerationsRequiredToFindASolution > MAX_ALLOWABLE_GENERATIONS) {
			  cout << "No solutions found this run!";
			  bFound = true;
		  }
	  }
  }//end while


void Mutate(string &bits) {
	for (int i=0; i<bits.length(); i++) {
		if (RANDOM_NUM < MUTATION_RATE) {
			if (bits.at(i) == '1') {
				bits.at(i) = '0';
            } else {
				bits.at(i) = '1';
            }
		}
	}
}

//---------------------------------- Crossover ---------------------------------------
//  Dependent on the CROSSOVER_RATE this function selects a random point along the
//  lenghth of the chromosomes and swaps all the  bits after that point.
//------------------------------------------------------------------------------------
void Crossover(string &offspring1, string &offspring2) {
  //dependent on the crossover rate
  if (RANDOM_NUM < CROSSOVER_RATE) {
    //create a random crossover point
    int crossover = (int) (RANDOM_NUM * CHROMO_LENGTH);

    string t1 = offspring1.substr(0, crossover) + offspring2.substr(crossover, CHROMO_LENGTH);
    string t2 = offspring2.substr(0, crossover) + offspring1.substr(crossover, CHROMO_LENGTH);

    offspring1 = t1; offspring2 = t2;
  }
}

//--------------------------------Roulette-------------------------------------------
//	selects a chromosome from the population via roulette wheel selection
//------------------------------------------------------------------------------------
string Roulette(int total_fitness, chromo_typ* Population) {
	//generate a random number between 0 & total fitness count
	float Slice = (float)(RANDOM_NUM * total_fitness);

	//go through the chromosones adding up the fitness so far
	float FitnessSoFar = 0.0f;

	for (int i=0; i<POP_SIZE; i++) {
		FitnessSoFar += Population[i].fitness;

		//if the fitness so far > random number return the chromo at this point
		if (FitnessSoFar >= Slice) {
			return Population[i].bits;
        }
	}

	return "";
}
 */
