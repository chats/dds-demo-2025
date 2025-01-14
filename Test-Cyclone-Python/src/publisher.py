import time
import random
import argparse
from cyclonedds.domain import DomainParticipant
from cyclonedds.pub import Publisher, DataWriter
from cyclonedds.topic import Topic
from config import (
    DDS_DOMAIN_ID,
    TOPIC_NAME,
    SensorData,
    create_reliable_qos
)

class SensorPublisher:
    def __init__(self, device_id: str):
        self.device_id = device_id
        self.dp = DomainParticipant(DDS_DOMAIN_ID)
        self.pub = Publisher(self.dp)
        self.topic = Topic(self.dp, TOPIC_NAME, SensorData)
        self.writer = DataWriter(self.pub, self.topic, qos=create_reliable_qos())
        
        self.current_temp = 25.0
        self.current_humidity = 60.0

    def cleanup(self):
        if hasattr(self, 'writer'):
            del self.writer
        if hasattr(self, 'topic'):
            del self.topic
        if hasattr(self, 'pub'):
            del self.pub
        if hasattr(self, 'dp'):
            del self.dp

    def get_next_temperature(self):
        change = random.choice([-0.5, 0.5])
        new_temp = self.current_temp + change
        new_temp = max(20.0, min(30.0, new_temp))
        self.current_temp = new_temp
        return new_temp

    def calculate_humidity(self, temperature):
        base_humidity = 60.0
        temp_diff = temperature - 25.0
        new_humidity = base_humidity - (temp_diff * 2.0)
        new_humidity += random.uniform(-1.0, 1.0)
        new_humidity = max(40.0, min(80.0, new_humidity))
        self.current_humidity = new_humidity
        return new_humidity

    def publish_data(self):
        temperature = self.get_next_temperature()
        humidity = self.calculate_humidity(temperature)
        
        data = SensorData(
            device_id=self.device_id,
            temperature=round(temperature, 2),
            humidity=round(humidity, 2),
            timestamp=int(time.time())
        )
        self.writer.write(data)
        print(f"Published [{self.device_id}]: Temp={data.temperature}°C, Humidity={data.humidity}%")

def parse_arguments():
    parser = argparse.ArgumentParser(description='DDS Sensor Data Publisher')
    parser.add_argument('-i', '--id', 
                       default='sensor-001',
                       help='Device ID (default: sensor-001)')
    return parser.parse_args()

def main():
    args = parse_arguments()
    publisher = SensorPublisher(args.id)
    
    try:
        print(f"Starting publisher with device ID: {args.id}")
        while True:
            publisher.publish_data()
            time.sleep(1)
    except KeyboardInterrupt:
        print(f"\nStopping publisher [{args.id}]...")
    finally:
        publisher.cleanup()

if __name__ == "__main__":
    main()