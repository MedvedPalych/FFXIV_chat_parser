#pragma once

#include <iostream>
#include <cstdint>

enum Filter {
    LobbyR = 1,
    ZoneR =  2,
    ChatR =  4,
    LobbyS = 8,
    ZoneS =  16,
    ChatS =  32,
    Other =  64
};

struct Message {
    uint8_t* message;
    uint32_t length;

    void print() {
        std::cout << "Length: " << length <<  " bytes" << std::endl;

        for (uint32_t i = 0; i < length; ++i) {
            std::cout << (unsigned short)message[i] << " ";
        }

        std::cout << std::endl;
    }

    static Message create_filter_message() {
        static uint8_t  data[9] {0};
        data[0] = 9;
        data[4] = 5;
        data[5] = Filter::ChatR | Filter::ChatS;
        return {data, 9};
    }

    bool validServerInit() {     // TODO
        return true;
    }

    bool validFilterSetReply() { // TODO
        return true;
    }
};
