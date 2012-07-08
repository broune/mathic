#include "ColumnPrinter.h"

namespace mathic {
  namespace {
	size_t getLineWidth(const std::string& str, size_t pos) {
	  size_t startPos = pos;
	  while (pos < str.size() && str[pos] != '\n')
		++pos;
	  return pos - startPos;
	}

	void printSpaces(std::ostream& out, size_t howMany) {
	  while (howMany > 0) {
		out << ' ';
		--howMany;
	  }
	}
  }

  ColumnPrinter::ColumnPrinter(size_t columnCount):
	_cols() {
	while (columnCount > 0) {
	  addColumn();
	  --columnCount;
	}
  }

  ColumnPrinter::~ColumnPrinter() {
	for (std::vector<Col*>::iterator it = _cols.begin();
		 it != _cols.end(); ++it)
	  delete *it;
  }

  void ColumnPrinter::setPrefix(const std::string& prefix) {
	_prefix = prefix;
  }

  void ColumnPrinter::addColumn(bool flushLeft,
								const std::string& prefix,
								const std::string& suffix) {
	std::auto_ptr<Col> col(new Col());
	col->prefix = prefix;
	col->suffix = suffix;
	col->flushLeft = flushLeft;

	_cols.push_back(0);
	_cols.back() = col.release(); // push_back didn't throw, so safe to release
  }

  size_t ColumnPrinter::getColumnCount() const {
	return _cols.size();
  }

  std::ostream& ColumnPrinter::operator[](size_t col) {
	MATHIC_ASSERT(col < getColumnCount());
	return _cols[col]->text;
  }

  void ColumnPrinter::print(std::ostream& out) const {
	// Calculate the width of each column.
	std::vector<size_t> widths(getColumnCount());
	for (size_t col = 0; col < getColumnCount(); ++col) {
	  const std::string& text = _cols[col]->text.str();
	  size_t maxWidth = 0;

	  size_t pos = 0;
	  while (pos < text.size()) {
		size_t width = getLineWidth(text, pos);
		if (width > maxWidth)
		  maxWidth = width;

		// We can't just increment pos unconditionally by width + 1, as
		// that could result in an overflow.
		pos += width;
		if (text[pos] == '\n')
		  ++pos;
	  }
	  widths[col] = maxWidth;
	}

	// Print each row
	std::vector<size_t> poses(getColumnCount());
	while (true) {
	  bool done = true;
	  for (size_t col = 0; col < getColumnCount(); ++col) {
		if (poses[col] < _cols[col]->text.str().size()) {
		  done = false;
		  break;
		}
	  }
	  if (done)
		break;

	  out << _prefix;
	  for (size_t col = 0; col < getColumnCount(); ++col) {
		out << _cols[col]->prefix;

		const std::string& text = _cols[col]->text.str();
		size_t& pos = poses[col];
		size_t width = getLineWidth(text, pos);

		if (!_cols[col]->flushLeft)
		  printSpaces(out, widths[col] - width);

		while (pos < text.size()) {
		  if (text[pos] == '\n') {
			++pos;
			break;
		  }
		  out << text[pos];
		  ++pos;
		}

		if (_cols[col]->flushLeft)
		  printSpaces(out, widths[col] - width);

		out << _cols[col]->suffix;
	  }
	  out << '\n';
	}
  }

  std::string ColumnPrinter::commafy(unsigned long long l) {
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

  std::string ColumnPrinter::percent(
      unsigned long long numerator,
      unsigned long long denominator) {
    return percent(static_cast<double>(numerator) / denominator);
  }

  std::string ColumnPrinter::percent(double ratio) {
    return oneDecimal(ratio * 100) + '%';
  }

  std::string ColumnPrinter::ratio(
    unsigned long long numerator,
    unsigned long long denominator) {
    return oneDecimal(static_cast<double>(numerator) / denominator);
  }

  std::string ColumnPrinter::oneDecimal(double d) {
    std::ostringstream out;
    unsigned long long l = static_cast<unsigned long long>(d * 10 + 0.5);
    out << l / 10 << '.' << l % 10;
	return out.str();
  }

  std::string ColumnPrinter::bytesInUnit(unsigned long long bytes) {
	std::ostringstream out;
	if (bytes < 1024) {
	  out << bytes << 'b';
    } else {
	  const char* units[] = {"kb", "mb", "gb", "tb"};
	  const size_t unitCount = sizeof(units) / sizeof(*units);
	  double amount = static_cast<double>(bytes) / 1024.0;
	  size_t i = 0;
	  for (i = 0; i < unitCount && amount >= 1024; ++i)
        amount /= 1024.0;
	  out << oneDecimal(amount) << units[i];
    }
    return out.str();
  }


  std::ostream& operator<<(std::ostream& out, const ColumnPrinter& printer) {
	printer.print(out);
	return out;
  }

  void print(FILE* out, const ColumnPrinter& pr) {
	std::ostringstream str;
	str << pr;
	fputs(str.str().c_str(), out);
  }
}
