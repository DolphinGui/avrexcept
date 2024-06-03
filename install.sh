#!/usr/bin/bash
set -ex
ARCHS="avr4 avr5 avr51 avr6 avrxmega2 avrxmega3 avrxmega4 avrxmega5 avrxmega6 avrxmega7"

for arch in $ARCHS
do
  make install ARCH=$arch PREFIX=$PREFIX/avr
done