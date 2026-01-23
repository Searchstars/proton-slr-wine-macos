#!/bin/sh
set -e

# Recreate configure flags from config.status (release build).
exec ./configure \
  --enable-win64 \
  --with-vulkan \
  --disable-wow64 \
  --without-x \
  "CFLAGS=-O2 -g0" \
  "CXXFLAGS=-O2 -g0"
