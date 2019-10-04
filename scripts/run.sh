#!/bin/bash

if [ "$1" = "release" ]
then
    ./bin/release/client/multi-pong $2 $3 $4
else
    ./bin/debug/client/multi-pong $1 $2 $3
fi