#!/usr/bin/env bash
export ATC=$PWD
git clone https://github.com/chenfengyuan/atc-bot-new.git
git clone https://github.com/chenfengyuan/atc-bot-py.git
git clone https://github.com/chenfengyuan/atc.git
git clone https://github.com/miloyip/rapidjson.git
# atc game
cd ${ATC}/atc
make
mkfifo atc_status action
#gtest
cd ${ATC}/
mkdir gtest
rsync -rtv /usr/src/gtest/ gtest/
cd gtest
cmake .
make
#atc-bot-new
cd ${ATC}/atc-bot-new
qmake
mkdir include
cd include
ln -s ../../rapidjson/include/rapidjson/ rapidjson
cd ..
cp ../gtest/libgtest.a .
make
cd ${ATC}/atc-bot-py
mkdir include
cd include
ln -s ../../rapidjson/include/rapidjson/
ln -s ../../atc-bot-new/ atc-bot
cd ..
qmake
make
