from cyclonedds.domain import DomainParticipant
from cyclonedds.pub import Publisher, DataWriter
from cyclonedds.topic import Topic
from cyclonedds.idl import IdlStruct
from dataclasses import dataclass
import time

@dataclass
class Message(IdlStruct):
    subject: str
    recipient: str
    fileUrl: str
    timestamp: int

class DDSPublisher:
    def __init__(self):
        self.participant = DomainParticipant()
        self.publisher = Publisher(self.participant)
        self.topic = Topic(self.participant, "Message", Message)
        self.writer = DataWriter(self.publisher, self.topic)

    def publish_message(self, subject: str, recipient: str, fileUrl: str):
        message = Message(
            subject=subject,
            recipient=recipient,
            fileUrl=fileUrl,
            timestamp=int(time.time())
        )
        self.writer.write(message)