//
//  Dictionary.h
//  KFoundation
//
//  Created by Kay Khandan on 11/27/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

#ifndef Dictionary_h
#define Dictionary_h

// Internal
#include "definitions.h"
#include "Ref.h"
#include "Array.h"

// Self
#include "RefDictionaryDecl.h"

namespace kfoundation {

//\/ DictionaryBase::Iterator /\///////////////////////////////////////////////

  template<typename KY, typename VREF>
  DictionaryBase<KY, VREF>::Iterator::Iterator(Ref< Array<Pair> > pairs)
  : _pairs(pairs),
    _pos(0),
    _last(pairs->getSize()),
    _thisPair(NULL)
  {
    if(_last >= 0) {
      _thisPair = &_pairs->at(0);
    }
  }


  template<typename KY, typename VREF>
  void DictionaryBase<KY, VREF>::Iterator::first() {
    _pos = 0;
    if(_last >= 0) {
      _thisPair = &_pairs->at(0);
    }
  }


  template<typename KY, typename VREF>
  void DictionaryBase<KY, VREF>::Iterator::next() {
    _pos++;
    if(_pos < _last) {
      _thisPair = &_pairs->at(_pos);
    }
  }


  template<typename KY, typename VREF>
  bool DictionaryBase<KY, VREF>::Iterator::hasMore() const {
    return _pos < _last;
  }


  template<typename KY, typename VREF>
  RefConst<KY> DictionaryBase<KY, VREF>::Iterator::getKey() const {
    if(IS_NULL(_thisPair)) {
      return NULL;
    }
    return _thisPair->_key;
  }


  template<typename KY, typename VREF>
  VREF DictionaryBase<KY, VREF>::Iterator::getValue() const {
    if(IS_NULL(_thisPair)) {
      return NULL;
    }
    return _thisPair->_value;
  }


//\/ DictionaryBase /\/////////////////////////////////////////////////////////

  // --- CONSTRUCTOR --- //

  template<typename KY, typename VREF>
  DictionaryBase<KY, VREF>::DictionaryBase() {
    _pairs = new Array<Pair>();
  }


  // --- METHODS --- //

  template<typename KY, typename VREF>
  kf_int32_t DictionaryBase<KY, VREF>::find(RefConst<KY> key) const {
    for(int i = _pairs->getSize() - 1; i >= 0; i--) {
      if(_pairs->at(i)._key->equals(key)) {
        return i;
      }
    }
    return KF_NOT_FOUND;
  }


  template<typename KY, typename VREF>
  void DictionaryBase<KY, VREF>::set(RefConst<KY> key, VREF value) {
    kf_int32_t index = find(key);
    if(index == KF_NOT_FOUND) {
      Pair& p = _pairs->push();
      p._key = key;
      p._value = value;
    } else {
      _pairs->at(index)._value = value;
    }
  }


  template<typename KY, typename VREF>
  VREF DictionaryBase<KY, VREF>::get(RefConst<KY> key) const {
    kf_int32_t index = find(key);
    if(index == KF_NOT_FOUND) {
      return NULL;
    }
    return _pairs->at(index)._value;
  }


  template<typename KY, typename VREF>
  Ref< RefArray<KY> > DictionaryBase<KY, VREF>::getKeys() const {
    Ref< RefArray<KY> > keys = new RefArray<KY>();
    kf_int32_t s = _pairs->getSize();
    for(int i = 0; i < s; i++) {
      keys->push(_pairs->at(i)._key);
    }
    return keys;
  }


  template<typename KY, typename VREF>
  void DictionaryBase<KY, VREF>::remove(RefConst<KY> key) {
    kf_int32_t i = find(key);
    if(i == KF_NOT_FOUND) {
      return;
    }
    _pairs->at(i)->_key = NULL;
    _pairs->at(i)->_value = NULL;
    _pairs->remove(i);
  }


  template<typename KY, typename VREF>
  bool DictionaryBase<KY, VREF>::containsKey(RefConst<KY> key) const {
    for(int i = _pairs->getSize() - 1; i >= 0; i--) {
      if(_pairs->at(i)._key->equals(key)) {
        return true;
      }
    }
    return false;
  }


  template<typename KY, typename VREF>
  typename DictionaryBase<KY, VREF>::Iterator
  DictionaryBase<KY, VREF>::getIterator() const {
    return DictionaryBase<KY, VREF>::Iterator(_pairs);
  }


  template<typename KY, typename VREF>
  void DictionaryBase<KY, VREF>::clear() {
    for(kf_int32_t i = _pairs->getSize() - 1; i >= 0; i--) {
      _pairs->at(i)._key = NULL;
      _pairs->at(i)._value = NULL;
    }
    _pairs->clear();
  }


//\/ RefDictionary /\//////////////////////////////////////////////////////////

  template<typename KEY, typename VALUE>
  RefDictionary<KEY, VALUE>::RefDictionary()
  : DictionaryBase<KEY, Ref<VALUE> >()
  {
    // Nothing;
  }


//\/ RefConstDictionary /\/////////////////////////////////////////////////////

  template<typename KEY, typename VALUE>
  RefConstDictionary<KEY, VALUE>::RefConstDictionary()
  : DictionaryBase<KEY, RefConst<VALUE> >()
  {
    // Nothing;
  }


} // namespace kfoundation

#endif /* Dictionary_h */