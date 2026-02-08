#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname "$0")" && pwd)
WINELOADER_DEFAULT="$SCRIPT_DIR/loader/wine64"

if [ $# -lt 1 ]; then
  echo "Usage: $0 /path/to/app.exe [args...]" >&2
  exit 2
fi

EXE_PATH="$1"
shift

case "$EXE_PATH" in
  /*) ;;
  ~/*) EXE_PATH="$HOME/${EXE_PATH#~/}" ;;
  *) EXE_PATH="$(cd "$(dirname "$EXE_PATH")" && pwd)/$(basename "$EXE_PATH")" ;;
esac

if [ ! -f "$EXE_PATH" ]; then
  echo "EXE not found: $EXE_PATH" >&2
  exit 1
fi

WINELOADER=${WINELOADER:-$WINELOADER_DEFAULT}
EXE_DIR=$(dirname "$EXE_PATH")
EXE_BASE=$(basename "$EXE_PATH")

cd "$EXE_DIR"

if [ "$(uname -s)" = "Darwin" ] && command -v arch >/dev/null 2>&1; then
  exec arch -x86_64 "$WINELOADER" "./$EXE_BASE" "$@"
else
  exec "$WINELOADER" "./$EXE_BASE" "$@"
fi
