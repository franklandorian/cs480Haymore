# Project 7 - Solar System :milky_way: :stars:
## Team Members - Kyle Respicio, Franklin Haymore, Ilya Kuznetsov

## Instructions

Firstly, you must be in the PA7 folder. Then run the following

```bash
mkdir build
cd build
cmake ..
make
./PA7 config.txt
```

To render different objects or use different shaders, change the flagged files in the config file.
To adjust speed, distance, size, etc. change the setting.txt in the assets folder.

## Controls

### While in focus view
Interaction Keys | Description
------------ | -------------
<kbd>0</kbd> | Zoom onto the Sun
<kbd>1-9</kbd> | Zoom onto the Specific Planet
<kbd>r</kbd> | Reset the view to the default
<kbd>f</kbd> | Change from focus view to free camera
<kbd>&uarr;</kbd> | Speed Up
<kbd>&darr;</kbd> | Speed Down


### Free camera view
Interaction Keys | Description
------------ | -------------
Mouse | Change camera view
<kbd>w</kbd> | Fly forward
<kbd>a</kbd> | Fly to the left
<kbd>s</kbd> | Fly backwards
<kbd>d</kbd> | Fly to the right
<kbd>e</kbd> | Fly down
<kbd>q</kbd> | Fly up
<kbd>f</kbd> | Change from free camera to focus view

### Extra Credit
* Ability to slow down/speed up time
* Config file
* Neptune and Uranus have rings
* Free roam camera
* Space is tethered to the Camera
