#!/bin/bash

mkdir -p src/protobuf
protoc -I . --cpp_out=./src/protobuf ./api.proto
