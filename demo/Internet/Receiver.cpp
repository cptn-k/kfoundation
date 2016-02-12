#include <kfoundation/InternetInputStream.h>
#include <kfoundation/UString.h>
#include <kfoundation/Logger.h>

using namespace kfoundation;

int main(int argc, char** argv) {
  Ref<InternetAddress> serverAddress;

  if(argc == 2) {
    serverAddress = new InternetAddress(new UString(argv[2]));
  } else {
    serverAddress = new InternetAddress(K"127.0.0.1:1501");
  }

  Ref<InternetInputStream> input = new InternetInputStream();

  try {
    input->bind(serverAddress);
  } catch (KFException e) {
    LOG_ERR << e.getMessage() << OVER;
    return 1;
  }

  LOG << "Server ready at " << *serverAddress << OVER;

  while(true) {
    try {
      input->listen();
    } catch (KFException e) {
      LOG_ERR << e.getMessage() << OVER;
      return 1;
    }

    LOG->print("Received: ")->print(input.AS(InputStream))->over();
  }
}