#include "message.h"

Message::Message(std::vector<uint8_t> msg) : message(msg) {}

void Message::printString() { //TODO
    std::cout << "Length: " << message.size() << " bytes" << std::endl;

    std::cout << asString() << std::endl;
}

void Message::printBytes() { 
    std::cout << "Length: " << message.size() << " bytes" << std::endl;

    std::cout << asBytes() << std::endl;
}

std::string Message::asString() {
    std::string result;

    for (unsigned i = 0; i < message.size(); ++i) {

        if ((message[i] == 208 || message[i] == 209) && (i < message.size() - 1)) {
            try {
                result.push_back(ruASCII.at(uint16_t((message[i]) * 256) + message[i + 1]));
                ++i;
            }
            catch (const std::out_of_range& e) {
                std::cout << " SHIT HAPPENED " << e.what() << "  ";
                result += std::to_string(uint16_t(message[i]));
                result.push_back(' ');
            }
            continue;
        }
        if (i == 9) {
            result += "  |  ";
            result.push_back(' ');
        }

        if (i <= 8) {
           result += std::to_string(unsigned short(message[i]));
           result.push_back(' ');
        } else {
            result.push_back(char(message[i]));
            result.push_back(' ');
        }
    }

    return result;
}

std::string Message::asBytes() {
    std::string result;

    for (unsigned i = 0; i < message.size(); ++i) {
        result += std::to_string(unsigned short(message[i]));
        result.push_back(' ');
    }

    return result;
}

bool Message::validServerInit() {     
    std::vector<uint8_t> greeting = { 68, 0, 0, 0, 0, 40, 35, 0, 0,83, 69, 82, 86, 69, 82, 32, 72, 69, 76, 76, 79, 46,
     32, 72, 79, 79, 75, 32, 83, 84, 65, 84, 85, 83, 58, 32, 82, 69, 67, 86, 32, 79, 78, 46, 32, 83, 69, 78, 68, 32, 79,
     78, 46, 32, 83, 69, 78, 68, 95, 76, 79, 66, 66, 89, 32, 79, 78, 46};
    //68 0 0 0 0 40 35 0 0 |||||S E R V E R   H E L L O .   H O O K   S T A T U S :   R E C V   O N .   S E N D   O N .   S E N D _ L O B B Y   O N .
    const Message validGreeting{ greeting };
    return *this == validGreeting;
}

bool Message::validFilterSetReply() { 
    std::vector<uint8_t> filterSet = { 39, 0, 0, 0, 0, 0, 0, 0, 0, 80, 97, 99, 107, 101, 116, 32, 102, 105, 108, 116, 101,
    114, 115, 32, 115, 101, 116, 58, 32, 48, 98, 48, 48, 49, 48, 48, 49, 48, 48};
    //39 0 0 0 0 0 0 0 0 |||||P a c k e t   f i l t e r s   s e t :   0 b 0 0 1 0 0 1 0 0
    const Message validFilterSetReply{ filterSet };
    return *this == validFilterSetReply;
}

bool operator==(const Message& lhs, const Message& rhs) {
    return lhs.message == rhs.message;
}
