#!/bin/bash

if [ "$1" = "release" ]
then
    ./bin/release/multi-pong $2 $3 $4
else
    ./bin/debug/multi-pong $1 $2 $3
fi