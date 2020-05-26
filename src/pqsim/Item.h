#ifndef ITEM_GUARD
#define ITEM_GUARD

#include <ostream>
#include <vector>

typedef unsigned long Value;

class Item;
enum CompareResult {Less, Equal, Greater};
CompareResult compare(Value a, Value b);
CompareResult compare(const Item& a, const Item& b);

class Item {
 public:
  Item(); // creates Item which is at end.
  explicit Item(Value value);
  Item(const Value* begin, const Value* end);

  bool atEnd() const;
  void toNext();
  Item getNext() const;
  size_t getLength() const;

  const Value* begin() const {return _current;}
  const Value* end() const {return _end;}

  Value getValue() const;

  void print(std::ostream& out) const;

  bool operator==(const Item& item);

 private:
  Value _value;
  const Value* _current;
  const Value* _end;
  Value dummy; // to make sizeof(Item) a power of two
};

template<bool> struct ItemOrValue{};
template<> struct ItemOrValue<true> {typedef Item Entry;};
template<> struct ItemOrValue<false> {typedef Value Entry;};

inline CompareResult compare(Value a, Value b) {
  if (a < b)
    return Less;
  if (b < a)
    return Greater;
  return Equal;
}

inline CompareResult compare(const Item& a, const Item& b) {
  if (a.getValue() < b.getValue())
    return Less;
  if (b.getValue() < a.getValue())
    return Greater;
  return Equal;
}


inline std::ostream& operator<<(std::ostream& out, const Item& item) {
  item.print(out);
  return out;
}

inline bool Item::atEnd() const {
  return _current == _end;
}

inline Value Item::getValue() const {
  ASSERT(!atEnd());
  return _value;
}

inline void Item::toNext() {
  ASSERT(!atEnd());
  ++_current;
  if (!atEnd()) {
	ASSERT(_value >= *_current);
	_value = *_current;
  }
}

inline Item Item::getNext() const {
  ASSERT(!atEnd());
  Item next(*this);
  next.toNext();
  return next;
}

inline bool Item::operator==(const Item& item) {
  return getValue() == item.getValue();
}

inline Item::Item(): _value(-1), _current(0), _end(0) {}

inline Item::Item(Value value): _value(value), _current(0), _end(0) {
  ++_end;
}

inline Item::Item(const Value* begin, const Value* end):
  _value(begin != end ? *begin : 0), _current(begin), _end(end) {}

inline size_t Item::getLength() const {
  return _end - _current;
}


#endif
