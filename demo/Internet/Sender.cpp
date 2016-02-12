#include <kfoundation/InternetOutputStream.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/StreamParser.h>
#include <kfoundation/Logger.h>
#include <kfoundation/UString.h>

using namespace kfoundation;

int main(int argc, char** argv) {
  Ref<InternetAddress> serverAddress;

  if(argc == 2) {
    serverAddress = new InternetAddress(new UString(argv[2]));
  } else {
    serverAddress = new InternetAddress(K"127.0.0.1:1501");
  }

  Ref<InternetOutputStream> output = new InternetOutputStream(serverAddress);
  Ref<StreamParser> reader = new StreamParser(System::IN);

  while (true) {
    try {
      output->connect();
    } catch(KFException e) {
      LOG_ERR << e.getMessage() << OVER;
      return 1;
    }

    Ref<UString> input = reader->readAllBeforeNewLine();
    if(input->equals(K"exit")) {
      break;
    }
    LOG << "Sending: " << input << OVER;
    input->printToStream(output.AS(OutputStream));
    output->close();
  }

  return 0;
}