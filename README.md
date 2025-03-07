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
                "/std:c++17",
                "/Wall",
                "/WX",
                "/wd4514",
                "/wd4820",
                "/wd5045",
                "/Od",
                "/diagnostics:caret",
                "/I${fileDirname}\\..\\..\\utility",
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
                "input.txt",
                ">",
                "output.txt",
                "2>",
                "errors.txt"
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
