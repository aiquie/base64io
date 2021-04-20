#!/bin/sh

BASE64IO=${1:-./base64io}

echo '-----------------'
echo '| 1 empty line  |'
echo '-----------------'
echo -n | $BASE64IO

echo '-----------------'
echo '| 5 empty lines |'
echo '-----------------'
for i in $(seq 1 5); do sleep 0.2; echo -n; done | ${BASE64IO}

echo '-----------------'
echo '| 1 data line   |'
echo '-----------------'
echo -n DATA | ${BASE64IO}

echo '-----------------'
echo '| 5 data lines  |'
echo '-----------------'
for i in $(seq 1 5); do sleep 0.2; echo -n DATA; done | ${BASE64IO}

echo '-----------------'
echo '| timeout 1s    |'
echo '-----------------'
for i in $(seq 1 5); do sleep 0.2; echo -n DATA; done | ${BASE64IO} -t 0.5

echo '-----------------'
echo '| decode check  |'
echo '-----------------'
cat ${BASE64IO} | md5sum
cat ${BASE64IO} | ${BASE64IO} | ${BASE64IO} -d | md5sum

echo '-----------------'
echo '| buff size     |'
echo '-----------------'
echo ${BASE64IO} | ${BASE64IO} -b 6

echo '-----------------'
echo '| wrap size     |'
echo '-----------------'
echo ${BASE64IO} | ${BASE64IO} -w 8

echo '-----------------'
echo '| buff and wrap |'
echo '-----------------'
cat ${BASE64IO} | ${BASE64IO} -b 5 | ${BASE64IO} -d | md5sum
cat ${BASE64IO} | ${BASE64IO} -w 7 | ${BASE64IO} -d | md5sum
