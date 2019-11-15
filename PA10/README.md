# Project 10 - Pinball:
## Team Members - Kyle Respicio, Franklin Haymore, Ilya Kuznetsov

## Instructions
Firstly, you must be in the PA10 folder. Then run the following

```bash
mkdir build
cd build
cmake ..
make
./PA10 config.txt
```

## Controls
### Lighting Controls
Interaction Keys | Description
------------ | -------------
<kbd>&uarr;</kbd> | Increase Ambient Lighting
<kbd>&darr;</kbd> | Decrease Ambient Lighting
<kbd>&larr;</kbd> | Increase Specular Lighing
<kbd>&rarr;</kbd> | Decrease Specular Lighing
<kbd>z</kbd> | Toggle between per-vertex and per-fragment lighting
<kbd>p</kbd> | Change color of the board

### Game Controls
Interaction Keys | Description
------------ | -------------
<kbd>Space</kbd> | Hold to Launch the Ball
<kbd>z</kbd> | Left Flipper
<kbd>m</kbd> | Right Flipper
<kbd>r</kbd> | Restart the Game
<kbd>w</kbd> | Move the pinball up (For debugging)
<kbd>a</kbd> | Move the pinball left (For debugging)
<kbd>s</kbd> | Move the pinball down (For debugging)
<kbd>d</kbd> | Move the pinball right (For debugging)

### Camera Controls
Interaction Keys | Description
------------ | -------------
<kbd>1</kbd> | Left View
<kbd>2</kbd> | Right View
<kbd>3</kbd> | Above View
<kbd>0</kbd> | Default View

## Extra Credit:
* KDB Plunger Intensity
* Replayability
* Rainbow color filters
