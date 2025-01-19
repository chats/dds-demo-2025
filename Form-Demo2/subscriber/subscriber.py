from cyclonedds.domain import DomainParticipant
from cyclonedds.sub import Subscriber, DataReader
from cyclonedds.topic import Topic
from cyclonedds.idl import IdlStruct
from dataclasses import dataclass
import time
from datetime import datetime

@dataclass
class Message(IdlStruct):
    subject: str
    recipient: str
    fileUrl: str
    timestamp: int

class DDSSubscriber:
    def __init__(self, filters=None):
        self.participant = DomainParticipant()
        self.subscriber = Subscriber(self.participant)
        self.topic = Topic(self.participant, "Message", Message)
        self.reader = DataReader(self.subscriber, self.topic)
        self.filters = filters if filters else ['*']
        
        if 'ALL' in self.filters or '*' in self.filters:
            print("Subscriber initialized: Accepting all messages")
        else:
            print(f"Subscriber initialized: Filtering for {self.filters}")

    def receive_messages(self, message_queue):
        while True:
            try:
                samples = self.reader.take()
                for sample in samples:
                    try:
                        if isinstance(sample, Message):
                            # Check if we should accept all messages or if the recipient is in our filter list
                            if ('ALL' in self.filters or '*' in self.filters or 
                                sample.recipient in self.filters):
                                message_data = {
                                    'subject': sample.subject,
                                    'recipient': sample.recipient,
                                    'fileUrl': sample.fileUrl,
                                    'timestamp': datetime.fromtimestamp(sample.timestamp).strftime('%Y-%m-%d %H:%M:%S')
                                }
                                print(f"Received message: {message_data['subject']} for {message_data['recipient']}")
                                message_queue.put(message_data)
                    except Exception as e:
                        print(f"Error processing sample: {e}")
                time.sleep(0.1)
            except Exception as e:
                print(f"Error taking samples: {e}")
                time.sleep(1)