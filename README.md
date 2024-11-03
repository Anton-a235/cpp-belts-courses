# Используемые файлы кофигурации для VS Code
## tasks.json
```json
{
    "tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: cl.exe сборка активного файла",
            "command": "cl.exe",
            "args": [
                "/Zi",
                "/EHsc",
                "/nologo",
                "/std:c++20",
                "/Wall",
                "/WX",
                "/wd5045",
                "/Od",
                "/diagnostics:column",
                "/Fe${fileDirname}\\${fileBasenameNoExtension}.exe",
                "${file}"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": [
                "$msCompile"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ],
    "version": "2.0.0"
}
```
## launch.json
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "C/C++: cl.exe сборка и отладка активного файла",
            "type": "cppvsdbg",
            "request": "launch",
            "program": "${fileDirname}\\${fileBasenameNoExtension}.exe",
            "args": [
                "<",
                "in.txt",
                ">",
                "out.txt"
            ],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}\\.vscode\\cwd",
            "environment": [],
            "console": "integratedTerminal",
            "preLaunchTask": "C/C++: cl.exe сборка активного файла",
        }
    ]
}
```
# Примечание
Некоторые решения используют возможности C++, рассматриваемые в дальнейших уроках курса.
