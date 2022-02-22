#!/bin/sh
export NODE_OPTIONS=--openssl-legacy-provider
npm run proto
npm run build
