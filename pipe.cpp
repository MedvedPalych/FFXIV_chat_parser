#include "pipe.h"
#include "utils.h"
#include <stdio.h>
#include <string>

Pipe::Pipe(): _handle {INVALID_HANDLE_VALUE} {
  
    //std::string pipeName("\\\\.\\pipe\\deucalion-6448"); //TODO автоопределение процесса средствами винды, а не ручное забивание

    std::string pipeName("\\\\.\\pipe\\deucalion-");
    pipeName += std::to_string(utils::find_FFXIV_PID());
    while (1) {                             // TODO избавиться от цикла и наладить обработку ошибок
           _handle = CreateFile(
            pipeName.data(),                // pipe name 
            GENERIC_READ | GENERIC_WRITE,   // read and write access
            0,                              // no sharing 
            NULL,                           // default security attributes
            OPEN_EXISTING,                  // opens existing pipe 
            0,                              // default attributes 
            NULL);                          // no template file 

        if (_handle == INVALID_HANDLE_VALUE) {
            exit(1);
        }

        if (_handle != INVALID_HANDLE_VALUE)
            break;

        if (GetLastError() != ERROR_PIPE_BUSY) {
            std::cout << "Could not open pipe. GLE=" << GetLastError() << "\n";
            exit(2);
        }

        // Не получилось соединиться с пайпом за 20 секунд (как?) 
        if (!WaitNamedPipe(pipeName.data(), 20000)) {
            std::cout << "Could not open pipe: 20 second wait timed out.";
            exit(3);
        }
    } 
}

Message Pipe::read() {
    uint8_t  buf[512] {};
    uint32_t bytes_read = 0;

    // Read from the pipe. 
    BOOL fSuccess = ReadFile(
        _handle,             // pipe handle 
        buf,                 // buffer to receive reply 
        512,                 // size of buffer 
        LPDWORD(&bytes_read),// number of bytes read 
        NULL);               // not overlapped 

    if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
        exit(7);
    std::cout << "\nRead " << bytes_read << " bytes\n";

    Message message {buf, bytes_read};

    return message;
}

void Pipe::changeMode(DWORD Mode) {
    BOOL fSuccess = SetNamedPipeHandleState(
      _handle,   // pipe handle 
      &Mode,     // new pipe mode 
      NULL,      // don't set maximum bytes 
      NULL);     // don't set maximum time 
    if (!fSuccess)
    {
        std::cout<< "SetNamedPipeHandleState failed. GLE=" << GetLastError() << "\n";
        exit(5);
    }
}

void Pipe::write(Message message) {
    uint32_t bytes_written = 0;
    std::cout << "Sending " << message.length << " byte message:";
    for (uint32_t i = 0; i < message.length; ++i) {
        std::cout << (unsigned short)message.message[i] << " ";
    }
    std::cout << std::endl;

    BOOL fSuccess = WriteFile(
        _handle,                 // pipe handle 
        message.message,         // message 
        message.length,          // message length 
        LPDWORD(&bytes_written), // bytes written 
        NULL);                   // not overlapped 

    if (!fSuccess) {
        std::cout << "WriteFile to pipe failed. GLE=" << GetLastError() << "\n";
        exit(6);
    }
    
    std::cout << "Message sent to server\n";
}

void Pipe::loop() {
  for (;;) {
      Message message = read();
      message.print();
  }
}
