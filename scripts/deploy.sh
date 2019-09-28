sh scripts/build.sh release 

rm -rf -d multi-pong

mkdir multi-pong

cp bin/release/multi-pong multi-pong
cp -r res multi-pong

echo "Deploy build created."
echo "cd multi-pong to find it"