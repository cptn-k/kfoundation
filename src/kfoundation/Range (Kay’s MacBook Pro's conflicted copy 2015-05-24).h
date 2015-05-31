//
//  Range.h
//  KFoundation
//
//  Created by Kay Khandan on 3/1/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__Range__
#define __KFoundation__Range__

// Internal
#include "Tuple.h"
#include "Direction.h"
#include "Streamer.h"

namespace kfoundation {
  
  class RangeIterator;
  
  class Range : public Streamer {
    
  // --- PRIVATE FIELDS --- //
    
    private: Tuple _begin;
    private: Tuple _end;
    private: Tuple _size;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public:  Range();
    private: Range(kf_int8_t size);
    public:  Range(const Tuple& begin, const Tuple& end);
    public:  explicit Range(const Tuple& end);
    public:  Range(const Range& other);
    
        
  // --- METHODS --- //
    
    public: kf_int8_t getNDimensions() const;
    public: const Tuple& getBegin() const;
    public: const Tuple& getEnd() const;
    public: kf_int64_t indexToOrdinal(const Tuple& index) const;
    public: Range translate(const Tuple& amount) const;
    public: Range grow(const int s) const;
    public: Range shrink(const int s) const;
    public: Range border(const Direction& d, const int s) const;
    public: Range flip(const Direction& d) const;
    public: Range intersectWith(const Range& other) const;
    public: Range joinWith(const Range& other) const;
    public: bool isAdjecentTo(const Range& other) const;
    public: bool contains(const Tuple& point) const;
    public: Direction getRelativePositionTo(const Range& other) const;
    public: bool isEmpty() const;
    public: const Tuple& getSize() const;
    public: kf_int64_t getVolume() const;
    public: RangeIterator getIterator() const;
    public: Range operator+ (const int n) const;
    public: Range operator- (const int n) const;
    public: Range operator* (const int n) const;
    public: Range operator/ (const int n) const;
    
    // Inherited from Streamer //
    public: void printToStream(ostream& os) const;
    
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__Range__) */
