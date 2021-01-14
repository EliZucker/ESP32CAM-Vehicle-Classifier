# Flying Vehicle Classifier (ESP32-CAM)

<img align="left" width="300" src="demo_vid.gif" />

## Overview
This project aims to create a small, energy-efficient, standalone module that can classify several types of flying vehicles.

An ESP32-CAM microcontroller running this code will take a photo from the on-board rear camera, run inference via a local pre-trained model, and display the classification result on its OLED display. This sequence is triggered by a manual button press. Currently, the project is configured specifically for pinouts on the [LILYGO TTGO T-Camera ESP32](https://www.aliexpress.com/item/32968683765.html?spm=a219c.10010108.1000016.1.17c16021VtcUmV&isOrigTitle=true) (~$15).

## Features
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**-** &nbsp;Vehicle classification model is trained from scratch, based on the MobileNetV1 architecture, and further compressed with full integer quantization of weights.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**-** &nbsp;Completely local inference on the ESP32 platform.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**-** &nbsp;Results produced from the trained deep learning model via TensorFlow Lite Micro.

<br clear="left"/>

## Deploying
1. Install the ESP-IDF toolchain **v4.0** with their [Get Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html).
2. Activate the toolchain by running ```get_idf``` (if installed normally).
2. From the project directory, run 
```idf.py build``` .
3. With the target controller plugged in, run ```idf.py -p <your port> flash``` . If you are unsure which port to use, consult the guide linked in step 1.

## Training Model

See the [training_model](training_model/) directory.

## Known Issues

- The output classes are limited to the 5 options I chose (compatible with dataset). Can be improved.

- With the TTGO T-Camera ESP32 module, the OLED display driver seems to have I2C issues. I addressed this for now by repeatedly re-instantiating the connection. There are corresponding error messages in serial output, but functionality is fine.

- Due to ESP32's limited DRAM, the model network cannot get any wider, even with an increased application partition size.

- Accuracy is not great because of an inaccurate dataset (explained in training section).




## License / Credit

Due to Tensorflow Lite Micro's general lack of documentation, I consulted and used some functions from the [person_detection](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite/micro/examples/person_detection) example they provide. I have included the original license (Apache 2.0) for that project with modifications by me listed.