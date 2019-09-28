#!/bin/bash

if [ "$1" = "release" ]
then
    ./bin/release/multi-pong 
else
    ./bin/debug/multi-pong 
fi