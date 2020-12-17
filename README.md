# semtech_udp_packet_emitter

Simple program to send UPD packets

## Example

```
semtech-udp-packet-emitter 84.237.104.128:2000
```

## Build

automake:

```
autogen.sh
./confiugure
make
sudo make install
```

cmake (Clang):

```
mkdir build
cd build
export CC=/usr/bin/clang;export CXX=/usr/bin/clang++;cmake ..
make
```
