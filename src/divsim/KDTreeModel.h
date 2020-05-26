#ifndef K_D_TREE_MODEL_GUARD
#define K_D_TREE_MODEL_GUARD

#include "Monomial.h"
#include "mathic/KDTree.h"
#include <string>
#include <vector>

template<
  bool UseDivMask,
  bool UseTreeDivMask,
  bool PackedTree,
  size_t LeafSize,
  bool AllowRemovals>
class KDTreeModelConfiguration;

/** Helper class for KDTreeModel. */
template<bool UDM, bool UTDM, bool PT, size_t LS, bool AR>
class KDTreeModelConfiguration {
 public:
  typedef int Exponent;
  typedef ::Monomial Monomial;
  typedef Monomial Entry;

  KDTreeModelConfiguration
    (size_t varCount,
     bool sortOnInsert,
     bool useDivisorCache,
     double rebuildRatio,
     size_t minRebuild):
   _varCount(varCount),
   _sortOnInsert(sortOnInsert),
   _useDivisorCache(useDivisorCache),
   _useAutomaticRebuild((rebuildRatio > 0.0 || minRebuild > 0) && UDM),
   _rebuildRatio(rebuildRatio),
   _minRebuild(minRebuild),
   _expQueryCount(0) {
     ASSERT(rebuildRatio >= 0);
   }

  size_t getVarCount() const {return _varCount;}
  bool getSortOnInsert() const {return _sortOnInsert;}

  Exponent getExponent(const Monomial& monomial, size_t var) const {
    ++_expQueryCount;
    ASSERT(var < monomial.size());
    return monomial[var];
  }

  NO_PINLINE bool divides(const Monomial& a, const Monomial& b) const {
    for (size_t var = 0; var < getVarCount(); ++var)
      if (getExponent(b, var) < getExponent(a, var))
        return false;
    return true;
  }

  bool isLessThan(const Monomial& a, const Monomial& b) const {
    for (size_t var = 0; var < getVarCount(); ++var) {
      if (getExponent(a, var) < getExponent(b, var))
        return true;
      if (getExponent(b, var) < getExponent(a, var))
        return false;
    }
    return false;
  }

  size_t getLeafSize() const {return LeafSize;}
  bool getUseDivisorCache() const {return _useDivisorCache;}
  bool getDoAutomaticRebuilds() const {return _useAutomaticRebuild;}
  double getRebuildRatio() const {return _rebuildRatio;}
  size_t getRebuildMin() const {return _minRebuild;}

  static const bool UseDivMask = UDM;
  static const bool UseTreeDivMask = UTDM;
  static const bool PackedTree = PT;
  static const size_t LeafSize = LS;
  static const bool AllowRemovals = AR;

  unsigned long long getExpQueryCount() const {return _expQueryCount;}

 private:
  const size_t _varCount;
  const bool _sortOnInsert;
  const bool _useDivisorCache;
  const bool _useAutomaticRebuild;
  const double _rebuildRatio;
  const size_t _minRebuild;
  mutable unsigned long long _expQueryCount;
};

/** An instantiation of the capabilities of KDTree. */
template<
  bool UseDivMask,
  bool UseTreeDivMask,
  bool PackedTree,
  size_t LeafSize,
  bool AllowRemovals
>
class KDTreeModel {
 private:
  typedef KDTreeModelConfiguration
    <UseDivMask, UseTreeDivMask, PackedTree, LeafSize, AllowRemovals> C;
  typedef mathic::KDTree<C> Finder;
 public:
  typedef typename Finder::Monomial Monomial;
  typedef typename Finder::Entry Entry;

 KDTreeModel(size_t varCount,
             bool minimizeOnInsert,
             bool sortOnInsert,
             bool useDivisorCache,
             double rebuildRatio,
             size_t minRebuild):
  _finder(C(varCount, sortOnInsert, useDivisorCache, rebuildRatio, minRebuild)),
  _minimizeOnInsert(minimizeOnInsert) {
    ASSERT(!UseTreeDivMask || UseDivMask);
  }


  void insert(const Entry& entry);
  template<class MultipleOutput>
  void insert(const Entry& entry, MultipleOutput& removed);

  Entry* findDivisor(const Monomial& monomial) {
    return _finder.findDivisor(monomial);
  }
  const Entry* findDivisor(const Monomial& monomial) const {
    return _finder.findDivisor(monomial);
  }
  std::string getName() const;

  template<class DO>
  void findAllDivisors(const Monomial& monomial, DO& out) {
    _finder.findAllDivisors(monomial, out);
  }
  template<class DO>
  void findAllDivisors(const Monomial& monomial, DO& out) const {
    _finder.findAllDivisors(monomial, out);
  }
  template<class EO>
  void forAll(EO& out) {
    _finder.forAll(out);
  }
  template<class EO>
  void forAll(EO& out) const {
    _finder.forAll(out);
  }
  size_t size() const {return _finder.size();}

  unsigned long long getExpQueryCount() const {
    return _finder.getConfiguration().getExpQueryCount();
  }

  class Comparer;

 private:
  Finder _finder;
  bool _minimizeOnInsert;
};

template<bool UDM, bool UTDM, bool PT, size_t LS, bool AR>
inline void KDTreeModel<UDM, UTDM, PT, LS, AR>::insert(const Entry& entry) {
  if (!_minimizeOnInsert) {
    _finder.insert(entry);
    return;
  }
  if (findDivisor(entry) != 0)
    return;
  _finder.removeMultiples(entry);
  _finder.insert(entry);
}

template<bool UDM, bool UTDM, bool PT, size_t LS, bool AR>
template<class MultipleOutput>
inline void KDTreeModel<UDM, UTDM, PT, LS, AR>::
insert(const Entry& entry, MultipleOutput& removed) {
  if (!_minimizeOnInsert) {
    _finder.insert(entry);
    return;
  }
  if (findDivisor(entry) != 0)
    return;
  _finder.removeMultiples(entry, removed);
  _finder.insert(entry);
}

template<bool UDM, bool UTDM, bool PT, size_t LS, bool AR>
inline std::string KDTreeModel<UDM, UTDM, PT, LS, AR>::getName() const {
  return _finder.getName() +
    (_minimizeOnInsert ? " remin" : " nomin");
}

#endif
