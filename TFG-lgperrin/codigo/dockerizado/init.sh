#!/bin/bash

docker run -d -p 6789:6789 -v ./workspace/demo:/home/src --name mageai --rm mageai/mageai /app/run_app.sh mage start demo
