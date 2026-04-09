#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Собирает консольные проекты в WebAssembly (Emscripten). Пути с кириллицей в Unicode — надёжно на Linux CI."""
from __future__ import annotations

import re
import shutil
import subprocess
import sys
from pathlib import Path
from dataclasses import dataclass

REPO_ROOT = Path(__file__).resolve().parents[2]
"""Корень репозитория (директория проекта), вычисленный относительно этого скрипта."""
GH_ACTION_OUTPUT = REPO_ROOT / "wasm-site"
"""Каталог, куда складываются артефакты сборки WebAssembly-сайта."""
PROJECTS_DIR = REPO_ROOT / "src"
"""Каталог с подкаталогами проектов (каждый проект — папка с `.cpp` и точкой входа)."""
STUB = REPO_ROOT / "wasm" / "emscripten-include"
"""Путь к локальным include/заглушкам для Emscripten (добавляется через `-I`)."""

TEMPLATE_HTML = REPO_ROOT / "web" / "template.html"
"""HTML-шаблон страницы проекта (копируется как `index.html` в папку каждого проекта)."""
INDEX_TEMPLATE_PATH = REPO_ROOT / "web" / "index.html"
"""HTML-шаблон главной страницы со списком проектов (используется как шаблон)."""

ENTRY_FILENAMES = {"Main.cpp", "Source.cpp"}
"""Имена файлов, наличие которых считается признаком точки входа (entrypoint) проекта."""

@dataclass
class Project:
    slug: str
    path: Path
    cpp_files: list[str]

def slugify(name: str) -> str:
    """Преобразует имя проекта в “безопасное” имя для URL
    
    - Приводит к нижнему регистру, заменяет пробелы на `-` чтоб не было %20 в URL-строке.
    - Удаляет символы, не подходящие для URL/имени каталога (с учётом Unicode)
    - Нормализует повторяющиеся дефисы

    Возвращает `"project"`, если после очистки строка пустая.
    """
    s = name.strip().lower().replace(" ", "-")
    """Промежуточная строка slug после тримминга/нижнего регистра/замены пробелов."""
    s = re.sub(r"[^\w\-]+", "", s, flags=re.UNICODE)
    """Промежуточная строка slug после удаления лишних символов (Unicode-aware)."""
    s = re.sub(r"-{2,}", "-", s).strip("-")
    """Промежуточная строка slug после схлопывания дефисов и обрезки по краям."""
    return s or "project"


def find_projects(projects_root: Path) -> list[Project]:
    """Возвращает список проектов, находящихся в `projects_root`."""
    projects: list[Project] = []

    for path in projects_root.rglob("*"):
        if not path.is_dir():
            continue

        cpp_files = list(path.glob("*.cpp"))
        if not cpp_files:
            continue

        has_entry = any(f.name in ENTRY_FILENAMES for f in cpp_files)
        if not has_entry:
            continue

        projects.append(Project(
            slug=slugify(path.name),
            path=path,
            cpp_files=[f.name for f in cpp_files]
        ))

    return projects

def render_index(projects: list[Project], template: str) -> str:
    """Подставляет список проектов в HTML-шаблон главной страницы.

    Ожидается, что `template` содержит маркер `{{PROJECTS}}`, который будет заменён
    на `<li>` элементы со ссылками на страницы проектов.
    """
    items = "\n    ".join(
        f'<li><a href="{p.slug}/index.html">{p.slug}</a></li>' 
        for p in projects
    )
    return template.replace("{{PROJECTS}}", items)


def main() -> None:
    """Точка входа сборки.

    - Проверяет наличие исходников и `em++` в PATH
    - Находит проекты и собирает каждый в отдельную папку
    - Копирует HTML-шаблон проекта и генерирует общий `index.html`
    """
    # Абсолютный путь к корневому каталогу артефактов сборки.
    gh_action_output: Path = GH_ACTION_OUTPUT.resolve()
    # Абсолютный путь к каталогу с проектами-исходниками.
    projects_root: Path = PROJECTS_DIR.resolve()

    # проверки на существование файлов и папок
    if gh_action_output.exists():
        shutil.rmtree(gh_action_output)
    gh_action_output.mkdir(parents=True)

    if not projects_root.is_dir():
        print(f"error: projects dir not found: {projects_root}", file=sys.stderr)
        sys.exit(1)

    if shutil.which("em++") is None:
        print("error: em++ not found in PATH. Activate Emscripten SDK (emsdk_env) and retry.", file=sys.stderr)
        sys.exit(1)

    if not TEMPLATE_HTML.is_file():
        print(f"error: missing template: {TEMPLATE_HTML}", file=sys.stderr)
        sys.exit(1)

    if not INDEX_TEMPLATE_PATH.is_file():
        print(f"error: missing index template: {INDEX_TEMPLATE_PATH}", file=sys.stderr)
        sys.exit(1)

    # Список проектов, найденных для сборки.
    projects : list[Project] = find_projects(projects_root)    
    # Сортировка проектов по slug для стабильного порядка в выводе и `index.html`.
    # projects.sort(key=lambda t: t[0])

    if not projects:
        print("error: no projects found to build.", file=sys.stderr)
        sys.exit(1)

    # Итерация по проектам: slug, директория исходников, список `.cpp` файлов.
    for project in projects:
        # Абсолютный путь к директории проекта.
        srcdir = project.path.resolve()

        # Каталог артефактов для текущего проекта.
        out_dir = gh_action_output / project.slug
        out_dir.mkdir(parents=True)

        # Базовые флаги компиляции/линковки Emscripten для веб-окружения.
        em_flags: list[str] = [
            "-std=c++17",
            "-O2",
            f"-I{STUB}",
            "-sALLOW_MEMORY_GROWTH=1",
            "-sEXIT_RUNTIME=1",
            "-sENVIRONMENT=web",
        ]

        # Если есть `.txt`, добавляем их в виртуальную FS через `--preload-file`.
        if any(srcdir.glob("*.txt")):
            em_flags.extend(["--preload-file", f"{srcdir}@/"])

        # Абсолютные пути к исходникам `.cpp` для передачи в `em++`.
        sources = [str(srcdir / cpp_file_name) for cpp_file_name in project.cpp_files]
        # Команда `em++` для сборки проекта в `main.js` + сопутствующие артефакты.
        cmd = ["em++", *em_flags, *sources, "-o", str(out_dir / "main.js")]
        subprocess.run(cmd, check=True, cwd=str(REPO_ROOT))
        
        shutil.copy(TEMPLATE_HTML, out_dir / "index.html")
        print(f"ok: {out_dir}")

    (INDEX_TEMPLATE_PATH / "index.html").write_text(
        render_index(projects, INDEX_TEMPLATE_PATH.read_text(encoding="utf-8")),
        encoding="utf-8",
    )
    print(f"Artifacts: {out_dir}")


if __name__ == "__main__":
    main()
