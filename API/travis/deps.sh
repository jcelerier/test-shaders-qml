#!/bin/bash -eux

git submodule init

case "$TRAVIS_OS_NAME" in
  linux)
    sudo add-apt-repository --yes ppa:ubuntu-toolchain-r/test
    sudo add-apt-repository --yes ppa:beineri/opt-qt55
    sudo add-apt-repository --yes ppa:boost-latest/ppa

    sudo apt-get update -qq
    sudo apt-get install -qq qt55-meta-full libboost1.55-dev libportmidi-dev

    wget https://www.dropbox.com/s/3xot58gakn6w898/cmake_3.2.3-3.2.3_amd64.deb?dl=1 -O cmake_3.2.3-3.2.3_amd64.deb
    sudo dpkg --force-overwrite -i cmake_3.2.3-3.2.3_amd64.deb

    wget https://www.dropbox.com/s/zvfaiylxh6ecp0w/gcc_5.2.0-1_amd64.deb?dl=1 -O gcc.deb
    sudo dpkg --force-overwrite -i  gcc.deb

    wget https://www.dropbox.com/s/w0vngwurefrayg3/JamomaCore-0.6-dev-12.04-amd64-release.deb?dl=1 -O jamoma.deb
    sudo dpkg -i jamoma.deb

  ;;
  osx)
    # work around a homebrew bug
    set +e
    brew update; brew update 
    brew install wget
    wget https://www.dropbox.com/s/n3dsifakgzjbsnh/Jamoma-Darwin20150828.zip?dl=0 -O JamomaDarwin20150828.zip
    unzip JamomaDarwin20150828.zip
    mv JamomaDarwin20150828 Jamoma
    brew install cmake qt5 boost
    set -e
  ;;
esac
