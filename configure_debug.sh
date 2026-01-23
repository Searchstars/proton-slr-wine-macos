#!/bin/sh
set -e

# Recreate configure flags from config_debug.status (debug build).
exec ./configure \
  --enable-win64 \
  --with-vulkan \
  --disable-wow64 \
  --without-x \
  "CFLAGS=-O0 -g" \
  "CXXFLAGS=-O0 -g"
