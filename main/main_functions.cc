/* Modifications copyright (C) 2020 Eli Zucker */

/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "main_functions.h"

#include "image_provider.h"
#include "model_settings.h"
#include "person_detect_model_data.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "driver/gpio.h"
#include "ssd1306.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON_BIT_MASK (uint64_t)1<<34
#define BUTTON_GPIO_NUM GPIO_NUM_34

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;

// An area of memory to use for input, output, and intermediate arrays.
constexpr int kTensorArenaSize = 152 * 1024;
static uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() {
  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_person_detect_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // Pull in only the operation implementations we need.
  // This relies on a complete list of all the ops needed by this graph.
  // An easier approach is to just use the AllOpsResolver, but this will
  // incur some penalty in code space for op implementations that are not
  // needed by this graph.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroMutableOpResolver<6> micro_op_resolver(error_reporter);
  micro_op_resolver.AddAveragePool2D();
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddDepthwiseConv2D();
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddMean();
  micro_op_resolver.AddPad();

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Get information about the memory area to use for the model's input.
  input = interpreter->input(0);

  // Set up GPIO for single button.
  gpio_config_t io_conf;
  io_conf.mode = GPIO_MODE_INPUT;
  io_conf.pin_bit_mask = BUTTON_BIT_MASK;
  io_conf.pull_up_en = (gpio_pullup_t) 1;
  gpio_config(&io_conf);

  // Clear OLED.
  ssd1306_128x64_i2c_init();
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_clearScreen();
}

void loop() {
  // Wait for button press. Poll every 50ms.
  vTaskDelay(50 / portTICK_PERIOD_MS);
  if (!gpio_get_level(BUTTON_GPIO_NUM)) {
    // Capture image immediately following button press.
    uint8_t image_data[kMaxImageSize];
    if (kTfLiteOk != GetImage(error_reporter, kNumCols, kNumRows, kNumChannels,
                              image_data)) {
      TF_LITE_REPORT_ERROR(error_reporter, "Image capture failed.");
    }

    // Manually convert uint8 pixel data to scaled int8 for quantized network input.
    for (int i = 0; i < (kMaxImageSize); ++i) {
      int pixel_val = (int) image_data[i];
      input->data.int8[i] = (int8_t) (pixel_val - 128);
    }

    // Run the model on this input and make sure it succeeds.
    if (kTfLiteOk != interpreter->Invoke()) {
      TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.");
    }

    // Process the inference results.
    TfLiteTensor* output = interpreter->output(0);
    RespondToDetection(output);
  }
}

void RespondToDetection(TfLiteTensor* output) {
  // Find index of label with highest score.
  int8_t highest_score = output->data.int8[0];
  int highest_scoring_class_index = 0;
  for (int i = 1; i < kCategoryCount; ++i) {
    if (output->data.int8[i] >= highest_score) {
      highest_score = output->data.int8[i];
      highest_scoring_class_index = i;
    }
  }

  // Display highest scoring label on OLED display.
  ssd1306_128x64_i2c_init();
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_clearScreen();
  ssd1306_printFixedN (0, 16, kCategoryLabels[highest_scoring_class_index], STYLE_BOLD, FONT_SIZE_2X);

  // Manually add 2nd word to fighter jet/passenger plane labels special case.
  if (highest_scoring_class_index == 1) {
      ssd1306_printFixedN (0, 45, "Jet", STYLE_BOLD, FONT_SIZE_2X);
  }
  else if (highest_scoring_class_index == 3) {
      ssd1306_printFixedN (0, 45, "Plane", STYLE_BOLD, FONT_SIZE_2X);
  }
}