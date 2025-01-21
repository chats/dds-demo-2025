from flask import Flask, render_template, request, Response, jsonify
import threading
import queue
import json
import requests
import pandas as pd
import io
from collections import deque
import traceback
import argparse

app = Flask(__name__)
message_queue = queue.Queue()
messages_cache = deque(maxlen=100)

def fetch_excel_data(file_url):
    try:
        print(f"Fetching Excel from URL: {file_url}")
        response = requests.get(file_url)
        
        if response.status_code != 200:
            print(f"Error fetching file: Status code {response.status_code}")
            return None
            
        print("Successfully fetched file, parsing Excel...")
        df = pd.read_excel(io.BytesIO(response.content))
        print(f"Excel data shape: {df.shape}")
        print(f"Columns: {df.columns.tolist()}")
        
        # แปลง DataFrame เป็น dict และจัดการข้อมูลวันที่
        try:
            # แปลงข้อมูลวันที่และจัดการ NaN
            for column in df.columns:
                if pd.api.types.is_datetime64_any_dtype(df[column]):
                    print(f"Converting datetime column: {column}")
                    df[column] = df[column].dt.strftime('%Y-%m-%d %H:%M:%S')
                df[column] = df[column].fillna('')

            # แปลง DataFrame เป็น dict
            data = {
                'headers': df.columns.tolist(),
                'rows': df.values.tolist()
            }
            print("Successfully converted Excel data to dict format")
            return data
            
        except Exception as e:
            print(f"Error processing DataFrame: {str(e)}")
            print(traceback.format_exc())
            return None
            
    except Exception as e:
        print(f"Error in fetch_excel_data: {str(e)}")
        print(traceback.format_exc())
        return None

def format_message(message_data):
    try:
        print(f"Processing message with subject: {message_data.get('subject')}")
        excel_data = None
        
        if message_data.get('fileUrl'):
            print("Fetching Excel data...")
            excel_data = fetch_excel_data(message_data['fileUrl'])
            if excel_data:
                print(f"Excel data fetched successfully. Headers: {excel_data['headers']}")
            else:
                print("Failed to fetch Excel data")

        formatted_message = {
            'subject': message_data.get('subject', ''),
            'recipient': message_data.get('recipient', ''),
            'fileUrl': message_data.get('fileUrl', ''),
            'timestamp': message_data.get('timestamp', ''),
            'excel_data': excel_data
        }
        
        # Test JSON serialization
        try:
            json.dumps(formatted_message)
            print("Message successfully serialized to JSON")
            return formatted_message
        except Exception as e:
            print(f"JSON serialization error: {str(e)}")
            print(traceback.format_exc())
            return None
            
    except Exception as e:
        print(f"Error in format_message: {str(e)}")
        print(traceback.format_exc())
        return None

@app.route('/')
def index():
    try:
        global filters
        if 'ALL' in filters or '*' in filters:
            filtered_messages = []
            for msg in messages_cache:
                formatted_msg = format_message(msg)
                if formatted_msg:
                    filtered_messages.append(formatted_msg)
        else:
            filtered_messages = []
            for msg in messages_cache:
                if msg.get('recipient') in filters:
                    formatted_msg = format_message(msg)
                    if formatted_msg:
                        filtered_messages.append(formatted_msg)
        
        print(f"Rendering index with {len(filtered_messages)} messages")
        return render_template('messages.html', 
                             messages=filtered_messages,
                             current_filters=filters)
    except Exception as e:
        print(f"Error in index route: {str(e)}")
        print(traceback.format_exc())
        return "Error processing request", 500

@app.route('/stream')
def stream():
    def event_stream():
        while True:
            try:
                message = message_queue.get(timeout=1)
                if message:
                    print(f"Got new message: {message.get('subject')}")
                    formatted_message = format_message(message)
                    if formatted_message:
                        messages_cache.appendleft(message)
                        try:
                            data = json.dumps(formatted_message)
                            print(f"Sending message to client: {message.get('subject')}")
                            yield f"data: {data}\n\n"
                        except Exception as e:
                            print(f"Error sending message: {str(e)}")
                            print(traceback.format_exc())
            except queue.Empty:
                yield ": keepalive\n\n"
            except Exception as e:
                print(f"Error in event stream: {str(e)}")
                print(traceback.format_exc())
                continue

    return Response(event_stream(), mimetype="text/event-stream")

def parse_filters(filter_str):
    if not filter_str:
        return ['*']  # Default to accept all if no filter specified
    return [f.strip() for f in filter_str.split(',')]

def start_subscriber(filter_values):
    from subscriber import DDSSubscriber
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