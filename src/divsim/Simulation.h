#ifndef SIMULATION_GUARD
#define SIMULATION_GUARD

#include "Monomial.h"
#include "mathic/Timer.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <algorithm>

class Simulation {
 public:
  Simulation(size_t repeats, bool printPartialData):
   _repeats(repeats), _printPartialData(printPartialData), _simType("none") {}

  void makeStandard(size_t varCount, size_t inserts, size_t queries, bool findAll);

  template<class DivFinder>
  void run();
  template<class DivFinder, class Param1>
  void run(const Param1& param1);
  template<class DivFinder, class Param1, class Param2>
  void run(const Param1& param1, const Param2& param2);
  template<class DivFinder, class Param1, class Param2, class Param3>
  void run(const Param1& param1, const Param2& param2, const Param3& param3);
  template<class DivFinder, class P1, class P2, class P3, class P4>
  void run(const P1& p1, const P2& p2, const P3& param3, const P4& param4);
  template<class DivFinder, class P1, class P2, class P3, class P4, class P5>
  void run(const P1& p1, const P2& p2, const P3& param3, const P4& param4,
    const P5& p5);
  template<class DivFinder, class P1, class P2, class P3, class P4, class P5,
  class P6>
  void run(const P1& p1, const P2& p2, const P3& param3, const P4& param4,
    const P5& p5, const P6& p6);

  void printData(std::ostream& out) const;

 private:
  struct SimData {
    bool operator<(const SimData& sd) const;
    void print(std::ostream& out);

    std::string _name;
    unsigned long _mseconds;
    unsigned long long _expQueryCount;
  };

  template<class DivFinder>
  void run(DivFinder& finder);

  enum EventType {
    InsertUnknown,
    InsertKnown,
    QueryNoDivisor,
    QueryHasDivisor,
    QueryUnknown,
    StateUnknown,
    StateKnown
  };
  struct Event {
    EventType _type;
    std::vector<int> _monomial;
    std::vector<const Monomial::Exponent*> _state;
#ifdef DEBUG
    std::vector<Monomial> _allMonomials;
#else
    size_t _monomialCount;
#endif
  };
  class MonomialStore;

  bool _findAll;
  std::vector<Event> _events;
  std::vector<SimData> _data;
  size_t _varCount;
  size_t _repeats;
  bool _printPartialData;
  std::string _simType;
};

template<class DivFinder>
void Simulation::run() {
  DivFinder finder(_varCount);
  run(finder);
}

template<class DivFinder, class Param1>
void Simulation::run(const Param1& param1) {
  DivFinder finder(_varCount, param1);
  run(finder);
}

template<class DivFinder, class Param1, class Param2>
void Simulation::run(const Param1& param1, const Param2& param2) {
  DivFinder finder(_varCount, param1, param2);
  run(finder);
}

template<class DivFinder, class Param1, class Param2, class Param3>
void Simulation::run
(const Param1& param1, const Param2& param2, const Param3& param3) {
  DivFinder finder(_varCount, param1, param2, param3);
  run(finder);
}

template<class DivFinder, class P1, class P2, class P3, class P4>
void Simulation::run
(const P1& param1, const P2& param2, const P3& param3, const P4& param4) {
  DivFinder finder(_varCount, param1, param2, param3, param4);
  run(finder);
}

template<class DivFinder, class P1, class P2, class P3, class P4, class P5>
void Simulation::run
(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5) {
  DivFinder finder(_varCount, p1, p2, p3, p4, p5);
  run(finder);
}

template<class DivFinder, class P1, class P2, class P3, class P4,
class P5, class P6>
void Simulation::run
(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5,
const P6& p6) {
  DivFinder finder(_varCount, p1, p2, p3, p4, p5, p6);
  run(finder);
}

class Simulation::MonomialStore {
public:
  MonomialStore() {clear();}
  void clear() {
#ifdef DEBUG
    _monomials.clear();
#else
    _monomialCount = 0;
#endif
  }

  void push_back(const Monomial& monomial) {
    proceed(monomial);
  }
  bool proceed(const Monomial& monomial) {
#ifdef DEBUG
    _monomials.push_back(monomial);
#else
    ++_monomialCount;
#endif
    return true;
  }

  template<class Finder>
  void checkInsert(Event& e, const Finder& finder) {
#ifdef DEBUG
    std::sort(_monomials.begin(), _monomials.end());
#endif

    if (e._type == InsertUnknown) {
#ifdef DEBUG
      e._allMonomials.clear();
      for (size_t i = 0; i < _monomials.size(); ++i)
        e._allMonomials.push_back(_monomials[i]);
#else
      e._monomialCount = _monomialCount;
#endif
      e._type = InsertKnown;
    } else {
#ifdef DEBUG
      ASSERT(_monomials == e._allMonomials);
#else
      if (_monomialCount != e._monomialCount) {
        std::cerr << "Finder \"" << finder.getName() <<
          "\" found incorrect number of monomials." << std::endl;
        std::exit(1);
      }
#endif
    }
  }

  template<class Finder>
  void checkQuery(Event& e, const Finder& finder) {
#ifdef DEBUG
    for (size_t d = 0; d < _monomials.size(); ++d) {
      for (size_t var = 0; var < e._monomial.size(); ++var) {
        ASSERT(_monomials[d][var] <= e._monomial[var]);
      }
    }
    std::sort(_monomials.begin(), _monomials.end());
#endif

    if (e._type == QueryUnknown) {
      bool noMonomials;
#ifdef DEBUG
      e._allMonomials.clear();
      for (size_t i = 0; i < _monomials.size(); ++i)
        e._allMonomials.push_back(_monomials[i]);
      noMonomials = _monomials.empty();
#else
      e._monomialCount = _monomialCount;
      noMonomials = _monomialCount == 0;
#endif
      e._type = noMonomials ? QueryNoDivisor : QueryHasDivisor;
    } else {
#ifdef DEBUG
      for (size_t i = 0; i < _monomials.size(); ++i)
        ASSERT(_monomials[i] == e._allMonomials[i]);
#else
      if (_monomialCount != e._monomialCount) {
        std::cerr << "Finder \"" << finder.getName() <<
          "\" found incorrect number of monomials." << std::endl;
        std::exit(1);
      }
#endif
    }
  }

private:
#ifdef DEBUG
  std::vector<Monomial> _monomials;
#else
  size_t _monomialCount;
#endif
};

struct ForAll {
public:
  ForAll(std::vector<const Monomial::Exponent*>& entries): _entries(entries) {}
  bool proceed(const Monomial& m) {
    _entries.push_back(m.getPointer());
    return true;
  }

private:
  std::vector<const Monomial::Exponent*>& _entries;
};

template<class DivFinder>
void Simulation::run(DivFinder& finder) {
  mic::Timer timer;
  std::vector<Monomial> divisors;
  std::vector<const Monomial::Exponent*> tmp;
  for (size_t step = 0; step < _repeats; ++step) {
    for (size_t i = 0; i < _events.size(); ++i) {
      Event& e = _events[i];
      if (e._type == InsertKnown || e._type == InsertUnknown) {
        divisors.clear();
        MonomialStore store;
        if (0) {
          // here to make sure it compiles, also easy to switch to checking this instead.
          finder.insert(e._monomial);
        } else
          finder.insert(e._monomial, store);
        store.checkInsert(e, finder);
      } else if (e._type == StateUnknown || e._type == StateKnown) {
        tmp.clear();
        ForAll forAll(tmp);
        finder.forAll(forAll);
        if (e._type == StateUnknown) {
          e._type = StateKnown;
          e._state.swap(tmp);
        } else {
          if (e._state != tmp) {
            std::cerr << "states differ." << std::endl;
            std::exit(1);
          }
        }
      } else if (!_findAll) {
        typename DivFinder::Entry* entry = finder.findDivisor(e._monomial);
        if (entry == 0) {
          if (e._type == QueryHasDivisor) {
            std::cerr << "Divisor finder \"" << finder.getName()
                      << "\" failed to find divisor." << std::endl;
            std::exit(1);
          }
          e._type = QueryNoDivisor;
        } else {
#ifdef DEBUG
          for (size_t var = 0; var < _varCount; ++var) {
            ASSERT((*entry)[var] <= e._monomial[var]);
          }
#endif
          if (e._type == QueryNoDivisor) {
            std::cerr << "Divisor finder \"" << finder.getName() <<
              "\" found incorrect divisor." << std::endl;
            std::exit(1);
          }
          e._type = QueryHasDivisor;
        }
      } else {
        ASSERT(_findAll);
        divisors.clear();
        MonomialStore store;
        const_cast<const DivFinder&>(finder) // to test const interface
          .findAllDivisors(e._monomial, store);
        store.checkQuery(e, finder);
      }
    }
  }

  SimData data;
  data._mseconds = (unsigned long)timer.getMilliseconds();
  data._name = finder.getName();
  data._expQueryCount = finder.getExpQueryCount();
  _data.push_back(data);
  if (_printPartialData)
    data.print(std::cerr);
  std::cout << finder.size() << std::endl;
}

#endif
