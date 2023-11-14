// Парсер и архиватор чата для FFXIV средствами библиотеки Deucalion, встроенной в АКТ
#include "pipe.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    
    Pipe pipe;
//    Message request = pipe.read();             // здороваемся с Девкалионом?
//    pipe.changeMode(PIPE_READMODE_MESSAGE);
//    if (!request.validServerInit()) {
//        std::cerr << "Received unknown message" << std::endl;
//        std::abort();
//    }

//    uint8_t filter[]{ 9, 0, 0, 0, 5, Filter::ChatR | Filter::ChatS, 0, 0, 0 };
//    Message initMsg{ filter, 9 };     // потому что статический метод не работает
    Message initMsg{Message::create_filter_message()}; // теперь работает!

    pipe.write(initMsg);

    Message reply = pipe.read();
    if (!reply.validFilterSetReply()) {
        std::cerr << "Received unknown message" << std::endl;
        std::abort();
    }

    pipe.loop();

    return 0;
}
