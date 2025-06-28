#! /bin/sh

cd work/sound_driver/
make
cd ../../
cp work/sound_driver/adp68k.bin cd/ADP68K.BIN
make
