#ifndef MONOMIAL_GUARD
#define MONOMIAL_GUARD

#include <vector>
#include <ostream>

class Monomial {
public:
  typedef int Exponent;

  Monomial(): _exponents(0) {IF_DEBUG(_size = 0);}
  Monomial(std::vector<Exponent>& v): _exponents(&v[0]) {
    IF_DEBUG(_size = v.size());
  }

  inline Exponent& operator[](size_t index) {
    ASSERT(index < _size);
    return _exponents[index];
  }
  inline const Exponent& operator[](size_t index) const {
    ASSERT(index < _size);
    return _exponents[index];
  }

  const Exponent* getPointer() const {return _exponents;}

#ifdef DEBUG
  size_t size() const {return _size;}
  bool operator==(const Monomial& m) const {return _exponents == m._exponents;}
  bool operator<(const Monomial& m) const {return _exponents < m._exponents;}
#endif

private:
#ifdef DEBUG
  size_t _size;
#endif
  Exponent* _exponents;
};

inline std::ostream& operator<<(std::ostream& out, const Monomial& monomial) {
#ifdef DEBUG
  out << "(Monomial:";
  for (size_t i = 0; i < monomial.size(); ++i)
    out << ' ' << monomial[i];
  out << ')';
#else
  out << "(Monomial)";
#endif
  return out;
}

#endif
