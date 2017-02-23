cd tools
mkdir cross
cd cross

# GCC & binutils
wget "ftp://ftp.gnu.org/gnu/binutils/binutils-2.27.tar.gz"
wget "ftp://ftp.gnu.org/gnu/gcc/gcc-6.3.0/gcc-6.3.0.tar.gz"

# Others
wget "https://ftp.gnu.org/gnu/gmp/gmp-6.1.2.tar.xz"
wget "http://www.mpfr.org/mpfr-current/mpfr-3.1.5.tar.gz"
wget "http://isl.gforge.inria.fr/isl-0.18.tar.gz"
wget "http://www.bastoul.net/cloog/pages/download/count.php3?url=./cloog-0.18.4.tar.gz"
wget "ftp://ftp.gnu.org/gnu/mpc/mpc-1.0.3.tar.gz"
wget "https://ftp.gnu.org/gnu/texinfo/texinfo-6.3.tar.gz"

echo "Extracting compiler utilities..."
for f in *.tar.gz ; do
	echo "Extracting $f..."
	tar -xzf "$f";
done
for f in *.tar.xz ; do
	echo "Extracting $f..."
	tar --xz -xf "$f";
done
echo "Done!"

WD=`pwd`
mkdir bin
export PREFIX="$WD"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# binutils
mv gmp-6.1.2 gcc-6.3.0/gmp
mv mpfr-3.1.5 gcc-6.3.0/mpfr
mv mpc-1.0.3 gcc-6.3.0/mpc

mkdir build-binutils
cd build-binutils

../binutils-2.27/configure --target="$TARGET" --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

cd ..

# GCC
mv gmp-6.1.2 gcc-6.3.0/gmp
mv mpfr-3.1.5 gcc-6.3.0/mpfr
mv mpc-1.0.3 gcc-6.3.0/mpc

mkdir build-gcc
cd build-gcc

../gcc-6.3.0/configure --target="$TARGET" --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

cd ..
