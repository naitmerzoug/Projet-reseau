#!/bin/bash

VBoxManage setproperty machinefolder "/tmp"

cd ../VM1-4
vagrant up
cd ../VM1-6
vagrant up
cd ../VM2-6
vagrant up
cd ../VM3-6
vagrant up
cd ../VM3-4
vagrant up
