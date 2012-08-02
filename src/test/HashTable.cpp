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

  typedef mathic::HashTable<HashTableConf> HashTab;
}

TEST(HashTable, NoOp) {
  HashTableConf C;
  HashTab H(C);

  H.insert(1,3);
  H.insert(14,7);
  H.insert(17,7);
  H.insert(14,4);

  HashTab::Node *p = H.lookup(14);
  ASSERT_FALSE(p == NULL);
  ASSERT_EQ(p->key,14);
  ASSERT_EQ(p->value,11);
};

