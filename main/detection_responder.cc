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

#include "detection_responder.h"
#include "model_settings.h"
#include "ssd1306.h"

// This dummy implementation writes person and no person scores to the error
// console. Real applications will want to take some custom action instead, and
// should implement their own versions of this function.
void RespondToDetection(tflite::ErrorReporter* error_reporter,
                        uint8_t highest_scoring_class_index) {
    ssd1306_128x64_i2c_init();
    ssd1306_setFixedFont(ssd1306xled_font6x8);
    ssd1306_flipHorizontal(1);
    ssd1306_flipVertical(1);
    ssd1306_clearScreen();
    ssd1306_printFixedN (0, 16, kCategoryLabels[highest_scoring_class_index], STYLE_BOLD, FONT_SIZE_2X);

    // Hacky: manually add 2nd word to fighter jet/passenger plane labels:
    if (highest_scoring_class_index == 1) {
        ssd1306_printFixedN (0, 45, "Jet", STYLE_BOLD, FONT_SIZE_2X);
    }
    else if (highest_scoring_class_index == 4) {
        ssd1306_printFixedN (0, 45, "Plane", STYLE_BOLD, FONT_SIZE_2X);
    }
    TF_LITE_REPORT_ERROR(error_reporter, "Highest scoring class index: %d",
                        highest_scoring_class_index);
}
