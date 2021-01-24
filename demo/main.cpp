// Copyright 2021 LemuriiL <LemuriiL@yandex.ru>
#include <Bloodhound.hpp>

int main(int argc, char *argv[]) {
  std::string directoryToCheck;
  if (argc == 1) {
    directoryToCheck = ".";
  } else {
    directoryToCheck = argv[1];
  }

  Brain open("../misc/ftp");
  open.takeBrokers();
  open.Print();
  return 0;
}