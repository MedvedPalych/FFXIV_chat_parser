#include <windows.h> 
#include "message.h"

class Pipe {
public:
  Pipe();

  Message read();
  void changeMode(DWORD Mode);
  void write(Message message);
  void loop();

private:
  HANDLE _handle;
};
