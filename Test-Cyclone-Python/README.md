# Test Python binding of CycloneDDS

## Setting up
To use python binding on MacOS Arm you have to
1. Install pyenv (https://github.com/pyenv/pyenv) to select proper version for cyclonedds.
2. Install python using pyenv (for example 3.11.2)
```
pyenv install 3.11.2
pyenv local 3.11.2
```
3. Build and install cyclonedds
```
git clone https://github.com/eclipse-cyclonedds/cyclonedds
cd cyclonedds && mkdir build install && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install
cmake --build . --config RelWithDebInfo --target install
```
4. Set environment of cyclonedds
```
export CYCLONEDDS_HOME="$(pwd)/install"
```
5. Install cyclonedds-python (using python 3.11.2) from source.
```
python -m pip install git+https://github.com/eclipse-cyclonedds/cyclonedds-python
```

## Running
1. Create your idl
2. Generate data type from idl
```
mkdir generated
idlc -l py idl/SensorTypes.idl -d generated
```
3. Run publisher or subscriber
```
source ~/.venv/myenv/bin/activate
python src/pubisher.py
python src/suscriber.py
```