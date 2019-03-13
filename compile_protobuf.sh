#!/bin/bash

mkdir -p protobuf
protoc -I . --cpp_out=protobuf ./api.proto
