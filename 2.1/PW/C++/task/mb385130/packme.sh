#!/bin/bash

#TODO: put your student id here
STUDENTID=mb385130

zip $STUDENTID.zip CMakeLists.txt adorate-42.cpp blimit.hpp blimit.cpp  
mv $STUDENTID.zip ../sols/
