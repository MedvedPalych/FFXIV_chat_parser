#pragma once
#include <windows.h>
#include <fstream>
#include "message.h"

class Pipe {
public:
  Pipe();
  ~Pipe();

  Message read();
  void changeMode(DWORD Mode);
  void write(Message message);
  void printMessageToFile(Message message);
  void loop();

private:
  std::ofstream fout;
  HANDLE _handle;
};