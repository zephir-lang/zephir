pyrus install pear/PHP_CodeSniffer
phpenv rehash
sudo apt-get -qq install re2c
git clone https://github.com/json-c/json-c
cd json-c
sh autogen. 2&>1 /dev/null
./configure --silent
make --silent
sudo make --silent install