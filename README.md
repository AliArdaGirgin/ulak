# Ulak
Serial communication testing tool
![image](https://github.com/user-attachments/assets/bea18d40-479b-4fc5-8a58-9114ef34c2d7)
# Prebuild Versions
## Windows
  run  
  > prebuild/windows10/ulak.exe
## Linux (only tested on Ubuntu-24.04)
  use launch instead of ulak, needed to set LD_LIBRARY_PATH, also sets backend x11 instead of wayland, wayland backend has issues with weird window borders and stay on top hints, need to be root to access tty
  > sudo prebuild/ubuntu24.04/launch


# Build
## Windows
  Download QT Creator and C++ compiler use src/ulak.pro in;
  > src/ulak.pro

## Linux
  in Debian
  > sudo apt install build-essential qt6-base-dev qt6-serialport-dev qtcreator

  > src/ulak.pro

  Tested on;
  Ubuntu 24.04
  qt6 creator verison 13.0.0
  qt6 version 6.4.2
