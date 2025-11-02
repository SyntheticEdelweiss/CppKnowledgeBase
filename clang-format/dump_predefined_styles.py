#!/usr/bin/env python3
"""
Dump predefined clang-format styles into files named <Style>.<Version>.clang-format

Usage:
  python dump_clang_styles.py                # uses default clang-format path
  python dump_clang_styles.py --clang /path/to/clang-format.exe
  python dump_clang_styles.py --outdir ./styles
"""

import argparse
import shutil
import subprocess
import sys
from pathlib import Path
import re

DEFAULT_CLANG = Path(r"C:\Qt\Tools\QtCreator\bin\clang\bin\clang-format.exe")
PREDEFINED_STYLES = ["LLVM", "Google", "Chromium", "Mozilla", "WebKit", "Microsoft"]

def find_clang(path_arg: str) -> Path:
    if path_arg:
        p = Path(path_arg)
        if p.is_file():
            return p
        raise FileNotFoundError(f"clang-format not found at: {p}")
    # try PATH first
    exe = shutil.which("clang-format")
    if exe:
        return Path(exe)
    # fallback default
    if DEFAULT_CLANG.is_file():
        return DEFAULT_CLANG
    raise FileNotFoundError("clang-format not found on PATH and default path missing")

def run(cmd):
    proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    return proc.returncode, proc.stdout, proc.stderr

def detect_version(clang_path: Path) -> str:
    rc, out, err = run([str(clang_path), "--version"])
    combined = (out + ("\n" + err if err else "")).strip()
    if rc != 0:
        raise RuntimeError(f"clang-format --version failed (exit {rc}): {combined}")
    m = re.search(r"(\d+(?:\.\d+)+)", combined)
    if m:
        return m.group(1)
    # sanitize fallback token
    token = re.sub(r"\s+", "_", combined)
    token = re.sub(r"[^\w\.\-]", "", token)
    if not token:
        raise RuntimeError("Unable to determine clang-format version string")
    return token

def dump_style(clang_path: Path, style: str):
    # use long form and quote style explicitly
    cmd = [str(clang_path), f'--style={style}', "-dump-config"]
    rc, out, err = run(cmd)
    return rc, out, err

def main():
    ap = argparse.ArgumentParser(description="Dump predefined clang-format styles to versioned files")
    ap.add_argument("--clang", "-c", help="Path to clang-format executable (optional)")
    ap.add_argument("--outdir", "-o", help="Output directory (default: current dir)", default=".")
    args = ap.parse_args()

    try:
        clang = find_clang(args.clang)
    except FileNotFoundError as e:
        print(f"ERROR: {e}", file=sys.stderr)
        sys.exit(2)

    try:
        version = detect_version(clang)
    except Exception as e:
        print(f"ERROR: {e}", file=sys.stderr)
        sys.exit(3)

    outdir = Path(args.outdir).resolve()
    outdir.mkdir(parents=True, exist_ok=True)

    for s in PREDEFINED_STYLES:
        style = s.strip()
        fname = outdir / f"{style}.{version}.clang-format"
        rc, out, err = dump_style(clang, style)
        if rc == 0 and out.strip():
            fname.write_text(out, encoding="utf-8")
            print(f"Wrote {fname}")
        else:
            # write diagnostics to file for inspection
            content_parts = []
            if out:
                content_parts.append("STDOUT:\n" + out)
            if err:
                content_parts.append("STDERR:\n" + err)
            if not content_parts:
                content_parts = [f"No output captured; exit code {rc}"]
            fname.write_text("\n\n".join(content_parts), encoding="utf-8")
            print(f"clang-format failed for style {style} (exit {rc}); output saved to {fname}", file=sys.stderr)

if __name__ == "__main__":
    main()
