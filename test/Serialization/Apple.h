#ifndef APPLE_H
#define APPLE_H

#include <kfoundation/KFObject.h>
#include <kfoundation/Streamer.h>

using namespace kfoundation;

class Apple : public KFObject, public Streamer {

public: Apple();
public: ~Apple();

  // Inherited from Streamer
public: void printToStream(Ref<OutputStream> stream) const;

};

#endif // APPLE_H