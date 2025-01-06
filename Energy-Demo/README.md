# Energy Dashboard Demo

Real-time renewable energy monitoring system using DDS and WebSocket.

## Dependencies

### Linux (Ubuntu/Debian)
```bash
# Install CycloneDDS
git clone https://github.com/eclipse-cyclonedds/cyclonedds.git
cd cyclonedds
mkdir build && cd build
cmake ..
make
sudo make install

# Install WebSocket dependencies
sudo apt-get install libwebsockets-dev libjansson-dev libssl-dev
```

### MacOS
```bash
# Install CycloneDDS
git clone https://github.com/eclipse-cyclonedds/cyclonedds.git
cd cyclonedds
mkdir build && cd build
cmake ..
make
sudo make install

# Install WebSocket dependencies
brew install libwebsockets jansson openssl
```

### Windows
```powershell
# Install vcpkg if not installed
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg integrate install

# Install dependencies
vcpkg install cyclonedds:x64-windows
vcpkg install libwebsockets:x64-windows
vcpkg install jansson:x64-windows
vcpkg install openssl:x64-windows
```

## Building

### Linux/MacOS
```bash
mkdir build && cd build
cmake ..
make
```

### Windows
```powershell
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake ..
cmake --build .
```

## Running

1. Start the DDS publisher:
```bash
./publisher -v
```

2. Start the WebSocket subscriber:
```bash
./web_subscriber
```

3. Start a regular DDS subscriber (optional, for monitoring):
```bash
./subscriber -t wind    # Filter wind plants
./subscriber -t solar   # Filter solar plants
```

4. Open `dashboard.html` in a web browser to view real-time data

## Usage

### Publisher Options
- `-h` Show help message
- `-v` Enable verbose output
- `-s device_id` Set device ID
- `-d domain_id` Set DDS domain ID
- `-t topic` Set topic name

### Subscriber Options
- `-h` Show help message
- `-t type` Filter by plant type (wind/solar)
- `-s plant_id` Filter by specific plant ID
- `-d domain_id` Set DDS domain ID
- `-n topic` Set topic name