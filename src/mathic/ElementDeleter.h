#ifndef MATHIC_ELEMENT_DELETER_GUARD
#define MATHIC_ELEMENT_DELETER_GUARD

#include "stdinc.h"
#include <memory>

namespace mathic {
  // The purpose of this class is to call delete on the elements of a
  // standard library container which holds pointers. It removes the
  // elements from the container as they are being deleted, but it does
  // not delete the container itself. Do not use this class to
  // deallocate a vector of built-in arrays, since those require
  // deallocation using delete[] rather than delete.
  //
  // This can be a convenience when coding, but the real purpose is to
  // avoid memory leaks in the face of an exception being thrown. When a
  // standard library container goes out of scope, its destructor is
  // called, but the destructor does not deallocate pointer elements, so
  // they will leak unless special care is taken to avoid that, such as
  // using this class.
  //
  // The most straight-forward solution may seem to use
  // e.g. vector<auto_ptr<T>> in place of vector<T*>, but this does not
  // work because storing auto_ptr<T> in a standard library container is
  // never safe (look this up on the internet to get the details).
  //
  // The reference counting smart pointer class shared_ptr from Boost or
  // TR1 would solve this issue even better by using
  // vector<shared_ptr<T>> instead of vector<T*>, but that would require
  // a dependency on Boost or on having a very recent compiler which
  // incorporates TR1.
  //
  // TODO: make an AutoVector that wraps std::vector instead of or as a
  // supplement to this class.
  //
  // Container::value_type is required to be a pointer type.
  template<class Container>
  class ElementDeleter {
   public:
    // Only the constructor can attach an ElementDeleter to a container,
    // and this helps ensure (but does not guarantee) the precondition
    // of the destructor of ElementDeleter.
    ElementDeleter(Container& container): _container(&container) {
    }

    // Calls deleteElements() and shares its precondition.
    ~ElementDeleter() {
      deleteElements();
    }

    // Call release() to prevent this ElementDeleter from manipulating
    // the container in any way.
    void release() {
      _container = 0;
    }

    // If release has been called, this method does nothing. Otherwise
    // the container must still be valid, and then delete is called on
    // each element of the array and clear is called on the container.
    void deleteElements() {
      if (_container == 0)
        return;

      // The code below may seem obviously correct, but it is a
      // non-trivial fact that it works in the face of exceptions.
      //
      // First of all, we are allowed to assume that destructors do not
      // throw exceptions, so the loop will run to completion.
      //
      // Normally clear() *can* throw an exception according to the
      // standard (which is weird), but if the copy constructor and
      // assignment operator do not throw exceptions, then it does
      // not. In our case, we require the element type to be a pointer
      // type, so for this reason only do we know that clear will not
      // throw an exception. Thus we do not have to worry about leaving
      // around a container full of invalid pointers if clear() should
      // throw an exception.

      typename Container::iterator end = _container->end();
      typename Container::iterator it = _container->begin();
      for (; it != end; ++it)
        delete *it;
      _container->clear();
    }

   private:
    Container* _container;
  };

  // exceptionSafePushBack is designed to work around the fact that this
  // code can leak memory:
  //
  //   vector<int*> vec;
  //   ElementDeleter<vector<int*> > elementDeleter(vec);
  //   auto_ptr<int> p(new int());
  //   vec.push_back(p.release())
  //
  // This is because push_back can fail by throwing a bad_alloc, and at
  // that point the pointer in p has already been released, so that
  // pointer is now lost and hence the memory is leaked.
  //
  // This can be fixed by replacing
  //
  //   vec.push_back(p.release())
  //
  // by
  //
  //   vec.push_back(0);
  //   vec.back() = p.release();
  //
  // but this is annoying and looks quite strange. It is much clearer to
  // write
  //
  //   exceptionSafePushBack(vec, p);
  //
  template<class Container, class Element>
  void exceptionSafePushBack(Container& container, std::auto_ptr<Element> pointer) {
    container.push_back(0);
    container.back() = pointer.release();
  }

  // Serves the same purpose as exceptionSafePushBack, except that this
  // overload will simply ignore an exception without propagating it.
  // Thus if there is not enough memory to add the element to the container,
  // the element will simply get deleted and the container will remain
  // unchanged. This works well if e.g. adding an element to a cache, where
  // it is no great problem if the element gets deleted rather than added.
  template<class Container, class Element>
  void noThrowPushBack(Container& container, std::auto_ptr<Element> pointer) throw () {
    try {
      exceptionSafePushBack(container, pointer);
    } catch (const std::bad_alloc&) {
      // Ignore the exception.
    }
  }
}

#endif
