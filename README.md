techkeys
========

![image](http://cdn.shopify.com/s/files/1/0218/4886/products/Card_Front_large.png?v=1389032701)

Various things for the `atmega16u2` based techkeys business card

These source come from [this geekhack thread](http://geekhack.org/index.php?topic=53378.30)

+ The `techkeys_source` is from the creator of the card.
+ The `KeyCard-*` directory contains a completely different implementation (see post #34 of the thread for usage). This contains an AVR studio project which is untested. (I build via avr-gcc)

My goal:
+ Seperate most of the HID protocol to create a simple GUI configurator
+ Learn something about atmega16u2
+ Get a simple version of firmata working


To program the board in linux, you must plug it in, and JUMP the two pins next to the USB port via something metal, then:

```
$ sudo dfu-programmer atmega16u2 erase
$ sudo dfu-programmer atmega16u2 flash MYHEX.hex --debug-level=5
``` 


Generating the `.hex` file is usually as simple as converting the output binary to hex with `avr-objcopy`

```
$ arv-objcopy -O ihex a.out MYHEX.hex
```


