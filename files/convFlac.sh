#!/bin/bash
ts=$(date +%s%N) ; 
ffmpeg -i sample03.wav out.flac ; 
tt=$((($(date +%s%N) - $ts)/1000000)) ; echo "Time taken: $tt milliseconds"
