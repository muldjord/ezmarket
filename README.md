# ezmarket
EZMarket is a kid friendly market management system. It is purely meant for fun.

## Features
* Register customers, categories and items using a barcode scanner that acts as a keyboard (many scanners have a keyboard mode integrated).
* Keep track of the item lifespan. Expired items aren't sellable, so be sure to put them up with a discount before they go bad.
* Add voice announcements to the `sounds/soundboard` folder to use them with the integrated announcements soundboard.
* Checkout items when customers visit the market by scanning the items in the 'Checkout' tab. The software keeps track of customer money balance and bonus points. You decide what the bonus points should be used for.
* Open / close the store. Whenever you close it, all items will age by one day.

## EZMarket prerequisites
On a Ubuntu system you need to install the following packages before compiling EZMarket:
```
$ sudo apt install build-essential cmake qt6-base-dev qt6-tools-dev libsfml-dev
```

## Compiling EZMarket
The following commands will clone the EZMarket repository, compile the code and install it into a subdirectory called `release`:
```
$ git clone https://github.com/muldjord/ezmarket.git
$ cd ezmarket
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make install
```

## Starting EZMarket
From the build folder run the following commands to start EZMarket:
```
$ cd ../release
$ ./EZMarket
```

## Release notes

### Version 1.0.0 (04jun2022)
* Initial release