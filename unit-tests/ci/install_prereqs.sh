pyrus install pear/PHP_CodeSniffer
phpenv rehash
sudo apt-get -qq install re2c
git clone https://github.com/json-c/json-c
cd json-c
sh autogen.sh 2&>1 /dev/null
./configure --silent 2&>1 /dev/null
make --silent 2&>1 /dev/null
sudo make --silent install 2&>1 /dev/null