# Prebuild Versions
## Windows
  run  
  > prebuild/windows10/ulak.exe
## Linux (only tested on Ubuntu-24.04)
  use launch instead of ulak, needed to set LD_LIBRARY_PATH, also sets backend x11 instead of wayland, wayland backend has issues with weird window borders and stay on top hints
  > prebuild/ubuntu24.04/launch


# Compile
## Windows
  Download QT Creator use
  > src/ulak.pro
  file to create projects
## Linux
  in Debian
  > sudo apt install build-essential qt6-base-dev qt6-serialport-dev qtcreator
  qt6 creator verison 13.0.0
  qt6 version 6.4.2
