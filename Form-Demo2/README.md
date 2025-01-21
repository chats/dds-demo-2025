# DDS Form Demo2
Python project for DDS pub/sub using Cyclone DDS with web forms and database integration

### Features
1. Publisher
    - Web form for data (Subject, Recipient, FileURL)
    - Using Flask as web server
    - Send data trough DDS publisher
2. Subscriber
    - Receive data DDS
    - Save data into SQLite database
    - Show data on web
    - Filter recipient data
 
### Running
1. Install dependencies
```
python -m venv ~/.venv/cyclonedds
source ~/.venv/cyclonedds/bin/activate
python -m pip install -r requirements.txt
```
2. Run publisher
```
cd publisher
python app.py --port 8080
```
3. Run subscriber
```
cd subscriber
python app.py --port 8081 [--filter All|<Text>]
```