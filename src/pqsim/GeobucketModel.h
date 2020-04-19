#ifndef GEOBUCKET_MODEL_GUARD
#define GEOBUCKET_MODEL_GUARD

#include "Model.h"
#include "mathic/Geobucket.h"

template<
  bool TrackFront,
  bool MinBucketBinarySearch,
  bool Premerge,
  bool CollectMax,
  int BucketStorage,
  size_t InsertFactor>
class GeobucketModelBase {
public:
  GeobucketModelBase(size_t geoBase, size_t minBucketSize):
    geoBase(geoBase), minBucketSize(minBucketSize) {}
  size_t geoBase;
  size_t minBucketSize;

  static const bool trackFront = TrackFront;
  static const bool minBucketBinarySearch = MinBucketBinarySearch;
  static const bool premerge = Premerge;
  static const bool collectMax = CollectMax;
  static const mathic::GeobucketBucketStorage bucketStorage =
    (mathic::GeobucketBucketStorage)BucketStorage;
  static const size_t insertFactor = InsertFactor;
};

template<
  bool OnSpans,
  bool TrackFront,
  bool MinBucketBinarySearch,
  bool Deduplicate,
  bool Premerge,
  bool CollectMax,
  int BucketStorage,
  size_t InsertFactor = 1>
class GeobucketModel :
  public Model<
    OnSpans,
    Deduplicate,
    false,
    mathic::Geobucket,
    GeobucketModelBase<
      TrackFront,
      MinBucketBinarySearch,
      Premerge,
      CollectMax,
      BucketStorage,
      InsertFactor> > {
public:
  GeobucketModel(size_t geoBase, size_t minBucketSize):
    Model<
    OnSpans,
    Deduplicate,
    false,
    mathic::Geobucket,
    GeobucketModelBase<
      TrackFront,
      MinBucketBinarySearch,
      Premerge,
      CollectMax,
      BucketStorage,
      InsertFactor> >(geoBase, minBucketSize) {}
};

#endif
