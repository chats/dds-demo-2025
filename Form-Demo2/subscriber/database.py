from sqlalchemy import create_engine, Column, Integer, String, DateTime
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from datetime import datetime

Base = declarative_base()

class MessageModel(Base):
    __tablename__ = 'messages'

    id = Column(Integer, primary_key=True)
    subject = Column(String)
    recipient = Column(String)
    fileUrl = Column(String)
    timestamp = Column(DateTime, default=datetime.utcnow)

engine = create_engine('sqlite:///messages.db')
Base.metadata.create_all(engine)
Session = sessionmaker(bind=engine)

