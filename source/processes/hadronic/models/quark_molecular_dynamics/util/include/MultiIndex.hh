#ifndef __MULTIINDEX__
#define __MULTIINDEX__

#include "globals.hh"
#include <iostream>

class MultiIndex
{
  friend std::ostream& operator<<(std::ostream& o,MultiIndex& i);
  int N;
  int* array,*n_min,*n_max;
  bool valid;
  void increase();
public:
  MultiIndex(int n,int n_max);
  MultiIndex(int n,int* n_max);
  MultiIndex(int n,int* n_min,int* n_max);
  MultiIndex(const MultiIndex&);
  ~MultiIndex();
  MultiIndex& operator++() { increase(); return *this; }
  MultiIndex& operator++(int) { increase(); return *this; }
  operator int*() { return array; }
  void reset();
  bool isValid() const { return valid; }
};

#endif
