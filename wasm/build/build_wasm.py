#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Собирает консольные проекты в WebAssembly (Emscripten). Пути с кириллицей в Unicode — надёжно на Linux CI."""
from __future__ import annotations

import shutil
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
OUT = REPO_ROOT / "wasm-site"
STUB = REPO_ROOT / "wasm" / "emscripten-include"

PROJECTS: list[tuple[str, str, list[str]]] = [
    ("inverse-matrix", "src/обратная матрица/обратная матрица", ["Source.cpp"]),
    ("rotation-method", "src/метод вращения/метод вращения", ["Main.cpp"]),
    (
        "hilbert-rotation",
        "src/матрица Гилберта и вращение/матрица Гилберта и вращение",
        ["Main.cpp"],
    ),
    ("normalization", "src/Нормализация данных/Нормализация данных", ["Main.cpp"]),
    (
        "matrix-1-norm",
        "src/Норма матрицы(1-норма)/Норма матрицы(1-норма)",
        ["Main.cpp"],
    ),
    (
        "simpson-trapezoid",
        "src/Метод Симпсона и Трапеций/Метод Симпсона и Трапеций",
        ["Main.cpp"],
    ),
    (
        "newton-simple-iteration",
        "src/Метод Ньютона и Простых итераций/Метод Ньютона и Простых итераций",
        ["Source.cpp"],
    ),
    (
        "interpolation-2",
        "src/Interpolation 2/Interpolation 2",
        [
            "C_Spline.cpp",
            "First_Function.cpp",
            "Lagrange.cpp",
            "Second_Funct.cpp",
            "Source.cpp",
            "Spline.cpp",
        ],
    ),
]

def main() -> None:
    if OUT.exists():
        shutil.rmtree(OUT)
    OUT.mkdir(parents=True)

    for slug, rel, cpp_files in PROJECTS:
        srcdir = (REPO_ROOT / rel).resolve()
        if not srcdir.is_dir():
            print(f"error: missing source dir: {srcdir}", file=sys.stderr)
            sys.exit(1)

        out_dir = OUT / slug
        out_dir.mkdir(parents=True)

        em_flags: list[str] = [
            "-std=c++17",
            "-O2",
            f"-I{STUB}",
            "-sALLOW_MEMORY_GROWTH=1",
            "-sEXIT_RUNTIME=1",
            "-sENVIRONMENT=web",
        ]

        if any(srcdir.glob("*.txt")):
            em_flags.extend(["--preload-file", f"{srcdir}@/"])

        sources = [str(srcdir / name) for name in cpp_files]
        cmd = ["em++", *em_flags, *sources, "-o", str(out_dir / "main.js")]
        subprocess.run(cmd, check=True, cwd=str(REPO_ROOT))
        shutil.copy(REPO_ROOT / "web" / "template.html", out_dir / "index.html")
        print(f"ok: {slug}")

    shutil.copy(REPO_ROOT / "web" / "index.html", OUT / "index.html")
    print(f"Artifacts: {OUT}")


if __name__ == "__main__":
    main()
