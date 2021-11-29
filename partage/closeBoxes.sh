#!/bin/bash

cd ../VM1-4
vagrant halt
vagrant destroy --force

cd ../VM1-6
vagrant halt
vagrant destroy --force

cd ../VM2-6
vagrant halt
vagrant destroy --force

cd ../VM3-6
vagrant halt
vagrant destroy --force

cd ../VM3-4
vagrant halt
vagrant destroy --force
