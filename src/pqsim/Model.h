#ifndef MODEL_GUARD
#define MODEL_GUARD

#include "Item.h"
#include <vector>

namespace ModelHelper {
  class NullConfigurationBase {};
  template<bool> struct OnSpans {};
  template<bool> struct UseDecreaseTop {};

  template<class DS>
  inline void push(DS& ds, const Value* begin, const Value* end, OnSpans<0>);
  template<class DS>
  inline void push(DS& ds, const Value* begin, const Value* end, OnSpans<1>);
  template<class DS, bool DT>
  inline Value pop(DS& ds, OnSpans<0>, UseDecreaseTop<DT>);

  template<class DS>
  inline Value pop(DS& ds, OnSpans<1>, UseDecreaseTop<0>);
  template<class DS>
  inline Value pop(DS& ds, OnSpans<1>, UseDecreaseTop<1>);

  inline Value deduplicate
   (std::vector<Value>& pending, const Value& a, const Value& b);
  inline Item deduplicate
   (std::vector<Item>& pending, const Item& a, const Item& b);
}

template<
  bool OnSpans,
  bool Deduplicate,
  bool UseDecreaseTop,
  template<class> class DataStructure,
  class ConfigurationBase = ModelHelper::NullConfigurationBase>
class Model {
public:
  Model(): _ds(Configuration()) {}
  template<class A>
  Model(const A& a): _ds(Configuration(a)) {}
  template<class A, class B>
  Model(const A& a, const B& b): _ds(Configuration(a, b)) {}

  std::string getName() const {
    return _ds.getName() +
      (UseDecreaseTop ? " dectop" : "") +
      (OnSpans ? " on spans" : " on elements");
  }

  void push(const Value* begin, const Value* end) {
    ModelHelper::push(_ds, begin, end, ModelHelper::OnSpans<OnSpans>());
  }

  Value pop() {
    return ModelHelper::pop(_ds,
      ModelHelper::OnSpans<OnSpans>(),
      ModelHelper::UseDecreaseTop<UseDecreaseTop>());
  }

  bool empty() const {return _ds.empty();}
  void print(std::ostream& out) const {_ds.print(out);}

  size_t getComparisons() const {
    return _ds.getConfiguration().getComparisons();
  }

  size_t getMemoryUse() const {
    return _ds.getMemoryUse();
  }

private:
  class Configuration : public ConfigurationBase {
  public:
    typedef typename ItemOrValue<OnSpans>::Entry Entry;

    Configuration(): _comparisons(0) {}
    template<class A, class B>
    Configuration(const A& a, const B& b):
      ConfigurationBase(a, b), _comparisons(0) {}

    typedef ::CompareResult CompareResult;
    CompareResult compare(const Entry& a, const Entry& b) const {
      ++_comparisons;
      return ::compare(a, b);
    }
    bool cmpLessThan(CompareResult r) const {return r == Less;}
    bool cmpEqual(CompareResult r) const {
      ASSERT(supportDeduplication);
      return r == Equal;
    }
    Entry deduplicate(const Entry& a, const Entry& b) const {
      ASSERT(supportDeduplication);
      return ModelHelper::deduplicate(_pending, a, b);
    }

    std::vector<Entry>& getPending() {return _pending;}
    size_t getComparisons() const {return _comparisons;}

    static const bool supportDeduplication = Deduplicate;

  private:
    mutable size_t _comparisons;
    mutable std::vector<Entry> _pending;
  };

  DataStructure<Configuration> _ds;
};

namespace ModelHelper {
  template<class DS>
  inline void push(DS& ds, const Value* begin, const Value* end, OnSpans<0>) {
    ds.push(begin, end);
  }
  template<class DS>
  inline void push(DS& ds, const Value* begin, const Value* end, OnSpans<1>) {
    ds.push(Item(begin, end));
  }

  template<class DS, bool DT>
  inline Value pop(DS& ds, OnSpans<0>, UseDecreaseTop<DT>) {
    Value topValue = ds.top();
    do {
      ds.pop();
    } while (!ds.empty() && ds.top() == topValue);
    return topValue;
  }

  template<class DS>
  inline Value pop(DS& ds, OnSpans<1>, UseDecreaseTop<0>) {
    ASSERT(DS::Configuration::supportDeduplication ||
      ds.getConfiguration().getPending().empty());
    Value topValue = ds.top().getValue();
    do {
      Item top = ds.pop();
      top.toNext();
      if (!top.atEnd())
        ds.push(top);

      if (DS::Configuration::supportDeduplication) {
        std::vector<Item>& pending = ds.getConfiguration().getPending();
        while (!pending.empty()) {
          // this has to work even if push changes pending, which is
          // why the loop is done like this.
          Item item = pending.back();
          pending.pop_back();
          ds.push(item);
        }
      } else {
        ASSERT(ds.getConfiguration().getPending().empty());
      }
    } while (!ds.empty() && ds.top().getValue() == topValue);
    ASSERT(DS::Configuration::supportDeduplication ||
      ds.getConfiguration().getPending().empty());
    return topValue;
  }

  template<class DS>
  inline Value pop(DS& ds, OnSpans<1>, UseDecreaseTop<1>) {
    ASSERT(DS::Configuration::supportDeduplication ||
      ds.getConfiguration().getPending().empty());
    Value topValue = ds.top().getValue();
    do {
      Item top = ds.top();
      top.toNext();
      if (!top.atEnd())
        ds.decreaseTop(top);
      else
        ds.pop();

      ASSERT(DS::Configuration::supportDeduplication ||
        ds.getConfiguration().getPending().empty());
      if (DS::Configuration::supportDeduplication) {
        std::vector<Item>& pending = ds.getConfiguration().getPending();
        while (!pending.empty()) {
          // this has to work even if push changes pending, which is
          // why the loop is done like this.
          Item item = pending.back();
          pending.pop_back();
          ds.push(item);
        }
      }
    } while (!ds.empty() && ds.top().getValue() == topValue);
    ASSERT(DS::Configuration::supportDeduplication ||
      ds.getConfiguration().getPending().empty());
    return topValue;
  }

  inline Value deduplicate
   (std::vector<Value>& pending, const Value& a, const Value& b) {
    return a;
  }
  inline Item deduplicate
   (std::vector<Item>& pending, const Item& a, const Item& b) {
    Item next = b;
    next.toNext();
    if (!next.atEnd())
      pending.push_back(next);
    return a;
  }
}

#endif
