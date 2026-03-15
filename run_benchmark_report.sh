#!/bin/sh
set -eu

aScriptDir="$(CDPATH= cd -- "$(dirname "$0")" && pwd)"
cd "$aScriptDir"

aLogsDir="BENCHMARK_LOGS"
aOutputFile="run_benchmark_report_output.txt"

rm -rf "$aLogsDir"
mkdir -p "$aLogsDir"

aRunStatus=0
./run_benchmarks.sh || aRunStatus=$?

if [ "$aRunStatus" -ne 0 ]; then
  echo "WARNING: run_benchmarks.sh completed with failures (status=$aRunStatus)."
  echo "Report will include available successful log files only."
fi

python3 - "$aLogsDir" "$aOutputFile" <<'PY'
import datetime as dt
import os
import re
import sys
from collections import defaultdict

logs_dir = sys.argv[1]
output_file = sys.argv[2]

stamp_re = re.compile(r'^(.*)_(\d{4})_(\d{2})_(\d{2})_(\d{2})_(\d{2})\.txt$')


def parse_time_us(value: str):
    value = value.strip()
    match = re.match(r'^([0-9]+(?:\.[0-9]+)?)(K)?\s+us$', value)
    if not match:
        return None
    number = float(match.group(1))
    if match.group(2):
        number *= 1000.0
    return number


def choose_section(sections):
    for key in ("L3 Blocks", "L2 Blocks", "L1 Blocks"):
        if key in sections:
            return key
    return next(iter(sections.keys()), None)


def choose_mode(modes):
    for key in ("Soft", "Normal"):
        if key in modes:
            return key
    return next(iter(modes.keys()), None)


def pretty_name(stem: str):
    parts = [part for part in stem.split("_") if part]
    out = []
    for part in parts:
        if part.isdigit():
            out.append(part)
        elif part in {"vv", "vh", "hv", "hh", "v", "h"}:
            out.append(part.upper())
        else:
            out.append(part.capitalize())
    return "".join(out)


rows_by_name = {}
for filename in sorted(os.listdir(logs_dir)):
    if not filename.endswith(".txt"):
        continue
    match = stamp_re.match(filename)
    if match is None:
        continue
    stem = match.group(1)
    stamp = dt.datetime(
        int(match.group(2)),
        int(match.group(3)),
        int(match.group(4)),
        int(match.group(5)),
        int(match.group(6)),
    )

    section_data = defaultdict(lambda: defaultdict(list))
    current_section = None

    with open(os.path.join(logs_dir, filename), "r", encoding="utf-8") as fh:
        for line in fh:
            line = line.rstrip("\n")
            if line.startswith("L1 Blocks") or line.startswith("L2 Blocks") or line.startswith("L3 Blocks"):
                current_section = line.split(" (", 1)[0].strip()
                continue
            if current_section is None or "|" not in line:
                continue

            parts = [piece.strip() for piece in line.split("|")]
            # Expected table row shape:
            # | mode | mean_time | ... | ... |   | mode | mean_time | ... | ... |
            for mode_idx, time_idx in ((1, 2), (6, 7)):
                if mode_idx >= len(parts) or time_idx >= len(parts):
                    continue
                mode = parts[mode_idx]
                if not mode or mode.lower() == "mode":
                    continue
                value = parse_time_us(parts[time_idx])
                if value is None:
                    continue
                section_data[current_section][mode].append(value)

    section = choose_section(section_data)
    if section is None:
        continue
    mode = choose_mode(section_data[section])
    if mode is None:
        continue
    values = section_data[section][mode]
    if not values:
        continue
    mean_value = sum(values) / len(values)
    name = pretty_name(stem)
    previous = rows_by_name.get(name)
    if previous is None or stamp > previous[0]:
        rows_by_name[name] = (stamp, mean_value)

rows = sorted(((name, value[1]) for name, value in rows_by_name.items()), key=lambda item: item[0])
with open(output_file, "w", encoding="utf-8") as out:
    for name, mean_value in rows:
        out.write(f"{name}: {mean_value:.2f} us\n")
PY

echo "Wrote $aOutputFile"
