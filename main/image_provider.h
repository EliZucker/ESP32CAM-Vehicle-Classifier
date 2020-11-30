#ifndef FLYING_VEHICLE_CLASSIFIER_IMAGE_PROVIDER_H_
#define FLYING_VEHICLE_CLASSIFIER_IMAGE_PROVIDER_H_

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"

// This is an abstraction around an image source like a camera, and is
// expected to return 8-bit sample data.  The assumption is that this will be
// called in a low duty-cycle fashion in a low-power application.  In these
// cases, the imaging sensor need not be run in a streaming mode, but rather can
// be idled in a relatively low-power mode between calls to GetImage().  The
// assumption is that the overhead and time of bringing the low-power sensor out
// of this standby mode is commensurate with the expected duty cycle of the
// application.  The underlying sensor may actually be put into a streaming
// configuration, but the image buffer provided to GetImage should not be
// overwritten by the driver code until the next call to GetImage();
//
// The reference implementation can have no platform-specific dependencies, so
// it just returns a static image. For real applications, you should
// ensure there's a specialized implementation that accesses hardware APIs.
TfLiteStatus GetImage(tflite::ErrorReporter* error_reporter, int image_width,
                      int image_height, int channels, uint8_t* image_data);

#endif  // FLYING_VEHICLE_CLASSIFIER_IMAGE_PROVIDER_H_
