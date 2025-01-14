import time
import argparse
import asyncio
import aiohttp
import json
from cyclonedds.domain import DomainParticipant
from cyclonedds.sub import Subscriber, DataReader
from cyclonedds.topic import Topic
from config import (
    DDS_DOMAIN_ID,
    TOPIC_NAME,
    SensorData,
    create_reliable_qos
)

class SensorSubscriber:
    def __init__(self, device_filter: str = None):
        self.device_filter = device_filter
        self.dp = DomainParticipant(DDS_DOMAIN_ID)
        self.sub = Subscriber(self.dp)
        self.topic = Topic(self.dp, TOPIC_NAME, SensorData)
        self.reader = DataReader(self.sub, self.topic, qos=create_reliable_qos())
        
    def send_to_dashboard(self, data_dict):
        """ส่งข้อมูลไปยัง web server ด้วย synchronous request"""
        import requests
        try:
            response = requests.post('http://localhost:8000/sensor_data', 
                                  json=data_dict,
                                  timeout=1)
            if response.status_code != 200:
                print(f"Error sending data: {response.status_code}")
        except Exception as e:
            print(f"Error sending data to dashboard: {e}")

    def process_data(self, data):
        # ถ้ามีการกำหนด filter และ device_id ไม่ตรงกับ filter ให้ข้าม
        if self.device_filter and data.device_id != self.device_filter:
            return
            
        # แปลงข้อมูลเป็น dict
        data_dict = {
            'device_id': data.device_id,
            'temperature': data.temperature,
            'humidity': data.humidity,
            'timestamp': data.timestamp
        }
        
        # ส่งข้อมูลไปยัง dashboard
        self.send_to_dashboard(data_dict)
        
        # แสดงข้อมูลใน console
        print(f"\nReceived: Device ID: {data.device_id}")
        print(f"Temperature: {data.temperature}°C")
        print(f"Humidity: {data.humidity}%")
        print(f"Timestamp: {data.timestamp}")
        print("---")

    def cleanup(self):
        if hasattr(self, 'reader'):
            del self.reader
        if hasattr(self, 'topic'):
            del self.topic
        if hasattr(self, 'sub'):
            del self.sub
        if hasattr(self, 'dp'):
            del self.dp

    def start_listening(self):
        if self.device_filter:
            print(f"Subscriber waiting for data from device: {self.device_filter}")
        else:
            print("Subscriber waiting for data from all devices...")
            
        try:
            while True:
                samples = self.reader.take()
                for data in samples:
                    self.process_data(data)
                time.sleep(0.1)
        except KeyboardInterrupt:
            print("\nStopping subscriber...")
        finally:
            self.cleanup()

def parse_arguments():
    parser = argparse.ArgumentParser(description='DDS Sensor Data Subscriber')
    parser.add_argument('-i', '--id',
                       help='Filter by Device ID (optional)')
    return parser.parse_args()

def main():
    args = parse_arguments()
    subscriber = SensorSubscriber(device_filter=args.id)
    
    try:
        subscriber.start_listening()
    except Exception as e:
        print(f"Error: {e}")
    finally:
        subscriber.cleanup()

if __name__ == "__main__":
    main()