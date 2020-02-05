ls
make
make
make
make
./obj64/threads.exe -n 1048576 -b counter -t 1
./obj64/threads.exe -n 1048576 -b counter -t 2
for t in 1 2; do for trial in `seq 5`; do ./obj64/threads.exe -n 1048576 -b counter -t$t; done; done;
make
make
make
make
make
make
cls
clear
make
reset
make
make
make
./obj64/crypto_aes.exe
./obj64/crypto_aes.exe -a
./obj64/crypto_aes.exe --help
xit
exit
