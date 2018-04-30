#!/bin/bash

ffmpeg -i $1/%05d.png $1/$(basename $1).$2
