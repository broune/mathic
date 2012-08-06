#ifndef MATHIC_PAIR_QUEUE_GUARD
#define MATHIC_PAIR_QUEUE_GUARD

#include "stdinc.h"

#include "TourTree.h"

#include <memtailor.h>
#include <limits>
#include <iterator>
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace mathic {
  // A priority queue of integer pairs (col, row) with custom
  // comparison function via template. Pairs must be added to the data
  // structure one column index at a time, such as will be the case
  // for S-pairs in Buchberger's algorithm.
  //
  // Think of the data structure as storing a subset of the following
  // triangle
  //
  // row
  // 3|        x
  // 2|      x x
  // 1|    x x x
  // 0|  x x x x
  //   ---------
  //   0 1 2 3 4 col
  //
  // As you can see in this triangle, any pair (col, row) must have
  // col > row.
  //
  // The custom comparison is not directly between pairs but between
  // triples (col, row, pd) where pd is extra information that is
  // computed as a function pairData of the pair (col, row).  The
  // purpose of this is that pd=pairData(col, row) can contain
  // information that is necessary to compare two pairs but that is
  // expensive to compute. Storing pd then avoids the need to
  // recompute pd on each pair when comparing two pairs. If you do not
  // need pd then just let pairData compute an empty struct.
  //
  // The data structure stores only one pd value per column to cut
  // down on the memory cost associated to storing many pd's. The pd
  // value of a pair is computed exactly twice. This is a speed-memory
  // trade-off between the extremes of storing no pd's (slow due to
  // frequent recomputations) and storing a pd for every pair (can
  // take a lot of memory).
  //
  // The data structure is the S-pair queue that is described from a
  // high level in the paper "Practical Grobner Basis Computation"
  // that is available at http://arxiv.org/abs/1206.6940
  template<class Configuration>
  class PairQueue;

  namespace PairQueueNamespace {
	typedef unsigned int Index;

	// Used by PairData<Configuration> to construct a PairData object
	// to hold data for (col, row). PairData is not constructed in any
	// other way than to call this function.
	//
	// The default implementation default-constructs the PairData and
	// then calls Configuration::computePairData. Specialize (do not
	// overload) this template function for your particular
	// configuration type if you want something else to happen -- for
	// example you might not want default construction to occur.
	template<class Configuration>
	void constructPairData
	(void* memory, Index col, Index row, Configuration& conf) {
	  MATHIC_ASSERT(memory != 0);
	  MATHIC_ASSERT(col > row);
	  typename Configuration::PairData* pd =
		new (memory) typename Configuration::PairData();
	  conf.computePairData(col, row, *pd);
	}

	// Used by PairData<Configuration> to destruct a PairData object
	// currently holding data for (col, row). PairData is not
	// destructed in any other way than to call this function.
	//
	// The default implementation just calls the
	// destructor. Specialize (do not overload) this template function
	// for your particular configuration type if you want something
	// else to happen -- for example PairData might hold memory
	// allocated from a memory pool that you want to return to the
	// pool but you do not want to put a reference to the memory pool
	// inside every PairData.
	template<class Configuration>
	void destructPairData
	(typename Configuration::PairData* pd,
	 Index col, Index row, Configuration& conf) {
	  MATHIC_ASSERT(pd != 0);
	  MATHIC_ASSERT(col > row);
	  typedef typename Configuration::PairData PairData;
	  pd->~PairData();
	}
  }

  template<class C>
  class PairQueue {
  public:
	typedef C Configuration;
	typedef typename C::PairData PairData;
	typedef PairQueueNamespace::Index Index;

	// PairQueue stores a copy of the passed in configuration.
	PairQueue(const Configuration& conf);
	~PairQueue();

	// Returns the stored configuration.
	Configuration& configuration() {return mConf;}

	// Returns the stored configuration.
	Configuration const& configuration() const {return mConf;}

	// Returns how many columns the triangle has. O(1) time.
	size_t columnCount() const {return mColumnCount;}

	// Returns how many pairs are in the triangle. O(columnCount())
	// time.
	size_t pairCount() const;

	// Returns true if there are no pairs in the triangle. O(1) time.
	bool empty() const;

	// As addColumn, but the range [sortedRowsBegin, sortedRowsEnd)
	// must be sorted in weakly descending order according to the
	// custom sorting function.
	template<class Iter>
	void addColumnDescending(Iter rowsBegin, Iter rowsEnd);

    // Returns the maximal pair according to the custom ordering on
	// pairs.
	std::pair<size_t, size_t> topPair() const;

	// Returns the PairData of topPair().
	const PairData& topPairData() const;

	// Removes topPair() from the data structure.
	void pop();

	// Returns how many bytes of memory this data structure consumes
	// not including sizeof(*this).
	size_t getMemoryUse() const;

	// Returns a string that describes how this data structure was
	// configured.
	std::string name() const;

  private:
	typedef unsigned short SmallIndex;

    class Column {
	public:
	  template<class Iter>
	  static Column* create
	  (Index col, Iter rowsBegin, Iter rowsEnd, C& conf, memt::Arena& arena);

	  const PairData& pairData() {MATHIC_ASSERT(!empty()); return mPairData;}
	  Index columnIndex() const {return mColumnIndex;}
	  Index rowIndex() const;

	  void incrementRowIndex(C& conf); // recomputes pairData if not empty
	  bool empty() const;
	  size_t size() const; // number of pairs remaining in this column

	  void destruct(C& conf) {
		// if empty then we already destructed the data
		if (!empty())
		  destruct(rowIndex(), conf);
	  }

	private:
	  // Do not call contructors on Column as that would construct the
	  // PairData directly which is not allowed -- instead call the
	  // factory function.
	  Column(); // not available
	  Column(const Column&); // not available
	  void operator=(const Column&); // not available

	  // Do not call the destructor as that would destruct the
	  // PairData directly which is not allowed -- instead call
	  // destruct(conf).
	  ~Column(); // not available

	  void destruct(Index row, C& conf) {
		PairQueueNamespace::
		  destructPairData(&mPairData, columnIndex(), row, conf);
	  }

	  PairData mPairData; // pairData of (columnIndex(), rowIndex())
	  Index mColumnIndex; // all pairs here have this column index
	  
	  bool big() const; // returns true if we need to use big part of union
	  union { // the current row index is *begin
		Index* bigBegin;
		SmallIndex* smallBegin;
	  };
	  union { // the row indices lie in [begin, end)
		Index* bigEnd; 
		SmallIndex* smallEnd;
	  };
	};

	class ColumnSizeSummer {
	public:
	  ColumnSizeSummer(): mSizeSum(0) {}
	  size_t sizeSum() const {return mSizeSum;}
	  bool proceed(Column const* const column) {
		mSizeSum += column->size();
		return true;
	  }
	private:
	  size_t mSizeSum;
	};

	class ColumnDestructor {
	public:
	  ColumnDestructor(C& conf): mConf(conf) {}
	  bool proceed(Column* const column) {
		column->destruct(mConf);
		return true;
	  }
	private:
	  C& mConf;
	};

	class QueueConfiguration : TourTreeSuggestedOptions {
	public:
	  QueueConfiguration(Configuration& conf): mConf(conf) {}

	  typedef Column* Entry;

	  typedef typename C::CompareResult CompareResult;
	  CompareResult compare(const Entry& a, const Entry& b) const {
		return mConf.compare(a->columnIndex(), a->rowIndex(), a->pairData(),
							 b->columnIndex(), b->rowIndex(), b->pairData());
	  }
	  bool cmpLessThan(CompareResult cr) const {
		return mConf.cmpLessThan(cr);
	  }

	private:
	  Configuration& mConf;
	};
	typedef TourTree<QueueConfiguration> ColumnQueue;

	ColumnQueue mColumnQueue;
	size_t mColumnCount;
	memt::Arena mArena;
	memt::Arena mScratchArena;
	Configuration mConf;
  };

  //// Implementation
  template<class C>
  template<class Iter>
  typename PairQueue<C>::Column* PairQueue<C>::Column::create
  (Index const col,
   Iter const rowsBegin, Iter const rowsEnd,
   C& conf,
   memt::Arena& arena) {
	Column* column = arena.allocObjectNoCon<Column>();
	column->mColumnIndex = col;

#ifdef MATHIC_DEBUG
	// check that the passed in range is weakly descending according
	// to the custom order.
	if (rowsBegin != rowsEnd) {
	  Iter prevIt = rowsBegin;
	  Iter it = rowsBegin;
	  for (++it; it != rowsEnd; ++it, ++prevIt) {
		memt::Arena::PtrNoConNoDecon<PairData> prevPd(arena);
		memt::Arena::PtrNoConNoDecon<PairData> currentPd(arena);

		PairQueueNamespace::constructPairData(prevPd.get(), col, *prevIt, conf);
		try {
		  PairQueueNamespace::constructPairData
			(currentPd.get(), col, *it, conf);
		} catch (...) {
		  PairQueueNamespace::
			destructPairData(prevPd.get(), col, *prevIt, conf);
		  throw;
		}

		// check prev >= current, which is equivalent to !(prev < current)
		MATHIC_ASSERT
		  (!conf.cmpLessThan(conf.compare(col, *prevIt, *prevPd,
										  col, *it, *currentPd)));
		PairQueueNamespace::
		  destructPairData(currentPd.get(), col, *prevIt, conf);
		PairQueueNamespace::destructPairData(prevPd.get(), col, *prevIt, conf);
	  }
	}
#endif

	size_t const entryCount = std::distance(rowsBegin, rowsEnd);
	if (column->big()) {
	  std::pair<Index*, Index*> const range =
		arena.allocArrayNoCon<Index>(entryCount);
	  column->bigBegin = range.first;
	  column->bigEnd = range.second;
	  Index* rangeIt = range.first;
	  Iter rowsIt = rowsBegin;
	  for (; rangeIt != range.second; ++rangeIt, ++rowsIt) {
		MATHIC_ASSERT(rowsIt != rowsEnd);
		MATHIC_ASSERT(*rowsIt < col);
		MATHIC_ASSERT(*rowsIt < std::numeric_limits<Index>::max());
		*rangeIt = static_cast<Index>(*rowsIt);
	  }
	  MATHIC_ASSERT(rowsIt == rowsEnd);
	} else {
	  std::pair<SmallIndex*, SmallIndex*> range =
		arena.allocArrayNoCon<SmallIndex>(entryCount);
	  column->smallBegin = range.first;
	  column->smallEnd = range.second;
	  SmallIndex* rangeIt = range.first;
	  Iter rowsIt = rowsBegin;
	  for (; rangeIt != range.second; ++rangeIt, ++rowsIt) {
		MATHIC_ASSERT(rowsIt != rowsEnd);
		MATHIC_ASSERT(*rowsIt < col);
		MATHIC_ASSERT(*rowsIt < std::numeric_limits<SmallIndex>::max());
		*rangeIt = static_cast<SmallIndex>(*rowsIt);
	  }
	}
	MATHIC_ASSERT(column->size() == entryCount);
	MATHIC_ASSERT(column->empty() == (entryCount == 0));
	PairQueueNamespace::constructPairData
	  (&column->mPairData, col, *rowsBegin, conf);
	return column;
  }

  template<class C>
  typename PairQueue<C>::Index PairQueue<C>::Column::rowIndex() const {
	MATHIC_ASSERT(!empty());
	if (big())
	  return *bigBegin;
	else
	  return *smallBegin;
  }

  template<class C>
  void PairQueue<C>::Column::incrementRowIndex(C& conf) {
	MATHIC_ASSERT(!empty());
	if (big()) {
	  ++bigBegin;
	  if (bigBegin == bigEnd) {
		MATHIC_ASSERT(empty());
		destruct(*(bigBegin - 1), conf);
		return;
	  }
	} else {
	  ++smallBegin;
	  if (smallBegin == smallEnd) {
		MATHIC_ASSERT(empty());
		destruct(*(smallBegin - 1), conf);
		return;
	  }
	}
	MATHIC_ASSERT(!empty());
	conf.computePairData(columnIndex(), rowIndex(), mPairData);
  }

  template<class C>
  bool PairQueue<C>::Column::empty() const {
	if (big())
	  return bigBegin == bigEnd;
	else
	  return smallBegin == smallEnd;
  }

  template<class C>
  size_t PairQueue<C>::Column::size() const {
	if (big())
	  return bigEnd - bigBegin;
	else
	  return smallEnd - smallBegin;
  }

  template<class C>
  bool PairQueue<C>::Column::big() const {
	return columnIndex() >=
	  static_cast<size_t>(std::numeric_limits<SmallIndex>::max());
  }

  template<class C>
  PairQueue<C>::PairQueue(const Configuration& conf):
	mConf(conf),
	mColumnQueue(QueueConfiguration(mConf)),
	mColumnCount(0) {
  }

  template<class C>
  PairQueue<C>::~PairQueue() {
	ColumnDestructor destructor(mConf);
	mColumnQueue.forAll(destructor);
  }

  template<class C>
  bool PairQueue<C>::empty() const {
	MATHIC_ASSERT(mColumnQueue.empty() || !mColumnQueue.top()->empty());
	return mColumnQueue.empty();
  }

  template<class C>
  size_t PairQueue<C>::pairCount() const {
	ColumnSizeSummer summer;
	mColumnQueue.forAll(summer);
	return summer.sizeSum();
  }

  template<class C>
  template<class Iter>
  void PairQueue<C>::addColumnDescending
  (Iter const sortedRowsBegin, Iter const sortedRowsEnd) {
	if (mColumnCount >= std::numeric_limits<Index>::max())
	  throw std::overflow_error("Too large column index in PairQueue.");
	Index const newColumnIndex = static_cast<Index>(mColumnCount);
	++mColumnCount;
	if (sortedRowsBegin == sortedRowsEnd)
	  return;
	memt::Arena::Guard guard(mArena);

	Column* column = Column::create
	  (newColumnIndex, sortedRowsBegin, sortedRowsEnd, mConf, mArena);

	try {
	  mColumnQueue.push(column);
	} catch (...) {
	  column->destruct(mConf);
	  throw;
	}
	guard.release();
  }

  template<class C>
  void PairQueue<C>::pop() {
	MATHIC_ASSERT(!empty());

	Column* const topColumn = mColumnQueue.top();
	MATHIC_ASSERT(topColumn != 0);
	MATHIC_ASSERT(!topColumn->empty());

	// Note that all mathic queues allow doing this sequence of
	// actions: top(), change top element in-place, do decreaseTop/pop.
	topColumn->incrementRowIndex(mConf);
	if (topColumn->empty()) {
	  topColumn->destruct(mConf);
	  mColumnQueue.pop();
	} else
	  mColumnQueue.decreaseTop(topColumn);
  }

  template<class C>
  size_t PairQueue<C>::getMemoryUse() const {
	return mArena.getMemoryUse() + mColumnQueue.getMemoryUse();
  }

  template<class C>
  std::string PairQueue<C>::name() const {
	return std::string("PairQueue-") + mColumnQueue.getName();
  }

  template<class C>
  std::pair<size_t, size_t> PairQueue<C>::topPair() const {
	MATHIC_ASSERT(!mColumnQueue.empty());
	Column* topColumn = mColumnQueue.top();
	MATHIC_ASSERT(topColumn != 0);
	MATHIC_ASSERT(!topColumn->empty());
	return std::make_pair(topColumn->columnIndex(), topColumn->rowIndex());
  }

  template<class C>
  const typename PairQueue<C>::PairData& PairQueue<C>::topPairData() const {
	MATHIC_ASSERT(!mColumnQueue.empty());
	Column* topColumn = mColumnQueue.top();
	MATHIC_ASSERT(topColumn != 0);
	MATHIC_ASSERT(!topColumn->empty());
	return topColumn->pairData();
  }
}

#endif

