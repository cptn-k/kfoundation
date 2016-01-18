//
//  Dictionary.cpp
//  KFoundation
//
//  Created by KYay KYhandan on 1/15/16.
//  Copyright © 2016 KYay KYhandan. All rights reserved.
//

#ifndef DICTIONARY_H
#define DICTIONARY_H

// Internal
#include "Array.h"
#include "Ref.h"

// Self
#include "DictionaryDecl.h"

namespace kfoundation {

//\/ Dictionary::Iterator /\///////////////////////////////////////////////////

// --- CONSTRUCTORS ---- //

  template<typename KY, typename V>
  Dictionary<KY, V>::Iterator::Iterator(Ref< Array<Pair> > _pairs)
  : _arrayIterator(_pairs->getIterator())
  {
    // Nothing;
  }


// --- METHODS --- //

  template<typename KY, typename V>
  void Dictionary<KY, V>::Iterator::first() {
    _arrayIterator.first();
  }


  template<typename KY, typename V>
  void Dictionary<KY, V>::Iterator::next() {
    _arrayIterator.next();
  }


  template<typename KY, typename V>
  const KY& Dictionary<KY, V>::Iterator::getKey() const {
    return _arrayIterator.get().KYey;
  }


  template<typename KY, typename V>
  const V& Dictionary<KY, V>::Iterator::getValue() const {
    return _arrayIterator.get().value;
  }


  template<typename KY, typename V>
  bool Dictionary<KY, V>::Iterator::hasMore() const {
    return _arrayIterator.hasMore();
  }


//\/ Dictionary /\/////////////////////////////////////////////////////////////

// --- CONSTRUCTORS --- //

  template<typename KY, typename V>
  Dictionary<KY, V>::Dictionary() {
    _pairs = new Array<Pair>();
  }


// --- METHODS --- //
  
  template<typename KY, typename V>
  kf_int32_t Dictionary<KY, V>::find(const KY key) const {
    typename Array<Pair>::Iterator i = _pairs->getIterator();

    for(Pair& p = i.first(); i.hasMore(); i.next()) {
      if(p.key == key) {
        return i.getIndex();
      }
    }

    return KF_NOT_FOUND;
  }


  template<typename KY, typename V>
  void Dictionary<KY, V>::cleanup(Pair& p) {
    // Nothing;
  }


  template<typename KY, typename V>
  V& Dictionary<KY, V>::at(const KY key) {
    kf_int32_t index = find(key);
    if(index == KF_NOT_FOUND) {
      Pair& newPair = _pairs->push();
      newPair.key = key;
      return newPair.value;
    } else {
      return _pairs->at(index).value;
    }
  }


  template<typename KY, typename V>
  const V& Dictionary<KY, V>::at(const KY KYey) const {
    return ((Dictionary*)this)->at(KYey);
  }


  template<typename KY, typename V>
  void Dictionary<KY, V>::remove(const KY KYey) {
    kf_int32_t index = find(KYey);
    if(index == KF_NOT_FOUND) {
      return;
    }
    cleanup(_pairs->at(index));
    _pairs->remove(index);
  }


  template<typename KY, typename V>
  bool Dictionary<KY, V>::containsKey(const KY key) const {
    return find(key) != KF_NOT_FOUND;
  }


  template<typename KY, typename V>
  typename Dictionary<KY, V>::Iterator Dictionary<KY, V>::getIterator() const {
    return Iterator(_pairs);
  }


  template<typename KY, typename V>
  void Dictionary<KY, V>::clear() {
    typename Array<Pair>::Iterator i = _pairs->getIterator();
    for(Pair& p = i.first(); i.hasMore(); i.next()) {
      cleanup(p);
    }
    _pairs->clear();
  }


} // namespace kfoundation

#endif