docker start tpe-arqui
docker exec -it tpe-arqui  make clean -C/root/x64barebones/Toolchain
docker exec -it tpe-arqui  make clean -C/root/x64barebones/
docker exec -it tpe-arqui  make -C/root/x64barebones/Toolchain
docker exec -it tpe-arqui make -C/root/x64barebones
docker stop tpe-arqui
