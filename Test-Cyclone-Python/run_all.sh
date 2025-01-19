#!/bin/bash

source ~/.venv/cyclonedds/bin/activate


# Run web server
python src/web_server.py &

# Run the subscriber
python src/web_subscriber.py &

# Run the publisher
python src/publisher.py -i Sensor-001 &
python src/publisher.py -i Sensor-002 &
