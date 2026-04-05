#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Собирает консольные проекты в WebAssembly (Emscripten). Пути с кириллицей в Unicode — надёжно на Linux CI."""
from __future__ import annotations

import shutil
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
OUT = REPO_ROOT / "wasm-site"
STUB = REPO_ROOT / "tools" / "emscripten-include"

PROJECTS: list[tuple[str, str, list[str]]] = [
    ("inverse-matrix", "обратная матрица/обратная матрица", ["Source.cpp"]),
    ("rotation-method", "метод вращения/метод вращения", ["Main.cpp"]),
    (
        "hilbert-rotation",
        "матрица Гилберта и вращение/матрица Гилберта и вращение",
        ["Main.cpp"],
    ),
    ("normalization", "Нормализация данных/Нормализация данных", ["Main.cpp"]),
    (
        "matrix-1-norm",
        "Норма матрицы(1-норма)/Норма матрицы(1-норма)",
        ["Main.cpp"],
    ),
    (
        "simpson-trapezoid",
        "Метод Симпсона и Трапеций/Метод Симпсона и Трапеций",
        ["Main.cpp"],
    ),
    (
        "newton-simple-iteration",
        "Метод Ньютона и Простых итераций/Метод Ньютона и Простых итераций",
        ["Source.cpp"],
    ),
    (
        "interpolation-2",
        "Interpolation 2/Interpolation 2",
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

INDEX_HTML = """<!DOCTYPE html>
<html lang="ru">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>Численные методы — WebAssembly</title>
  <style>
    body { font-family: system-ui, sans-serif; max-width: 42rem; margin: 2rem auto; padding: 0 1rem; line-height: 1.5; }
    ul { padding-left: 1.2rem; }
    a { color: #0b57d0; }
  </style>
</head>
<body>
  <h1>Численные методы (WASM)</h1>
  <p>Сборки Emscripten для запуска в браузере. Для ввода с клавиатуры используйте поля ввода в странице (stdin).</p>
  <ul>
    <li><a href="inverse-matrix/index.html">Обратная матрица</a></li>
    <li><a href="rotation-method/index.html">Метод вращения</a></li>
    <li><a href="hilbert-rotation/index.html">Матрица Гилберта и вращение</a></li>
    <li><a href="normalization/index.html">Нормализация данных</a></li>
    <li><a href="matrix-1-norm/index.html">Норма матрицы (1-норма)</a></li>
    <li><a href="simpson-trapezoid/index.html">Метод Симпсона и трапеций</a></li>
    <li><a href="newton-simple-iteration/index.html">Метод Ньютона и простых итераций</a></li>
    <li><a href="interpolation-2/index.html">Интерполяция 2</a></li>
  </ul>
</body>
</html>
"""


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
        cmd = ["em++", *em_flags, *sources, "-o", str(out_dir / "index.html")]
        subprocess.run(cmd, check=True, cwd=str(REPO_ROOT))
        print(f"ok: {slug}")

    (OUT / "index.html").write_text(INDEX_HTML, encoding="utf-8")
    print(f"Artifacts: {OUT}")


if __name__ == "__main__":
    main()
