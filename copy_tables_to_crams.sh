#!/usr/bin/env bash
# Copy the selected output tables into the CRAMS directory.
# Usage: ./copy_tables_to_crams.sh [DEST_DIR]
#   DEST_DIR defaults to $CRAMS_DIR below; pass a path to override.
set -euo pipefail

ROOT=~/Work/codes/XS4GCRv2/build/output/

# Destination paper directory (edit this default or pass one as $1).
CRAMS_DIR="${1:-$HOME/Work/codes/crams/data}"

TABLES=(
	crams_fragmentation_evoli2019.csv
	crams_fragmentation_evoli2026_st99.csv
	crams_fragmentation_evoli2026_w93.csv
	crams_fragmentation_fluka4dragon.csv
	crams_fragmentation_usine_galprop17_opt12.csv
	crams_fragmentation_usine_galprop17_opt22.csv
	crams_fragmentation_usine_webber03+coste12.csv
	crams_inelastic_crosec.csv
	crams_inelastic_glauber.csv
	crams_inelastic_tripathi99.csv
	)

mkdir -p "$CRAMS_DIR"

missing=0
for table in "${TABLES[@]}"; do
    src="$ROOT/$table"
    if [[ -f "$src" ]]; then
        cp -v "$src" "$CRAMS_DIR/"
        echo "copied $table"
    else
        echo "MISSING $src" >&2
        missing=1
    fi
done

echo "---"
echo "destination: $CRAMS_DIR"
[[ $missing -eq 0 ]] && echo "all tables copied" || echo "some tables were missing (see above)"
exit $missing
