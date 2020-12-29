# ESP-8266-LED-controller-and-alarm


## Table of contents
* [General info](#general-info)
* [Setup](#setup)
* [Features](#Features)
* [To-Do](#To Do)

## General info
Creates a website that can remote control LED to any color the user wishes and creates an alarm clock that simulates sunrise. The LED strip I used was the WS2812B with 60 LED's, if you have a different LED strip then you need to change the code. 
I also used the ESP 8266, if you use the ESP32 or any other board you need to modify the code. I use a 5V 3A power supply to power the board and the LED's.

## Setup
To run this project, make sure you have connected the LED data pin to the correct pin on the Board. If you have the ESp8266 and use the exact same code you will need to connect the data pin to D2. If you want to change the pin then remember that the number behind the D does not necesarily mean the number of the GPIO pin in your program. To check which numbers your pins have check on a datasheet of your microcontroller. (e.g. ESP8266 pins). If you want to change the time on which the sunrise starts you need to modify the number behind timeClient.getHours() in LED_clock. It uses only full hours because the sunrise takes 30 minutes

## Features

* Creates an alarm that goes of at a certain hour and simulates a sunrise over 30 minutes
* User can control LED's with a webserver and create any color with sliders

## To-Do
* Create preset colors (e.g. "warm orange")
* Improve design of websites
