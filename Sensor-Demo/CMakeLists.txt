cmake_minimum_required(VERSION 3.10)
project(sensor_demo C)

if(NOT TARGET CycloneDDS::ddsc)
  # Find the CycloneDDS package.
  find_package(CycloneDDS REQUIRED)
endif()


# Generate code from IDL
idlc_generate(TARGET SensorData_lib FILES idl/SensorData.idl WARNINGS no-implicit-extensibility)

# Publisher executable
add_executable(SensorDemoPublisher src/opt/publisher.c)
target_link_libraries(SensorDemoPublisher CycloneDDS::ddsc SensorData_lib)

# Subscriber executable
add_executable(SensorDemoSubscriber src/opt/subscriber.c)
target_link_libraries(SensorDemoSubscriber CycloneDDS::ddsc SensorData_lib)