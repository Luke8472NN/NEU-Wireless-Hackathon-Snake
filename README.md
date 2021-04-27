# NEU-Wireless-Hackathon-Snake
Northeastern University Wireless Club 11/11/16 Hackathon Snake Game.

Copied from my old profile, https://github.com/Luke8472

Eternally grateful for https://github.com/devendrashivraj for the board design!

## Description
A Snake Game coded in Arduino and uploaded to an Arduino Uno. A PCB was designed as a shield for the Arduino Uno, and to house all electronic components.

<img src="https://user-images.githubusercontent.com/6588879/32711837-0ff9953c-c80f-11e7-8d23-fd53238651b2.jpg" width="400" height="509"> <img src="https://user-images.githubusercontent.com/6588879/32713508-89c0caea-c817-11e7-9fde-f6d1d716a945.gif" width="400" height="509">

## How it Works
1) Turn the game on, using the power switch located in the upper left corner of the PCB.
2) Press the joystick once, to start playing the game.
3) The direction of the snake can be controlled using the joystick.
4) The speed of the snake can be controlled using the potentiometer located to the left of the joystick.
5) The option for walls on or off can be controlled by the switch below the potentiometer. </br>
   a) If the walls are on, the user loses if the snake hits the edge of the display. </br>
   b) If the walls are off, the snake can go out one edge, and come in through the other (world wraps around).
6) The score is displayed above the joystick, and represents the number of candies eaten by the snake.
7) If the game is left idle for more than 15 seconds, a Snake AI will begin playing the game.

## Goal of the Game
Eat all of the candy without having the snake cross over itself. Once the snake grows to the size of the display (score = 128), the user wins.

## Prerequisites
LedControl library: https://github.com/wayoda/LedControl

## Board Design
<img src="https://user-images.githubusercontent.com/6588879/32715439-5ac8356c-c820-11e7-9790-e499dbc9216d.jpg" width="800" height="544">
