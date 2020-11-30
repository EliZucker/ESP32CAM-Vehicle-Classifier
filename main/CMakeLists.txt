
#
# Main component of TF Micro project 'flight_vehicle_classifier'.
#

idf_component_register(
    SRCS esp/image_provider.cc esp/main.cc main_functions.cc model_settings.cc flight_vehicle_model_grayscale/flight_vehicle_model_data.cc esp/app_camera_esp.c
    INCLUDE_DIRS "")
