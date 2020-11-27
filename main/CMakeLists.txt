
#
# Main component of TF Micro project 'person_detection'.
#

idf_component_register(
    SRCS detection_responder.cc esp/image_provider.cc esp/main.cc main_functions.cc model_settings.cc person_model_grayscale/person_detect_model_data.cc esp/app_camera_esp.c
    INCLUDE_DIRS "")
