//
//  Chat.cpp
//  KFoundation
//
//  Created by Kay Khandan on 2/10/16.
//  Copyright © 2016 Kay Khandan. All rights reserved.
//

#include <kfoundation/InternetInputStream.h>
#include <kfoundation/InternetOutputStream.h>
#include <kfoundation/StreamParser.h>
#include <kfoundation/Logger.h>

using namespace kfoundation;

int main(int argc, char** argv) {
  Ref<InternetAddress> serverAddress;

  if(argc == 2) {
    serverAddress = new InternetAddress(new UString(argv[2]));
  } else {
    serverAddress = new InternetAddress(K"127.0.0.1:1501");
  }

  

}