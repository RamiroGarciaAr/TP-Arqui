cd root
cd Toolchain
make clean all
cd ..
make clean all
./run.sh
qemu-system-x86_64 --version
docker exec <container-id> apt install pulseaudio
exit
cd root
cd Toolchain
make clean all
cd ..
make clean all
./run.sh
exit
