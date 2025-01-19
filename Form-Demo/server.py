from flask import Flask, request, jsonify, render_template, send_from_directory
from flask_cors import CORS
from cyclonedds.domain import DomainParticipant
from cyclonedds.topic import Topic
from cyclonedds.pub import Publisher, DataWriter
from cyclonedds.idl import IdlStruct
from DocumentModule import Document  # Import generated IDL class

app = Flask(__name__)
CORS(app)

# Initialize DDS participant, publisher, and writer
dp = DomainParticipant()
pub = Publisher(dp)
topic = Topic(dp, "Document", Document)
writer = DataWriter(pub, topic)

@app.route('/')
def home():
    return send_from_directory('.', 'index.html')

@app.route('/submit', methods=['POST'])
def submit_document():
    try:
        data = request.json
        
        # Create Document instance using the generated IDL class
        doc = Document(
            recipient=data['recipient'],
            subject=data['subject'],
            url=data['url']
        )
        
        # Publish the document
        writer.write(doc)
        
        return jsonify({"message": "Document published successfully"}), 200
    
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001)