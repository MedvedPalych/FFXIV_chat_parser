#include "pipe.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    
    setlocale(LC_ALL, "ru");
    Pipe pipe;

    Message request = pipe.read();
    if (!request.validServerInit()) {
        std::cerr << "Hook initialization failed" << std::endl;
        std::abort();
    }
    request.printString();

    pipe.write(Message::initFilter());

    Message reply = pipe.read();
    if (!reply.validFilterSetReply()) {
        std::cerr << "Filter setting failed" << std::endl;
        std::abort();
    }
    reply.printString();

    pipe.loop();
    
    return 0;
}
