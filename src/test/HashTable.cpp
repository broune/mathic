#include "mathic/HashTable.h"
#include <gtest/gtest.h>

namespace {
  class HashTableConf 
  {
  public:
    typedef int Key;
    typedef int Value;

    size_t hash(Key k) {return k;}
    bool keysEqual(Key k1, Key k2) {return k1==k2;}
    void combine(Value &a, const Value &b){a+=b;}
  };
}

TEST(HashTable, NoOp) {
  HashTableConf C;
  mathic::HashTable<HashTableConf> H(C);
  ASSERT_TRUE(true);
};

