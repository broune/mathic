#include "stdinc.h"

#include "DivListModel.h"
#include "KDTreeModel.h"
#include "Simulation.h"
#include "mathic/Timer.h"
#include <iostream>

int main() {
  const size_t repeats = IF_DEBUG(true ? 1 :) 1;
  Simulation sim(repeats, true);
  mic::Timer timer;
  std::cout << "Generating simulation. ";

    
#ifdef DEBUG
  sim.makeStandard(10, 400, 1000, true);
#else
  sim.makeStandard(10, 5000, 2000000, true);
#endif
  timer.print(std::cout);
  std::cout << std::endl;

#ifndef DEBUG
  sim.run<KDTreeModel<1,1,1,1,1> >(0, 0, 0, 1.0, 1000);
  sim.run<KDTreeModel<1,1,1,1,0> >(0, 0, 0, 1.0, 1000);
return 0;

  sim.run<KDTreeModel<0,0,1,2,1> >(1, 0, 0, 0.0, 0); // best tree, no mask
  sim.run<KDTreeModel<0,0,0,2,1> >(1, 0, 0, 0.0, 0); // best tree, no mask

  sim.run<KDTreeModel<0,0,1,2,1> >(1, 0, 0, 1.0, 1000); // best tree, mask
  sim.run<KDTreeModel<0,0,0,2,1> >(1, 0, 0, 1.0, 1000); // best tree, mask

  sim.run<DivListModel<0, 0> >(1, 1, 0, 0.5, 500);
  return 0;
#endif


  /*
  for (int minimizeOnInsert = 1; minimizeOnInsert <= 1; ++minimizeOnInsert) {
    for (int order = 0; order <= 2; ++order) {
      bool moveDivisorToFront = (order == 1);
      bool sortOnInsert = (order == 2);
      sim.run<DivListModel<1> >
        (minimizeOnInsert, moveDivisorToFront, sortOnInsert);
      sim.run<DivListModel<0> >
        (minimizeOnInsert, moveDivisorToFront, sortOnInsert);
    }
  }
  //*/

  //sim.run<DivListModel<0, 0> >(1, 0, 1, 0.0, 0); // best array, no mask
  /*
  sim.run<DivListModel<0> >(1, 0, 0)
  sim.run<DivListModel<0> >(1, 1, 0);
  sim.run<DivListModel<0> >(1, 0, 1);
  sim.run<DivListModel<1> >(1, 0, 0);
  sim.run<DivListModel<1> >(1, 1, 0);
  sim.run<DivListModel<1> >(1, 0, 1);
  //*/

  sim.run<KDTreeModel<0,0,1,1,0> >(0, 0, 0, 0.0, 0);
  sim.run<DivListModel<0,0> >(0, 1, 0, 0.5, 500);
  for (int mini = 0; mini <= 0; ++mini) {
    for (int sortOnInsert = 0; sortOnInsert <= 1; ++sortOnInsert) {
      for (int useDivCache = 0; useDivCache <= 1; ++useDivCache) {
        sim.run<KDTreeModel<1,1,1,1,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);
        sim.run<KDTreeModel<1,0,1,1,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);
        sim.run<KDTreeModel<0,0,1,1,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);

        sim.run<KDTreeModel<1,1,1,2,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);
        sim.run<KDTreeModel<1,0,1,2,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);
        sim.run<KDTreeModel<0,0,1,2,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);

        sim.run<KDTreeModel<1,1,1,3,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);
        sim.run<KDTreeModel<1,0,1,3,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);
        sim.run<KDTreeModel<0,0,1,3,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);

        sim.run<KDTreeModel<1,1,1,20,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);

        sim.run<KDTreeModel<1,1,0,1,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);
        sim.run<KDTreeModel<1,1,0,2,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);
        sim.run<KDTreeModel<1,1,0,40,0> >(mini,sortOnInsert,useDivCache, 0.5, 10);

		/*
        for (size_t leafSize = 5; leafSize <= 15; leafSize += 5)
          for (size_t start = 0; start <= 0; start += 200)
            for (double ratio = 0; ratio < 0.1; ratio += 0.2)
              sim.run<KDTreeModel<0,0> >(leafSize, mini,sortOnInsert,useDivisorCache, ratio, start);

              //*/
        /*sim.run<KDTreeModel<0> >(8, mini,sortOnInsert,useDivisorCache, 0, 0);
        sim.run<KDTreeModel<1> >(8, mini,sortOnInsert,useDivisorCache, 0, 0);
        sim.run<KDTreeModel<0> >(20, mini,sortOnInsert,useDivisorCache, 0, 0);
        sim.run<KDTreeModel<1> >(20, mini,sortOnInsert,useDivisorCache, 0, 0);
        sim.run<KDTreeModel<0> >(40, mini,sortOnInsert,useDivisorCache, 0, 0);
        sim.run<KDTreeModel<1> >(40, mini,sortOnInsert,useDivisorCache, 0, 0);
        sim.run<KDTreeModel<0> >(60, mini,sortOnInsert,useDivisorCache, 0, 0);
        sim.run<KDTreeModel<1> >(60, mini,sortOnInsert,useDivisorCache, 0, 0);*/

        /*
        sim.run<KDTreeModel<1> >(10, mini,sortOnInsert,useDivisorCache, 0.001, 2000);
        sim.run<KDTreeModel<1> >(10, mini,sortOnInsert,useDivisorCache, 0.001, 4000);

        sim.run<KDTreeModel<0> >(10, mini,sortOnInsert,useDivisorCache, 0.001, 6000);
        sim.run<KDTreeModel<1> >(10, mini,sortOnInsert,useDivisorCache, 0.001, 6000);

        sim.run<KDTreeModel<1> >(10, mini,sortOnInsert,useDivisorCache, 0.001, 8000);
        sim.run<KDTreeModel<1> >(10, mini,sortOnInsert,useDivisorCache, 0.001, 10000);*/
        //sim.run<KDTreeModel>(10, mini,sortOnInsert,useDivisorCache, 4, 2000);
        //sim.run<KDTreeModel>(20, mini,sortOnInsert,useDivisorCache, 0.75, 2000);
        //sim.run<KDTreeModel>(8, mini,sortOnInsert,useDivisorCache, 0.75, 2000);
      }
    }
  }
  //*/
  /*
  for (int mini = 1; mini <= 1; ++mini) {
    for (int noneFrontSort = 0; noneFrontSort <= 2; ++noneFrontSort) {
      bool tof = (noneFrontSort == 1);
      bool sort = (noneFrontSort == 2);
      
      for (double ratio = 0.5; ratio < 0.51; ratio += 0.1)
        for (size_t start = 500; start <= 500; start += 100)
          sim.run<DivListModel<1, 1> >(mini, tof, sort, ratio, start);
    }
  }
  //*/

  /* best for single query from best to worst
  sim.run<KDTreeModel<1,1> >(40, 1, 0, 0, 1.0, 1000); // best tree, mask
  sim.run<KDTreeModel<0,0> >(15, 1, 0, 0, 0.0, 0); // best tree, no mask
  sim.run<DivListModel<0, 1> >(1, 1, 0, 0.5, 500); // best array, mask
  sim.run<DivListModel<1, 1> >(1, 1, 0, 0.5, 500); // should be best linked, mask
  sim.run<DivListModel<0, 0> >(1, 0, 1, 0.0, 0); // best array, no mask
  sim.run<DivListModel<1, 0> >(1, 0, 1, 0.0, 0); // best linked, no mask
  //*/

  /* base div lists
  sim.run<DivListModel<0, 0> >(1, 0, 0, 0.0, 0); // base array
  sim.run<DivListModel<1, 0> >(1, 0, 0, 0.0, 0); // base linked
  //*/

  std::cout << "\n\n";
  sim.printData(std::cout);
  return 0;
}
