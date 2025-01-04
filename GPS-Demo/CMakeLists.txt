cmake_minimum_required(VERSION 3.10)
project(gps_demo C)

if(NOT TARGET CycloneDDS::ddsc)
  # Find the CycloneDDS package.
  find_package(CycloneDDS REQUIRED)
endif()


# Generate code from IDL
idlc_generate(TARGET GPSData_lib FILES idl/VehicleGPS.idl WARNINGS no-implicit-extensibility)

# Publisher executable
add_executable(GPSDemoPublisher src/publisher.c)
target_link_libraries(GPSDemoPublisher CycloneDDS::ddsc GPSData_lib)

# Subscriber executable
add_executable(GPSDemoSubscriber src/subscriber.c)
target_link_libraries(GPSDemoSubscriber CycloneDDS::ddsc GPSData_lib)