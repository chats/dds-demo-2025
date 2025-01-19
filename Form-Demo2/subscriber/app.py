from flask import Flask, render_template, request, Response
from subscriber import DDSSubscriber
import threading
import queue
import json
import argparse
import requests
import pandas as pd
import io
from collections import deque

app = Flask(__name__)
message_queue = queue.Queue()
messages_cache = deque(maxlen=100)

def fetch_excel_data(file_url):
    try:
        response = requests.get(file_url)
        if response.status_code == 200:
            df = pd.read_excel(io.BytesIO(response.content))
            return {
                'headers': df.columns.tolist(),
                'rows': df.values.tolist()
            }
        return None
    except Exception as e:
        print(f"Error fetching Excel file: {e}")
        return None

def format_message(message_data):
    excel_data = fetch_excel_data(message_data['fileUrl']) if message_data['fileUrl'] else None
    return {
        'subject': message_data['subject'],
        'recipient': message_data['recipient'],
        'fileUrl': message_data['fileUrl'],
        'timestamp': message_data['timestamp'],
        'excel_data': excel_data
    }

@app.route('/')
def index():
    global messages_cache, filters
    if 'ALL' in filters or '*' in filters:
        filtered_messages = list(messages_cache)
    else:
        filtered_messages = [msg for msg in messages_cache 
                           if msg['recipient'] in filters]
    
    return render_template('messages.html', 
                         messages=filtered_messages, 
                         current_filters=filters)

@app.route('/stream')
def stream():
    def event_stream():
        while True:
            try:
                message = message_queue.get(timeout=1)
                if message:
                    data = format_message(message)
                    messages_cache.appendleft(data)
                    yield f"data: {json.dumps(data)}\n\n"
            except queue.Empty:
                yield ": keepalive\n\n"
            except Exception as e:
                print(f"Streaming error: {e}")
                continue
    return Response(event_stream(), mimetype="text/event-stream")

def parse_filters(filter_str):
    if not filter_str:
        return ['*']  # Default to accept all if no filter specified
    return [f.strip() for f in filter_str.split(',')]

def start_subscriber(filter_values):
    subscriber = DDSSubscriber(filter_values)
    subscriber.receive_messages(message_queue)

def main():
    parser = argparse.ArgumentParser(description='Start the DDS Subscriber')
    parser.add_argument('--port', type=int, default=8081,
                       help='Port to run the subscriber web interface (default: 8081)')
    parser.add_argument('--filter', type=str,
                       help='Filter messages by recipient(s). Use comma for multiple values. Use "ALL" or "*" for all messages.')
    args = parser.parse_args()

    global filters
    filters = parse_filters(args.filter)
    
    # Print startup information
    if 'ALL' in filters or '*' in filters:
        print("Starting subscriber: Accepting all messages")
    else:
        print(f"Starting subscriber: Filtering messages for recipients: {filters}")

    subscriber_thread = threading.Thread(target=start_subscriber, args=(filters,))
    subscriber_thread.daemon = True
    subscriber_thread.start()
    
    app.run(port=args.port, debug=True, threaded=True)

if __name__ == '__main__':
    main()