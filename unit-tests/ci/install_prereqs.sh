- sudo apt-get -qq install re2c
git clone https://github.com/json-c/json-c
cd json-c
sh autogen.sh
./configure --silent
make --silent
sudo make --silent install