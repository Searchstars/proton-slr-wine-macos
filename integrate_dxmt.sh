#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname "$0")" && pwd)
ROOT="$SCRIPT_DIR"
cd "$ROOT"

if [ -f "$ROOT/build.env" ]; then
  while IFS= read -r line; do
    case "$line" in
      ""|\#*) continue ;;
    esac

    key=$(printf '%s\n' "$line" | sed -E 's/[[:space:]]*=.*$//')
    val=$(printf '%s\n' "$line" | sed -E 's/^[^=]*=[[:space:]]*//')

    case "$key" in
      DXMT_DIR) DXMT_DIR="$val" ;;
      DXMT_MESON) DXMT_MESON="$val" ;;
      DXMT_WINE_BUILD_PATH) DXMT_WINE_BUILD_PATH="$val" ;;
    esac
  done < "$ROOT/build.env"
fi

DXMT_DIR=${DXMT_DIR:-"$ROOT/tmp_patches/dxmt"}
DXMT_BUILD_DIR=${DXMT_BUILD_DIR:-"$DXMT_DIR/build"}
DXMT_MESON=${DXMT_MESON:-meson}
DXMT_WINE_BUILD_PATH=${DXMT_WINE_BUILD_PATH:-"$ROOT"}

SKIP_BUILD=0
NO_BACKUP=0
USE_ARCH_WRAPPER=1
BACKUP_DIR=""

usage() {
  cat <<'EOF'
Usage: ./integrate_dxmt.sh [options]

Options:
  --skip-build         Skip DXMT rebuild and only install existing artifacts.
  --no-backup          Overwrite Wine DLLs without creating a backup.
  --backup-dir <dir>   Backup destination (default: /tmp/dxmt_wine_backup_<timestamp>).
  --no-arch-wrapper    Don't force arch -x86_64 on macOS.
  -h, --help           Show this help.
EOF
}

while [ $# -gt 0 ]; do
  case "$1" in
    --skip-build)
      SKIP_BUILD=1
      shift
      ;;
    --no-backup)
      NO_BACKUP=1
      shift
      ;;
    --backup-dir)
      BACKUP_DIR="$2"
      shift 2
      ;;
    --no-arch-wrapper)
      USE_ARCH_WRAPPER=0
      shift
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "Unknown option: $1" >&2
      usage >&2
      exit 2
      ;;
  esac
done

run_cmd() {
  if [ "$USE_ARCH_WRAPPER" -eq 1 ] && [ "$(uname -s)" = "Darwin" ] && command -v arch >/dev/null 2>&1; then
    arch -x86_64 "$@"
  else
    "$@"
  fi
}

if [ ! -d "$DXMT_DIR" ]; then
  echo "DXMT_DIR does not exist: $DXMT_DIR" >&2
  exit 1
fi

if [ "$SKIP_BUILD" -eq 0 ]; then
  echo "[dxmt] Building DXMT in: $DXMT_BUILD_DIR"
  run_cmd "$DXMT_MESON" compile -C "$DXMT_BUILD_DIR"
fi

if [ "$NO_BACKUP" -eq 0 ] && [ -z "$BACKUP_DIR" ]; then
  BACKUP_DIR="/tmp/dxmt_wine_backup_$(date +%Y%m%d_%H%M%S)"
fi

if [ "$NO_BACKUP" -eq 0 ]; then
  mkdir -p "$BACKUP_DIR"
  echo "[dxmt] Backup dir: $BACKUP_DIR"
fi

copy_one() {
  src="$1"
  dst="$2"

  if [ ! -f "$src" ]; then
    echo "Missing artifact: $src" >&2
    exit 1
  fi

  mkdir -p "$(dirname "$dst")"

  if [ "$NO_BACKUP" -eq 0 ] && [ -f "$dst" ]; then
    backup_dst="$BACKUP_DIR/${dst#"$DXMT_WINE_BUILD_PATH"/}"
    mkdir -p "$(dirname "$backup_dst")"
    cp -f "$dst" "$backup_dst"
  fi

  cp -f "$src" "$dst"
  echo "[dxmt] Installed: $dst"
}

copy_one "$DXMT_BUILD_DIR/src/d3d11/d3d11.dll" \
  "$DXMT_WINE_BUILD_PATH/dlls/d3d11/x86_64-windows/d3d11.dll"
copy_one "$DXMT_BUILD_DIR/src/dxgi/dxgi.dll" \
  "$DXMT_WINE_BUILD_PATH/dlls/dxgi/x86_64-windows/dxgi.dll"
copy_one "$DXMT_BUILD_DIR/src/d3d10/d3d10core.dll" \
  "$DXMT_WINE_BUILD_PATH/dlls/d3d10core/x86_64-windows/d3d10core.dll"
copy_one "$DXMT_BUILD_DIR/src/winemetal/winemetal.dll" \
  "$DXMT_WINE_BUILD_PATH/dlls/winemetal/x86_64-windows/winemetal.dll"
copy_one "$DXMT_BUILD_DIR/src/winemetal/unix/winemetal.so" \
  "$DXMT_WINE_BUILD_PATH/dlls/winemetal/winemetal.so"

echo "[dxmt] Integration complete."
if [ "$NO_BACKUP" -eq 0 ]; then
  echo "[dxmt] Backup saved at: $BACKUP_DIR"
fi
