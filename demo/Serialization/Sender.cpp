#include "Person.h"
#include "Apple.h"

#include <kfoundation/InternetOutputStream.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/StreamParser.h>
#include <kfoundation/Logger.h>
#include <kfoundation/UString.h>
#include <kfoundation/XmlObjectSerializer.h>

using namespace kfoundation;

int main(int argc, char** argv) {
  if(argc != 3) {
    System::ERR << "Bad usage. Use: " << argv[0] << " person_name apple_color"
        << OVER;
    return 1;
  }
  
  Ref<Person> person = new Person(new UString(argv[1]));
  person->setApple(new Apple(new UString(argv[2])));
  
  Ref<InternetAddress> serverAddress = new InternetAddress(K"127.0.0.1:1501");
  Ref<InternetOutputStream> output = new InternetOutputStream(serverAddress);
  
  try {
    output->connect();
  } catch(KFException e) {
    LOG_ERR << e.getMessage() << OVER;
    return 1;
  }
 
  XmlObjectSerializer::printToStream(*person, output.AS(OutputStream));
  output->close();
  
  return 0;
}