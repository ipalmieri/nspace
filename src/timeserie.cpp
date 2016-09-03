#include "timeserie.h"

using namespace std;
using namespace tools;

template<typename T> void timeserie<T>::addEntry(const date& dt, const T& value)
{
  _serie[dt] = value;
}

template<typename T> T timeserie<T>::getValue(const date& dt)
{
  return _serie[dt];
}


