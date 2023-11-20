#include "pipe.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <string>

constexpr auto BUFSIZE = 2048;

Pipe::Pipe() : _handle{ INVALID_HANDLE_VALUE } {

    std::string pipeName("\\\\.\\pipe\\deucalion-");

    pipeName += std::to_string(utils::find_FFXIV_PID());

    time_t now;
    time(&now);
    struct tm* local = localtime(&now);
    std::string name = std::to_string(local->tm_mday) + std::to_string(local->tm_mon + 1) +
        std::to_string(local->tm_year + 1900) + "_" + std::to_string(local->tm_hour)
        + std::to_string(local->tm_min) + std::to_string(local->tm_sec) + ".txt";
    fout.open(name);
    if (!fout.is_open()) {
        std::cout << "Failed to create file " << name << std::endl;
    }

    std::cout << "Connecting @ " <<  pipeName << "\n";

    for (;;) {
        _handle = CreateFile(
            pipeName.data(),                // pipe name 
            GENERIC_READ | GENERIC_WRITE,   // read and write access
            0,                              // no sharing 
            NULL,                           // default security attributes
            OPEN_EXISTING,                  // opens existing pipe 
            0,                              // default attributes 
            NULL);                          // no template file 

        if (_handle == INVALID_HANDLE_VALUE) {
            std::abort();
        } else {
            break;
        }

        if (GetLastError() != ERROR_PIPE_BUSY) {
            std::cout << "Could not open pipe. GLE=" << GetLastError() << "\n";
            std::abort();
        }

        if (!WaitNamedPipe(pipeName.data(), 20000)) {
            std::cout << "Could not open pipe: 20 second wait timed out.";
            std::abort();
        }
    }
}

Pipe::~Pipe() {
    fout.close();
}

Message Pipe::read() {
    uint8_t  buf[BUFSIZE] {};
    uint32_t bytes_read = 0;

    BOOL fSuccess = ReadFile(
        _handle,              // pipe handle 
        buf,                  // buffer to receive reply 
        BUFSIZE,              // size of buffer 
        LPDWORD(&bytes_read), // number of bytes read 
        NULL);                // not overlapped 

    if (!fSuccess && GetLastError() != ERROR_MORE_DATA) {
        exit(7);
    }
       
    std::cout << bytes_read << " bytes read.\n";

    std::vector<uint8_t> received {&(buf[0]), &(buf[0]) + bytes_read};
    Message message {received};

    return message;
}

void Pipe::changeMode(DWORD Mode) {
    BOOL fSuccess = SetNamedPipeHandleState(
      _handle,   // pipe handle 
      &Mode,     // new pipe mode 
      NULL,      // don't set maximum bytes 
      NULL);     // don't set maximum time 

    if (!fSuccess) {
        std::cout<< "SetNamedPipeHandleState failed. GLE=" << GetLastError() << "\n";
        std::abort();
    }
}

void Pipe::write(Message sMsg) {
    uint32_t bytes_written = 0;

    std::cout << "Sending " << sMsg.message.size() << " byte message:" << std::endl;

    for (uint8_t element: sMsg.message) {
        std::cout << (unsigned short)element << " ";
    }
    std::cout << std::endl;

    BOOL fSuccess = WriteFile(
        _handle,                 // pipe handle 
        sMsg.message.data(), // message 
        sMsg.message.size(),  // message length 
        LPDWORD(&bytes_written), // bytes written 
        NULL);                   // not overlapped 

    if (!fSuccess) {
        std::cout << "WriteFile to pipe failed. GLE=" << GetLastError() << std::endl;
        std::abort();
    }
    
    std::cout << "Message sent to server" << std::endl;
}

void Pipe::printMessageToFile(Message message) {
    if (fout.is_open()) {
        fout << message.asString() << std::endl;
    }
    else {
        std::cerr << "Couldn't write message to file" << std::endl;
    }
}

void Pipe::loop() {
    for (;;) {
        Message message = read();
        message.printString();
        printMessageToFile(message);
    }
}
