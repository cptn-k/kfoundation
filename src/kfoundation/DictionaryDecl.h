//
//  Dictionary.hpp
//  KFoundation
//
//  Created by Kay Khandan on 1/15/16.
//  Copyright © 2016 Kay Khandan. All rights reserved.
//

#ifndef DICTIONARY_DECL_H
#define DICTIONARY_DECL_H

// Internal
#include "definitions.h"
#include "KFObject.h"
#include "RefDecl.h"
#include "ArrayDecl.h"

namespace kfoundation {

  template<typename KY, typename V>
  class Dictionary : public KFObject {

  // --- NESTED TYPES --- //

    public: class Pair {
      public: KY key;
      public: V value;
    };


    public: class Iterator {

    // --- FIELDS --- //

      private: typename Array<Pair>::Iterator _arrayIterator;


    // --- CONSTRUCTOR --- //

      public: Iterator(Ref< Array<Pair> > _pairs);


    // --- METHODS --- //

      public: void first();
      public: void next();
      public: const KY& getKey() const;
      public: const V& getValue() const;
      public: bool hasMore() const;

    };


  // --- FIELDS --- //

    private: Ref< Array<Pair> > _pairs;


  // --- CONSTRUCTORS --- //

    public: Dictionary();


  // --- METHODS --- //

    private: kf_int32_t find(const KY key) const;
    protected: virtual void cleanup(Pair& p);
    public: V& at(const KY key);
    public: const V& at(const KY key) const;
    public: void remove(const KY key);
    public: bool containsKey(const KY key) const;
    public: Iterator getIterator() const;
    public: void clear();

  };

} // namespace kfoundation

#endif /* Dictionary_hpp */