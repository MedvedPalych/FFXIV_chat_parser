#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <map>
#include <string>


enum Filter {
    LobbyR = 1,
    ZoneR = 2,
    ChatR = 4,
    LobbyS = 8,
    ZoneS = 16,
    ChatS = 32,
    Other = 64
};

const std::map<const uint16_t, const char> ruASCII {
    {53424, 'a' },
    {53425, 'б' },
    {53426, 'в' },
    {53427, 'г' },
    {53428, 'д' },
    {53429, 'е' },
    {53649, 'ё' },
    {53430, 'ж' },
    {53431, 'з' },
    {53432, 'и' },
    {53433, 'й' },
    {53434, 'к' },
    {53435, 'л' },
    {53436, 'м' },
    {53437, 'н' },
    {53438, 'о' },
    {53439, 'п' }, 
    {53632, 'р' },
    {53633, 'с' },
    {53634, 'т' },
    {53635, 'у' },
    {53636, 'ф' },
    {53637, 'х' },
    {53638, 'ц' },
    {53639, 'ч' },
    {53640, 'ш' },
    {53641, 'щ' },
    {53642, 'ъ' },
    {53643, 'ы' },
    {53644, 'ь' },
    {53645, 'э' },
    {53646, 'ю' },
    {53647, 'я' },

    {53392, 'А' },
    {53393, 'Б' },
    {53394, 'В' },
    {53395, 'Г' },
    {53396, 'Д' },
    {53397, 'Е' },
    {53377, 'Ё' },
    {53398, 'Ж' },
    {53399, 'З' },
    {53400, 'И' },
    {53401, 'Й' },
    {53402, 'К' },
    {53403, 'Л' },
    {53404, 'М' },
    {53405, 'Н' },
    {53406, 'О' },
    {53407, 'П' },
    {53408, 'Р' },
    {53409, 'С' },
    {53410, 'Т' },
    {53411, 'У' },
    {53412, 'Ф' },
    {53413, 'Х' },
    {53414, 'Ц' },
    {53415, 'Ч' },
    {53416, 'Ш' },
    {53417, 'Щ' },
    {53418, 'Ъ' },
    {53419, 'Ы' },
    {53420, 'Ь' },
    {53421, 'Э' },
    {53422, 'Ю' },
    {53423, 'Я' }
};

struct Message {
    std::vector<uint8_t> message;
    Message(std::vector<uint8_t>);

    void printString();
    void printBytes();
    
    std::string asString();
    std::string asBytes();

    bool validServerInit();
    bool validFilterSetReply();

    static Message initFilter() {
        uint8_t filter[]{ 9, 0, 0, 0, 5, Filter::ChatR | Filter::ChatS, 0, 0, 0 };
        std::vector<uint8_t> filterVector{ &(filter[0]), &(filter[0]) + 9 };
        return { filterVector };
    }

};

bool operator==(const Message& lhs, const Message& rhs);
