//
//  Header.h
//  KFoundation
//
//  Created by Kay Khandan on 11/27/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

#ifndef DictionaryBase_H
#define DictionaryBase_H

// Internal
#include "RefDecl.h"
#include "RefArrayDecl.h"
#include "ArrayDecl.h"
#include "SerializingStreamer.h"

namespace kfoundation {

  template<typename KY, typename VREF>
  class DictionaryBase : public KFObject, public SerializingStreamer {

  // --- NESTED TYPES --- //

    public: class Pair : public SerializingStreamer {
      public: RefConst<KY> _key;
      public: VREF _value;
      private: static void printObject(Ref<ObjectSerializer> s, RefConst<KFObject> o);
      public: void serialize(Ref<ObjectSerializer> serializer) const;
    };


    public: class Iterator {
      private: Ref< Array<Pair> > _pairs;
      private: kf_int32_t _pos;
      private: kf_int32_t _last;
      private: Pair* _thisPair;
      public: Iterator(Ref< Array<Pair> > pairs);
      public: void first();
      public: void next();
      public: bool hasMore() const;
      public: RefConst<KY> getKey() const;
      public: VREF getValue() const;
    };


  // --- FIELDS --- //

    private: Ref< Array<Pair> > _pairs;


  // --- CONSTRUCTORS --- //

    public: DictionaryBase();


  // --- METHODS --- //

    private: kf_int32_t find(RefConst<KY> key) const;
    public: void set(RefConst<KY> key, VREF value);
    public: VREF get(RefConst<KY> key) const;
    public: Ref< RefArray<KY> > getKeys() const;
    public: void remove(RefConst<KY> key);
    public: bool containsKey(RefConst<KY> key) const;
    public: Iterator getIterator() const;
    public: void clear();

    // From SerializingStreamer
    public: void serialize(Ref<ObjectSerializer> serializer) const;

  };


  template<typename KEY, typename VALUE>
  class RefDictionary : public DictionaryBase<KEY, Ref<VALUE> > {
    public: RefDictionary();
  };


  template<typename KEY, typename VALUE>
  class RefConstDictionary : public DictionaryBase<KEY, RefConst<VALUE> > {
    public: RefConstDictionary();
  };


} // namespace kfoundation

#endif /* Header_h */