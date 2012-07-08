#include "stdinc.h"
#include "Simulator.h"

#include "mathic/ColumnPrinter.h"
#include <queue>
#include <iterator>
#include <algorithm>
#include <set>

namespace {
  std::string commafy(unsigned long l) {
    std::stringstream out;
    out << l;
    std::string str;
    for (size_t i = 0; i < out.str().size(); ++i) {
      str += out.str()[i];
      if (i != out.str().size() - 1 && ((out.str().size() - i) % 3) == 1)
        str += ',';
    }
    return str;
  }

  struct SimBuilder {
    typedef Simulator::Event Event;
    SimBuilder(bool popDuplicates,
     std::vector<Value>& mem,
     std::vector<Event>& events):
     _popDuplicates(popDuplicates),
     _mem(mem),
     _events(events),
     _pushCount(0),
     _popCount(0),
     _liveCountSum(0),
     _pushSum(0),
     _duplicateCount(0) {
      _events.clear();
      _mem.clear();
    }

    size_t getLiveCount() const {return _queue.size();}
    size_t getPushCount() const {return _pushCount;}
    size_t getPushSum() const {return _pushSum;}
    size_t getPopCount() const {return _popCount;}
    size_t getAvgLiveCount() const {
      return _events.empty() ? 0 : _liveCountSum / _events.size();
    }
    size_t getDupliateCount() const {return _duplicateCount;}

    // call nothing but addToPush and isInPush
    // after calling beginPush until you call endPush.
    void beginPush() {
      ASSERT(_push.empty());
      _liveCountSum += getLiveCount();
      _events.push_back(Event());
      _events.back().begin = _mem.size();
    }
    void addToPush(Value v) {
      if (isLive(v))
        ++_duplicateCount;
      _push.insert(v); // for faster isInPush
      _mem.push_back(v);
      _queue.insert(v);
    }
    bool isInPush(Value v) {
      return _push.find(v) != _push.end();
    }
    void endPush() {
      Event& e = _events.back();
      e.size = _mem.size() - e.begin;
      std::vector<Value>::iterator begin = _mem.begin() + e.begin;
      std::sort(begin, begin + e.size, std::greater<Value>());
      ++_pushCount;
      _pushSum += e.size;
      _push.clear();
    }

    // todo: move clients to use begin...endpush
    template<class It>
    void push(It begin, It end) {
      beginPush();
      for (It it = begin; it != end; ++it)
        addToPush(*it);
      endPush();
    }

    Value pop() {
      _liveCountSum += getLiveCount();
      ++_popCount;
      Value top = *_queue.rbegin();
      do {
        std::multiset<Value>::iterator last = _queue.end();
        --last;
        _queue.erase(last);
      } while (_popDuplicates && !_queue.empty() && *_queue.rbegin() == top);
      Event e;
      e.popValue = top;
      _events.push_back(e);
      return top;
    }

    bool noLive() const {return _queue.empty();}

    Value getRandomLive() const {
      ASSERT(!noLive());
      const size_t randomIndex = rand() % _queue.size();
      std::multiset<Value>::const_iterator it = _queue.begin();
      std::advance(it, randomIndex);
      return *it;
    }

    bool isLive(Value v) const {
      return _queue.find(v) != _queue.end();
    }

  private:
    bool _popDuplicates;
    std::multiset<Value> _queue;
    std::multiset<Value> _push;
    std::vector<Value>& _mem;
    std::vector<Event>& _events;
    size_t _pushCount;
    size_t _popCount;
    size_t _liveCountSum;
    size_t _pushSum;
    size_t _duplicateCount;
  };

  std::string makeDescription(SimBuilder& sim, size_t repeats,
    std::string name) {
    std::ostringstream out;
    const size_t traffic = sim.getPushSum();
    out << "*** Simulation \"" << name << "\"\n ";
    out << sim.getPushCount() << " spans\n ";
    out << (traffic / sim.getPushCount()) << " per span on average\n ";
    out << sim.getAvgLiveCount() << " elements in queue on average\n ";
    out << traffic << " entries popped in total.\n ";
    out << (sim.getDupliateCount() * 100 / traffic) << "% duplicates\n ";
    out << repeats << " repeats.\n";
    return out.str();
  }
}

void Simulator::dupSpans
 (size_t pushSumGoal, size_t avgSpan, size_t avgLiveGoal,
 size_t dupPercentage) {
  SimBuilder sim(true, _mem, _events);
  //size_t liveDeviation = 30;
  size_t spanDeviation = 10;
  ASSERT(avgSpan > 0);
  if (avgSpan <= spanDeviation)
    spanDeviation = avgSpan - 1;

  while (sim.getPushSum() < pushSumGoal || !sim.noLive()) {
    bool doPush = (sim.getAvgLiveCount() < avgLiveGoal);
    if (sim.getPushSum() == pushSumGoal)
      doPush = false;
    else if (sim.noLive())
      doPush = true;

    if (doPush) {
      const size_t size =
        avgSpan - spanDeviation + rand() % (2 * spanDeviation);
      sim.beginPush();
      for (size_t i = 0; i < size; ++i) {
        if (!sim.noLive() &&
          static_cast<size_t>(rand() % 100) <= dupPercentage) {
          Value v;
          size_t tries = 0;
          // if there are only fewer elements to duplicate than there
          // are elements in the push, then this will never succeed.
          // It will also take a long time to succeed even in some situations
          // where it is possible. So we only try it for so long.
          do {
            v = (tries < 20 ? sim.getRandomLive() : rand());
            ++tries;
          } while (sim.isInPush(v));
          sim.addToPush(v);
        } else {
          Value v;
          do {
            v = rand();
          } while (sim.isInPush(v) || sim.isLive(v));
          sim.addToPush(v);
        }
      }
      sim.endPush();
    } else
      sim.pop();
  }
  _description = makeDescription(sim, _repeats, "dup spans");
}

void Simulator::orderSpans
(size_t spanCount, size_t spanSize, size_t avgSize) {
  SimBuilder sim(true, _mem, _events);
  size_t insNum = (spanSize + spanCount * avgSize) * 3;
  size_t deviation = (spanSize) / 2;
  if (deviation == 0)
    deviation = 1;
  if (deviation > avgSize)
    deviation = avgSize - 1;

  while (sim.getLiveCount() > 0 || sim.getPushCount() < spanCount) {
    const size_t live = sim.getLiveCount();
    const size_t minLive = avgSize - deviation;
    const size_t pushes = sim.getPushCount();
    if (live == 0 || (pushes < spanCount && live < minLive)) {
      sim.beginPush();
      for (size_t i = 0; i < spanSize; ++i) {
        Value v;
        do {
          v = insNum + rand() % (2*avgSize);
        } while (sim.isInPush(v));
        --insNum;
        sim.addToPush(v);
      }
      sim.endPush();
    } else
      sim.pop();
  }

  _description = makeDescription(sim, _repeats, "ordered spans");
}

void Simulator::randomSpans(size_t spanCount, size_t spanSize, size_t initialSize) {
  SimBuilder sim(false, _mem, _events);
  while (sim.getLiveCount() || sim.getPushCount() < spanCount) {
    bool doPop = rand() % (spanSize + 1) != 0;
    if (sim.getLiveCount() == 0)
      doPop = false;
    if (sim.getPushCount() == spanCount)
      doPop = true;

    if (doPop)
      sim.pop();
    else {
      size_t size = (sim.getPushCount() == 0 ? initialSize : spanSize);
      sim.beginPush();
      for (size_t i = 0; i < size; ++i) {
        Value v;
        do {
          v = rand();
        } while (sim.isInPush(v));
        sim.addToPush(v);
      }
      sim.endPush();
    }
  }
  _description = makeDescription(sim, _repeats, "random spans");
}

void Simulator::printEventSummary(std::ostream& out) const {
  out << _description << std::endl;
}

void Simulator::printEvents(std::ostream& out) const {
  out << "*** The " << _events.size() << " events are\n";
  for (size_t i = 0; i < _events.size(); ++i)
    print(_events[i], out);
}

void Simulator::print(const Event& e, std::ostream& out) const {
  if (e.size == 0)
    out << "Pop " << e.popValue << '\n';
  else {
    out << "Insert";
    std::vector<Value>::const_iterator it = _mem.begin() + e.begin;
    std::vector<Value>::const_iterator end = it + e.size;
    for (; it != end; ++it)
      out << ' ' << *it;
    out << '\n';
  }
}

void Simulator::printData(std::ostream& out) const {
  std::vector<SimData> sorted(_data);
  sort(sorted.begin(), sorted.end());
  out << "*** Simulation outcome ***" << std::endl;
  mic::ColumnPrinter pr;
  pr.addColumn(true);
  pr.addColumn(false, " ", "ms");
  pr.addColumn(false, " ", "cmps");
  pr.addColumn(false, " ", "kb");
  for (std::vector<SimData>::const_iterator it = sorted.begin();
    it != sorted.end(); ++it) {
    pr[0] << it->name << '\n';
    pr[1] << commafy(it->mseconds) << '\n';
    pr[2] << commafy(it->comparisons) << '\n';
    pr[3] << commafy(it->memoryUse / 1024) << '\n';
  }
  pr.print(out);
}

void Simulator::setupEvents() {
  size_t activeSum = 0;
  std::priority_queue<int> queue;
  typedef std::vector<Event>::iterator Iterator;
  Iterator end = _events.end();
  for (Iterator it = _events.begin(); it != end; ++it) {
    Event& e = *it;
    activeSum += queue.size();
    if (e.size == 0) {
      e.popValue = queue.top();
      queue.pop();
    } else {
      Value* begin = &(_mem[e.begin]);
      Value* end = begin + e.size;
      sort(begin, end, std::greater<Value>());
      for (const Value* v = begin; v != end; ++v)
        queue.push(*v);
    }
  }
  _avgSize = activeSum / _events.size();
}

void Simulator::SimData::print(std::ostream& out) {
  out << name
    << " " << commafy(mseconds) << " ms"
    << " " << commafy(comparisons) << " cmps"
    << " " << commafy(memoryUse / 1024) << " kb"
    << '\n';
}

bool Simulator::SimData::operator<(const SimData& sd) const {
  return mseconds < sd.mseconds;
}
