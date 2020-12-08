#include "tensorflow/lite/c/common.h"

#ifndef MATERIALS_CLASSIFIER_MAIN_FUNCTIONS_H_
#define MATERIALS_CLASSIFIER_MAIN_FUNCTIONS_H_

// Expose a C friendly interface for main functions.
#ifdef __cplusplus
extern "C" {
#endif

// Initializes all data needed for the example. The name is important, and needs
// to be setup() for Arduino compatibility.
void setup();

// Runs one iteration of data gathering and inference. This should be called
// repeatedly from the application code. The name needs to be loop() for Arduino
// compatibility.
void loop();

// Determine and display camera image label on OLED screen.
void RespondToDetection(TfLiteTensor* output);

#ifdef __cplusplus
}
#endif

#endif  // MATERIALS_CLASSIFIER_MAIN_FUNCTIONS_H_
