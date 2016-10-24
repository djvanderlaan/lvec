
wget https://sourceforge.net/projects/boost/files/boost/1.62.0/boost_1_62_0.tar.gz

tar -xzf boost_1_62_0.tar.gz

cd boost_1_62_0

sudo apt-get install libbz2-dev

./bootstrap.sh --with-libraries=filesystem,iostreams \
  --prefix=$PWD/../boost

./b2

./b2 install






