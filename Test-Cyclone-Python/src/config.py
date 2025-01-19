from dataclasses import dataclass
from enum import auto
from typing import TYPE_CHECKING, Optional

#from cyclonedds.idl import IdlStruct
from cyclonedds.core import Qos, Policy
from cyclonedds.util import duration


import cyclonedds.idl as idl
import cyclonedds.idl.annotations as annotate
import cyclonedds.idl.types as types


# DDS Configuration
DDS_DOMAIN_ID = 0
TOPIC_NAME = "SensorData"

# SensorData IDL
@dataclass
@annotate.final
@annotate.autoid("sequential")
class SensorData(idl.IdlStruct, typename="SensorData"):
    device_id: types.bounded_str[32]
    annotate.key("device_id")
    temperature: types.float64
    humidity: types.float64
    timestamp: types.int32



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