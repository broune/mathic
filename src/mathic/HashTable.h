// MathicGB copyright 2012 all rights reserved. MathicGB comes with ABSOLUTELY
// NO WARRANTY and is licensed as GPL v2.0 or later - see LICENSE.txt.


// issues:
//  1. memory management of nodes
//  2. statistics gathering
//  3. resizing
//  4. put ASSERT's in, to check for all manner of contracts

#ifndef MATHIC_HASHTABLE_GUARD
#define MATHIC_HASHTABLE_GUARD

#include "stdinc.h"

namespace mathic {

  template<class Configuration> 
  class HashTable;

  class BasicHashTableConfiguration {
    typedef int Key;
    typedef int Value;

    size_t hash(Key k);
    bool equals(Key k1, Key k2);
    bool combine(Value &a, const Value &b);
  };

  template<class Configuration> 
  class HashTable;

  template<class C> 
  class HashTable {
  public:
    typedef C Configuration;
    typedef void * node;

    // Create a hash table
    HashTable(Configuration &conf, unsigned int nbits = 10);

    ~HashTable() {}

    // Returns the stored configuration.
    Configuration& configuration() {return mConf;}
    
    // Returns the stored configuration.
    Configuration const& configuration() const {return mConf;}

    // insert the key 'k' into the hash table.  If the key is already there,
    // call C.combine on the old and new values.
    // If combine returns false, then remove the node from the hash table.
    // and return std::pair(false, ...)
    // else return std::pair(true, node in the hash table).
    std::pair<bool, node *> insert(const Key &k, const Value &v);

    // remove 'p' from the hash table.  'p' itself is not removed???!
    void remove(node *p);

    const Key &key(node *p) const { return static_cast<Node *>(p)->k; }

    const Value &value(node *p) const { return static_cast<Node *>(p)->v; }

    void reset(); // Major assumption: all nodes have been removed from the table already

    void hardReset(); // Slow, avoid if possible.

    // Returns how many bytes of memory this data structure consumes
    // not including sizeof(*this).
    size_t memoryUse() const;
    
    // Returns a string that describes how this data structure was
    // configured.
    std::string name() const;

  private:
    struct node {
      Node *next;
      Key k;
      Value v;
    };


    size_t mHashMask; // this is the number, in binary:  00001111...1, where
                      // the number of 1's is mLogTableSize

    size_t mTableSize;
    size_t mLogTableSize; // number of bits in the table: mTableSize should be 2^mLogTableSize

    size_t mNodeCount;  // current number of nodes in the hash table
    size_t mBinCount; // number of nonzero bins
    size_t mMaxCountBeforeRebuild;

    memt::BufferPool mNodePool;
    std::vector<node *> mHashTable;
    Configuration mConf;
  };

  template<class C>
  HashTable::HashTable(const Configuration &conf, unsigned int nbits):
    mLogTableSize(nbits),
    mTableSize(1 << nbits),
    mHashMask((1 << nbits) - 1),
    mNodePool(sizeof(Node)),
    mConf(conf)
  {
    mHashTable.resize(mTableSize);
    mMaxCountBeforeRebuild = mConf.rebuildThreshold * mTableSize;

    MATHIC_ASSERT(tableIsZero(mHashTable));
  }
    

  
} // namespace mathic

#endif

// Local Variables:
// indent-tabs-mode: nil
// End:
