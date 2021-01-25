# semtech_udp_packet_emitter

Simple program to send UPD packets

## Example

```
semtech-udp-packet-emitter 84.237.104.128:2000
./semtech-udp-packet-emitter 84.237.104.128:2000 --frame-counter 1 | nc -u -w 0 84.237.104.128 1700
./semtech-udp-packet-emitter 84.237.104.128:1700 | nc -u -w 0 84.237.104.128 1700
```


```
./chirpstack-ns-log -d 1122334455667788
```

## Build

automake:

```
autogen.sh
./configure
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

Когда netrwork server получает пакет от базовой станции, вызываются:

GetDeviceProfile
GetDevice
GetDeviceProfile
GetDeviceActivation
GetDeviceQueueItemsForDevEUI
GetDeviceActivation
GetGateway
GetGatewayProfile

## Chirpstack

```
cs-app lsdev
cs-dev lsapp
cs-dev 
```
