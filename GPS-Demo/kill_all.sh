#!/bin/bash

ps -ef | grep GPSDemoPublisher | grep -v grep | awk '{print $2}' | xargs kill -9