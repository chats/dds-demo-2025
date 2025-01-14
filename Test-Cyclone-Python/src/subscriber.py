import time
import argparse
from dataclasses import dataclass
from cyclonedds.domain import DomainParticipant
from cyclonedds.sub import Subscriber, DataReader
from cyclonedds.topic import Topic
from config import (
    DDS_DOMAIN_ID,
    TOPIC_NAME,
    SensorData,
    create_reliable_qos
)

@dataclass
class SensorStats:
    count: int = 0
    temp_min: float = float('inf')
    temp_max: float = float('-inf')
    temp_sum: float = 0.0
    humid_min: float = float('inf')
    humid_max: float = float('-inf')
    humid_sum: float = 0.0
    last_stats_time: float = time.time()
    
    def update(self, temp: float, humid: float):
        self.count += 1
        # Temperature stats
        self.temp_min = min(self.temp_min, temp)
        self.temp_max = max(self.temp_max, temp)
        self.temp_sum += temp
        # Humidity stats
        self.humid_min = min(self.humid_min, humid)
        self.humid_max = max(self.humid_max, humid)
        self.humid_sum += humid
        
    def get_averages(self):
        if self.count == 0:
            return 0.0, 0.0
        return self.temp_sum / self.count, self.humid_sum / self.count
    
    def display(self):
        if self.count == 0:
            return "No data received yet"
            
        temp_avg, humid_avg = self.get_averages()
        return f"""
Statistics (samples: {self.count}):
Temperature:
  Min: {self.temp_min:.1f}°C
  Max: {self.temp_max:.1f}°C
  Avg: {temp_avg:.1f}°C
Humidity:
  Min: {self.humid_min:.1f}%
  Max: {self.humid_max:.1f}%
  Avg: {humid_avg:.1f}%
"""

class SensorSubscriber:
    def __init__(self, device_filter: str = None):
        self.device_filter = device_filter
        self.dp = DomainParticipant(DDS_DOMAIN_ID)
        self.sub = Subscriber(self.dp)
        self.topic = Topic(self.dp, TOPIC_NAME, SensorData)
        self.reader = DataReader(self.sub, self.topic, qos=create_reliable_qos())
        # สร้าง dict เก็บสถิติแยกตาม device_id
        self.stats = {}
        # กำหนดความถี่ในการแสดงสถิติ (วินาที)
        self.stats_interval = 5

    def get_device_stats(self, device_id: str) -> SensorStats:
        if device_id not in self.stats:
            self.stats[device_id] = SensorStats()
        return self.stats[device_id]

    def data_callback(self, data):
        # ถ้ามีการกำหนด filter และ device_id ไม่ตรงกับ filter ให้ข้าม
        if self.device_filter and data.device_id != self.device_filter:
            return
            
        print(f"\nReceived: Device ID: {data.device_id}")
        print(f"Temperature: {data.temperature}°C")
        print(f"Humidity: {data.humidity}%")
        print(f"Timestamp: {data.timestamp}")
        print("---")

        # อัพเดตสถิติ
        stats = self.get_device_stats(data.device_id)
        stats.update(data.temperature, data.humidity)
        
        # แสดงสถิติทุก stats_interval วินาที
        current_time = time.time()
        if current_time - stats.last_stats_time >= self.stats_interval:
            print(f"\nDevice {data.device_id} {stats.display()}")
            stats.last_stats_time = current_time

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
                    self.data_callback(data)
                time.sleep(0.1)
        except KeyboardInterrupt:
            print("\nStopping subscriber...")
            # แสดงสถิติสุดท้ายก่อนจบโปรแกรม
            for device_id, stats in self.stats.items():
                print(f"\nFinal statistics for device {device_id}:")
                print(stats.display())
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