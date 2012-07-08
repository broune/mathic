#include "stdinc.h"
#include "Item.h"

void Item::print(std::ostream& out) const {
  if (atEnd())
	out << "invalid";
  else
	out << getValue();
}
