cmake_minimum_required(VERSION 3.10)
project(AirQualityDemo C)

find_package(CycloneDDS REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)

# Set explicit paths for libwebsockets
set(LIBWEBSOCKETS_LIBRARY "/opt/homebrew/lib/libwebsockets.dylib")
set(LIBWEBSOCKETS_INCLUDE_DIR "/opt/homebrew/include")

# Set explicit paths for jansson
set(JANSSON_LIBRARY "/opt/homebrew/lib/libjansson.dylib")
set(JANSSON_INCLUDE_DIR "/opt/homebrew/include")

idlc_generate(TARGET AirQualityData_lib FILES idl/AirQuality.idl)

add_executable(AirQualityDemoPublisher src/publisher.c)
target_link_libraries(AirQualityDemoPublisher CycloneDDS::ddsc AirQualityData_lib)

add_executable(AirQualityDemoSubscriber src/subscriber.c)
target_link_libraries(AirQualityDemoSubscriber CycloneDDS::ddsc AirQualityData_lib)

add_executable(web_subscriber src/web_subscriber.c)
target_include_directories(web_subscriber PRIVATE 
    ${LIBWEBSOCKETS_INCLUDE_DIR}
    ${JANSSON_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(web_subscriber 
    CycloneDDS::ddsc 
    AirQualityData_lib 
    ${LIBWEBSOCKETS_LIBRARY}
    ${JANSSON_LIBRARY}
    ${OPENSSL_LIBRARIES}
)