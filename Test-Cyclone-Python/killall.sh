#!/bin/bash

ps aux | egrep "python" | egrep "web_server.py" | awk '{print $2}' | xargs kill -9
ps aux | egrep "python" | egrep "subscriber.py" | awk '{print $2}' | xargs kill -9
ps aux | egrep "python" | egrep "publisher.py" | awk '{print $2}' | xargs kill -9
