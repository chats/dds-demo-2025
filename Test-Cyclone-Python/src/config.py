from dataclasses import dataclass
from cyclonedds.idl import IdlStruct
from cyclonedds.core import Qos, Policy
from cyclonedds.util import duration

# DDS Configuration
DDS_DOMAIN_ID = 0
TOPIC_NAME = "SensorReadings"

# Define data type directly instead of using generated code
@dataclass
class SensorData(IdlStruct):
    device_id: str
    temperature: float
    humidity: float
    timestamp: int

# QoS Configurations
def create_reliable_qos():
    return Qos(
        Policy.Reliability.Reliable(duration(seconds=1)),
        Policy.History.KeepLast(10),
        Policy.Durability.Transient
    )

def create_best_effort_qos():
    return Qos(
        Policy.Reliability.BestEffort(),
        Policy.History.KeepLast(1),
        Policy.Durability.Volatile
    )