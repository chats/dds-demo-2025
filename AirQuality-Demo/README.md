# Bangkok Air Quality Monitor

Real-time air quality monitoring system for Bangkok and vicinity using DDS.

## Features
- Simulated AQI data for 50 Bangkok districts
- Real-time updates every 5 minutes
- Interactive web dashboard with color-coded markers
- District-based filtering capabilities

## Dependencies

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install libwebsockets-dev libjansson-dev libssl-dev cyclonedds-dev
```

### MacOS
```bash
brew install libwebsockets jansson openssl cyclonedds
```

### Windows
Using vcpkg:
```powershell
vcpkg install cyclonedds:x64-windows
vcpkg install libwebsockets:x64-windows
vcpkg install jansson:x64-windows
```

## Building
```bash
mkdir build && cd build
cmake ..
make
```

## CycloneDDS configuration
```
export CYCLONEDDS_URI=file://$PWD/cyclonedds.xml
```

## Running

1. Start Publisher:
```bash
./aqi_publisher
```

2. Start WebSocket Bridge:
```bash
./web_subscriber
```

3. Start Console Monitor (Optional):
```bash
./aqi_subscriber          # View all districts
./aqi_subscriber -d BKK01 # Filter specific district
```

4. Open `air_dashboard.html` in a web browser

## Architecture
- `publisher.c`: Simulates AQI data for each district
- `subscriber.c`: Console-based monitoring
- `web_subscriber.c`: WebSocket bridge for web dashboard
- `district_data.h`: District definitions and AQI calculations
- `air_dashboard.html`: Interactive web visualization

## Protocol
DDS Topic: "AQITopic"
WebSocket Port: 8080

## Directory Structure
```
.
├── CMakeLists.txt
├── idl/
│   └── AirQuality.idl
├── src/
│   ├── publisher.c
│   ├── subscriber.c
│   ├── web_subscriber.c
│   └── district_data.h
└── web/
    └── air_dashboard.html
```