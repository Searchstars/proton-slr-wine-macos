#!/bin/sh
set -e

CURRENT_DIR=$(pwd)
TOMCRYPT_INCLUDE_PATH="-I${CURRENT_DIR}/libs/tomcrypt/src/headers"

exec env \
  FREETYPE_CFLAGS="-I/usr/local/opt/freetype/include/freetype2 -I/usr/local/opt/libpng/include/libpng16" \
  FREETYPE_LIBS="-L/usr/local/opt/freetype/lib -lfreetype" \
  TOMCRYPT_PE_CFLAGS="${TOMCRYPT_INCLUDE_PATH}" \
  TOMCRYPT_PE_LIBS="tomcrypt" \
  CPPFLAGS="${TOMCRYPT_INCLUDE_PATH}" \
  CFLAGS="${TOMCRYPT_INCLUDE_PATH} -O0 -g" \
  CXXFLAGS="${TOMCRYPT_INCLUDE_PATH} -O0 -g" \
  ./configure \
    --enable-win64 \
    --with-vulkan \
    --disable-wow64 \
    --without-x
