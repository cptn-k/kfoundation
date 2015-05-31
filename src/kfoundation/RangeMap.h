//
//  RangeMap.h
//  KFoundation
//
//  Created by Hamed KHANDAN on 5/21/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__RangeMap__
#define __KFoundation__RangeMap__

#include "Range.h"

namespace kfoundation {
  class RangeMap {
    
  // --- FIELDS --- //
    
    private: Range _local;
    private: Tuple _offset;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: RangeMap();
    public: RangeMap(const Range& local);
    public: RangeMap(const Range& local, const Tuple& offset);
    
    
  // --- METHODS --- //
    
    public: const Range& getLocalRange() const;
    public: const Tuple& getGlobalOffset() const;
    public: Tuple mapLocalToGlobal(const Tuple& t);
    public: Tuple mapGlobalToLocal(const Tuple& t);
    
  };
} // namespace kfoundation

#endif /* defined(__KFoundation__RangeMap__) */
