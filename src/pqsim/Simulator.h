#ifndef SIMULATOR_GUARD
#define SIMULATOR_GUARD

#include "Item.h"
#include <queue>
#include <vector>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <string>

class Simulator {
public:
  Simulator(size_t repeats): _repeats(repeats), _simType("none") {}

  void dupSpans(size_t pushSumGoal, size_t avgSpan, size_t avgLiveGoal,
    size_t dupPercentage);
  void orderSpans(size_t spanCount, size_t spanSize, size_t avgSize);
  void randomSpans(size_t spanCount, size_t spanSize, size_t initialSize);

  template<class PQueue>
  void run(PQueue& pq, bool printData = true, bool printStates = false);

  void printEventSummary(std::ostream& out) const;
  void printEvents(std::ostream& out) const;
  void printData(std::ostream& out) const;

  struct Event {
    Event(): size(0) {}
    size_t begin;
    size_t size; // insert _mem( [begin, begin + size) ) if size > 0.
	Value popValue; // pop this value if size == 0
  };

private:
  struct SimData {
    std::string name;
    unsigned long comparisons;
    unsigned long mseconds;
    size_t memoryUse;
    bool operator<(const SimData& sd) const;
    void print(std::ostream& out);
  };

  void setupEvents();
  void print(const Event& e, std::ostream& out) const;

  std::vector<Event> _events;
  std::vector<SimData> _data;
  std::vector<Value> _mem;
  size_t _spanCount;
  size_t _spanSize;
  size_t _avgSize;
  size_t _repeats;
  std::string _simType;
  std::string _description;
};

template<class PQueue>
void Simulator::run(PQueue& pqueue, bool printData, bool printStates) {
  clock_t timeBegin = clock();
  std::vector<Event>::const_iterator end = _events.end();
  for (size_t turn = 0; turn < _repeats; ++turn) {
    typedef std::vector<Event>::const_iterator CIterator;
    CIterator end = _events.end();
    for (CIterator it = _events.begin(); it != end; ++it) {
      const Event& e = *it;
      if (printStates) {
        std::cerr << "*** The next event is\n";
        print(e, std::cerr);
        std::cerr << "*** The state of " << pqueue.getName()
          << " (" << pqueue.getComparisons() << " comparisons) is\n";
        pqueue.print(std::cerr);
        std::cerr << '\n';
      }
      if (e.size == 0) {
        Value item = pqueue.pop();
        if (!(item == e.popValue)) {
          std::cerr << "ERROR: queue " << pqueue.getName()
            << " gave incorrect value " << item << std::endl;
          exit(1);
        }
      } else {
        const Value* begin = &_mem[e.begin];
        pqueue.push(begin, begin + e.size);
      }
    }
  }
  clock_t timeEnd = clock();

  SimData data;
  data.name = pqueue.getName();
  data.memoryUse = pqueue.getMemoryUse();
  data.comparisons = pqueue.getComparisons();
  data.mseconds = (unsigned long)
    ((double(timeEnd) - timeBegin) * 1000) / CLOCKS_PER_SEC;
  _data.push_back(data);
  if (printData)
    data.print(std::cerr);
}

#endif
