# Draw-that-Shape
**Draw that Shape** is an interactive learning game targeted toward children learning their shapes.

## Description
This game runs on a BeagleBone Black (BBB) running Debian GNU/Linux 11 (bullseye) and requires extra peripherals such as a usb webcam, LEDs, and pushbutton.

## [Final Video](https://youtu.be/FPcHXyPLIwY)

## Installation
Update package manager and install OpenCV
```
sudo apt-get update
sudo apt-get install libopencv-dev
```

Clone the repository.
```
cd Draw-that-Shape
```

Build executables by going into `km` and `ul` directories in the repository and using the command
```
make
```

Copy executables to home directory from root directory of repository.
```
cp km/myshape.ko ul/test ~
```

Setup automatic game startup on BBB boot.
You may need to edit run_shape.sh depending on name of home directory.
```
cp /setup/run_shape.sh /usr/local/bin
```
```
cp /setup/shape.service /etc/systemd/system
```
Reload systemd to recognize the new service.
```
sudo systemctl daemon-reload
```
Enable the game service at startup.
```
sudo systemctl enable shape.service
```
Reboot BBB

## Circuit Diagram
![732F116A-7606-4AE5-A4CF-58EC9677B7B3_1_201_a](https://github.com/eth1030/Draw-that-Shape/assets/98787002/60a3cf91-d365-4bee-a45e-6dd15d38a7e3)

## How to Play
To start the game, the player pushes the push button, and has 10 seconds to draw a shape denoted by the blue LEDs. The number corresponds to the following shapes:
1. circle
2. triangle
3. rectangle

At the end of the 10 seconds, the player will find out if they drew the correct shape. The red and green LED represent the outcome:
- Green: Correct
- Red: Incorrect

Now if the player wants to play another round, they can press the button again which will start the 10 second timer.

## Authors 
Emika Hammond  <eth@bu.edu>

Shamir Legaspi <slegaspi@bu.edu>

and a big thanks to our Embedded System Professor Babak Kia.
