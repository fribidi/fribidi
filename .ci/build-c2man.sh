set -e

git clone https://github.com/fribidi/c2man.git
cd c2man

./Configure -dE
mkdir -p $(pwd)/c2man-install
echo "binexp=$(pwd)/c2man-install" >> config.sh
echo "installprivlib=$(pwd)/c2man-install" >> config.sh
echo "mansrc=$(pwd)/c2man-install" >> config.sh
sh config_h.SH
sh flatten.SH
sh Makefile.SH

make depend
make
make install
