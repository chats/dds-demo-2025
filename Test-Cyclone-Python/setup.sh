#!/bin/bash

# Create directory structure
mkdir -p src/generated
touch src/__init__.py
touch src/generated/__init__.py

# Generate Python code from IDL
echo "Generating Python code from IDL..."
pushd src/generated
idlc -l py ../../idl/sensor_types.idl
popd

# Install dependencies
#echo "Installing dependencies..."
#pip install cyclonedds cyclonedds-python

echo "Setup complete! You can now run:"
echo "python src/subscriber.py  # in one terminal"
echo "python src/publisher.py   # in another terminal"