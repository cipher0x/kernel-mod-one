# kernel-mod-one

This is a kernel module that setups a char driver that returns all ones. It provides the sam functionality as /dev/zero except flips all bits

## Installation

make
sudo insmod one.ko

## Remove module
sudo rmmod one

## Usage
The driver device endpoint appears under:
/dev/one
