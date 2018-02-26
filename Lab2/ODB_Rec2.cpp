#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

class OneDimensionalBalls {
  public:
    long long countValidGuesses(vector <int> firstPicture, vector <int> secondPicture);
    long long cvg(int v, int index);
    vector <int> FP;
    vector <int> SP;
    vector <int> Matched;
    map <int, int> SP_Balls;
};

long long OneDimensionalBalls::cvg(int v, int index)
{
  long long rv;
  int second_val, sb_index;
  map <int, int>::iterator mit;
  int i;

  rv = 0;

  for (i = -1; i <= 1; i += 2) {
    second_val = FP[index]+v*i;
    mit = SP_Balls.find(second_val);
    if (mit != SP_Balls.end() && !(Matched[mit->second])) {
      if (index+1 == FP.size()) {
        rv++;
      } else {
        Matched[mit->second] = 1;
        rv += cvg(v, index+1);
        Matched[mit->second] = 0;
      }
    }
  }
  return rv;
}

long long OneDimensionalBalls::countValidGuesses(
      vector <int> firstPicture, 
      vector <int> secondPicture)
{
  int i, d;
  set <int> velocities;
  set <int>::iterator vit;
  long long rv;

  FP = firstPicture;
  SP = secondPicture;
  Matched.resize(SP.size(), 0);
  for (i = 0; i < SP.size(); i++) SP_Balls[SP[i]] = i;

  for (i = 0; i < SP.size(); i++) {
    d = SP[i] - FP[0];
    if (d < 0) d = -d;
    if (d != 0) velocities.insert(d);
  }

  rv = 0;
  for (vit = velocities.begin(); vit != velocities.end(); vit++) {
    rv += cvg(*vit, 0);
  }
  return rv;
}
