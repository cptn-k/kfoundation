#include "Person.h"
#include "Apple.h"

#include <kfoundation/InternetInputStream.h>
#include <kfoundation/UString.h>
#include <kfoundation/Logger.h>
#include <kfoundation/ParseException.h>

using namespace kfoundation;

int main(int argc, char** argv) {
  Ref<InternetAddress> serverAddress
      = new InternetAddress(K"127.0.0.1:1501");
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
    
    Ref<Person> person = new Person();
    person->readFromXmlStream(input.AS(InputStream));
    
    LOG << "Received person \"" << person->getName()
        << "\" with a \"" << person->getApple()->getColor()
        << "\" apple" << OVER;
  }
}