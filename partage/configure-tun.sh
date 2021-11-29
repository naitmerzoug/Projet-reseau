#!/bin/bash


ip addr add $1/255.255.255.0 dev $4

ip link set $4 up

ip route add $2 via 172.16.1.10

ip route add default via $3


