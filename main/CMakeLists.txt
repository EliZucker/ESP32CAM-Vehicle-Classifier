
#
# Main component of TF Micro project 'flying_vehicle_classifier'.
#

idf_component_register(
    SRCS esp32_interfacing/image_provider.cc esp32_interfacing/main.cc main_functions.cc model_settings.cc flying_vehicle_model_grayscale/flying_vehicle_model_data.cc esp32_interfacing/app_camera_esp.c
    INCLUDE_DIRS "")
