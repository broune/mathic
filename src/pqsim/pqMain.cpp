#include "stdinc.h"

#include "StlSetModel.h"
#include "HeapModel.h"
#include "GeobucketModel.h"
#include "TourTreeModel.h"
#include "Simulator.h"
#include <iostream>
#include <ctime>

namespace {
  size_t toInt(const char* str) {
	std::istringstream in(str);
	size_t i;
	in >> i;
	return i;
  }
}

int main(int argc, const char** args) {
  srand(static_cast<unsigned int>(time(0)));
  srand(0);
  if (argc < 4) {
	std::cerr << "usage: elements span-length target-avg-size\n";
	return 0;
  }
  size_t elements = toInt(args[1]);
  size_t spanSize = toInt(args[2]);
  size_t avgOrInitialSize = toInt(args[3]);
  size_t dups = 30;
  if (argc >= 5)
    dups = toInt(args[4]);

  size_t repeats = 500;
  IF_DEBUG(repeats = 2;);

  std::cerr << "Generating simulation..." << std::endl;
  Simulator sim(repeats);
  //sim.orderSpans(elements / spanSize, spanSize, avgOrInitialSize);
  //sim.randomSpans(elements / spanSize, spanSize, avgOrInitialSize);
  sim.dupSpans(elements, spanSize, avgOrInitialSize, dups);
  sim.printEventSummary(std::cerr);
  //sim.printEvents(std::cerr);
  std::cerr << '\n' << std::endl;

#ifdef DEBUG
  {TourTreeModel<0,0> x; sim.run(x);}
  {HeapModel<0,0,0> x; sim.run(x);}
#else
  {TourTreeModel<1,0> x; sim.run(x);}
  {TourTreeModel<0,0> x; sim.run(x);}
  {GeobucketModel<0,0,0,0,0,0,0> x(4, 32); sim.run(x);}
  {GeobucketModel<0,0,0,0,0,0,0> x(2, 32); sim.run(x);}
  {GeobucketModel<0,0,0,1,0,0,0> x(4, 32); sim.run(x);}
  {StlSetModel<1> x; sim.run(x);}
  {StlSetModel<0> x; sim.run(x);}
  {HeapModel<0,0,0> x; sim.run(x);}
  {HeapModel<1,0,0> x; sim.run(x);}
  {HeapModel<0,1,0> x; sim.run(x);}
  {HeapModel<1,1,0> x; sim.run(x);}
#endif

  sim.printData(std::cout);
  return 0;
}
