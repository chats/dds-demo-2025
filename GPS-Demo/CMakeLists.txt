cmake_minimum_required(VERSION 3.10)
project(gps_demo C)

find_package(CycloneDDS REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)

# Set explicit paths for libwebsockets
set(LIBWEBSOCKETS_LIBRARY "/opt/homebrew/lib/libwebsockets.dylib")
set(LIBWEBSOCKETS_INCLUDE_DIR "/opt/homebrew/include")

# Set explicit paths for jansson
set(JANSSON_LIBRARY "/opt/homebrew/lib/libjansson.dylib")
set(JANSSON_INCLUDE_DIR "/opt/homebrew/include")


# Generate code from IDL
idlc_generate(TARGET GPSData_lib FILES idl/VehicleGPS.idl WARNINGS no-implicit-extensibility)

# Publisher executable
add_executable(GPSDemoPublisher src/publisher.c)
target_link_libraries(GPSDemoPublisher CycloneDDS::ddsc GPSData_lib)

# Subscriber executable
add_executable(GPSDemoSubscriber src/subscriber.c)
target_link_libraries(GPSDemoSubscriber CycloneDDS::ddsc GPSData_lib)

add_executable(web_subscriber src/web_subscriber.c)
target_include_directories(web_subscriber PRIVATE 
    ${LIBWEBSOCKETS_INCLUDE_DIR}
    ${JANSSON_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(web_subscriber 
    CycloneDDS::ddsc 
    GPSData_lib 
    ${LIBWEBSOCKETS_LIBRARY}
    ${JANSSON_LIBRARY}
    ${OPENSSL_LIBRARIES}
)