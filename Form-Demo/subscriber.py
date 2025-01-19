from cyclonedds.domain import DomainParticipant
from cyclonedds.topic import Topic
from cyclonedds.sub import Subscriber, DataReader
from cyclonedds.util import duration
from DocumentModule import Document  # Import generated IDL class
import time

# Initialize DDS participant
dp = DomainParticipant()
topic = Topic(dp, "Document", Document)
sub = Subscriber(dp)
reader = DataReader(sub, topic)

print("Waiting for documents...")

while True:
    # Read next sample
    sample = reader.read_next()
    if sample:
        print(f"\nReceived document:")
        print(f"Recipient: {sample.recipient}")
        print(f"Subject: {sample.subject}")
        print(f"URL: {sample.url}")
    time.sleep(0.1)  # Small delay to reduce CPU usage