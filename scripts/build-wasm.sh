#!/usr/bin/env bash
# Собирает все консольные проекты в WebAssembly (Emscripten) в каталог wasm-site/.
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT="${REPO_ROOT}/wasm-site"
STUB="${REPO_ROOT}/tools/emscripten-include"

rm -rf "${OUT}"
mkdir -p "${OUT}"

build_one() {
  local slug="$1"
  local rel_srcdir="$2"
  shift 2
  local abs_srcdir="${REPO_ROOT}/${rel_srcdir}"
  local out_dir="${OUT}/${slug}"

  if [[ ! -d "${abs_srcdir}" ]]; then
    echo "error: missing source dir: ${abs_srcdir}" >&2
    exit 1
  fi

  mkdir -p "${out_dir}"

  local -a em_flags=(
    -std=c++17
    -O2
    "-I${STUB}"
    -sALLOW_MEMORY_GROWTH=1
    -sEXIT_RUNTIME=1
    -sENVIRONMENT=web
  )

  local -a abs_cpp=()
  local f
  for f in "$@"; do
    abs_cpp+=("${abs_srcdir}/${f}")
  done

  shopt -s nullglob
  local txt_files=("${abs_srcdir}"/*.txt)
  shopt -u nullglob
  if [[ ${#txt_files[@]} -gt 0 ]]; then
    em_flags+=(--preload-file "${abs_srcdir}@/")
  fi

  em++ "${em_flags[@]}" "${abs_cpp[@]}" -o "${out_dir}/index.html"
  echo "ok: ${slug}"
}

build_one "inverse-matrix" "обратная матрица/обратная матрица" Source.cpp
build_one "rotation-method" "метод вращения/метод вращения" Main.cpp
build_one "hilbert-rotation" "матрица Гилберта и вращение/матрица Гилберта и вращение" Main.cpp
build_one "normalization" "Нормализация данных/Нормализация данных" Main.cpp
build_one "matrix-1-norm" "Норма матрицы(1-норма)/Норма матрицы(1-норма)" Main.cpp
build_one "simpson-trapezoid" "Метод Симпсона и Трапеций/Метод Симпсона и Трапеций" Main.cpp
build_one "newton-simple-iteration" "Метод Ньютона и Простых итераций/Метод Ньютона и Простых итераций" Source.cpp
build_one "interpolation-2" "Interpolation 2/Interpolation 2" \
  C_Spline.cpp First_Function.cpp Lagrange.cpp Second_Funct.cpp Source.cpp Spline.cpp

cat > "${OUT}/index.html" << 'HTMLEOF'
<!DOCTYPE html>
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
HTMLEOF

echo "Artifacts: ${OUT}"
