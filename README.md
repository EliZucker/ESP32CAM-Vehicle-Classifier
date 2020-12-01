# ESP32 Flying Vehicle Classifier

<img align="left" width="340" src="demo_vid.gif" />

## Overview
The goal of this project was to create a small, low-power, standalone module, that can classify several types of flying vehicles.

An ESP32-CAM microcontroller running this code will take a photo with the on-board rear camera when a button is pressed, run inference via a local pre-trained model, and display the classification result on the on-board OLED display. Currently, the project is specifically configured for pinouts on the [LILYGO TTGO T-Camera ESP32](https://www.aliexpress.com/item/32968683765.html?spm=a219c.10010108.1000016.1.17c16021VtcUmV&isOrigTitle=true) (~$15).

## Features
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**-** &nbsp;Completely local inference on the ESP32 platform

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**-** &nbsp;Results produced from a deep learning model via Tensorflow Lite Micro

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**-** &nbsp;Core vehicle classification model trained from scratch, based on the MobileNetV1 architecture

<br clear="left"/>

## Deploying
test

```
get_idf
idf.py build
```