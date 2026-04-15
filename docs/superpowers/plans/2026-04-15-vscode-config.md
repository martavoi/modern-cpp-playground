# VS Code Build And Debug Configuration Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add minimal VS Code workspace configuration so the project can configure, build, and debug each demo target directly from the editor.

**Architecture:** Use a workspace-local `.vscode` folder with explicit shell-based build tasks and explicit `cppdbg` launch configurations. Keep the setup aligned with the existing single-config CMake build directory and default Apple clang toolchain rather than introducing extension-specific build logic.

**Tech Stack:** VS Code, `.vscode/tasks.json`, `.vscode/launch.json`, Microsoft C/C++ extension (`cppdbg`), LLDB on macOS, CMake 3.25+, Apple clang 21

---

## File Structure

- Create: `.vscode/tasks.json`
- Create: `.vscode/launch.json`

File responsibilities:

- `.vscode/tasks.json`: explicit VS Code tasks for configure, full build, and per-target builds.
- `.vscode/launch.json`: per-target debug launch entries that build before launching and point at the correct executable paths under `build/`.

### Task 1: Add Build Tasks

**Files:**
- Create: `/Users/dmartavoi/Projects/cpp26/.vscode/tasks.json`

- [ ] **Step 1: Create the VS Code tasks file**

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Configure",
      "type": "shell",
      "command": "cmake",
      "args": ["-S", ".", "-B", "build", "-DCMAKE_BUILD_TYPE=Debug"],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "problemMatcher": []
    },
    {
      "label": "Build All",
      "type": "shell",
      "command": "cmake",
      "args": ["--build", "build"],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "dependsOn": ["Configure"],
      "dependsOrder": "sequence",
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "problemMatcher": ["$gcc"]
    },
    {
      "label": "Build ranges_demo",
      "type": "shell",
      "command": "cmake",
      "args": ["--build", "build", "--target", "ranges_demo"],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "dependsOn": ["Configure"],
      "dependsOrder": "sequence",
      "problemMatcher": ["$gcc"]
    },
    {
      "label": "Build concepts_demo",
      "type": "shell",
      "command": "cmake",
      "args": ["--build", "build", "--target", "concepts_demo"],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "dependsOn": ["Configure"],
      "dependsOrder": "sequence",
      "problemMatcher": ["$gcc"]
    },
    {
      "label": "Build cpp26_demo",
      "type": "shell",
      "command": "cmake",
      "args": ["--build", "build", "--target", "cpp26_demo"],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "dependsOn": ["Configure"],
      "dependsOrder": "sequence",
      "problemMatcher": ["$gcc"]
    }
  ]
}
```

- [ ] **Step 2: Validate the JSON file syntax**

Run: `python3 -m json.tool ".vscode/tasks.json" >/dev/null`

Expected: exit code `0` and no output.

- [ ] **Step 3: Verify the configure task command directly**

Run: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`

Expected: CMake configures successfully and writes build files to `build/`.

- [ ] **Step 4: Verify the default whole-project build command directly**

Run: `cmake --build build`

Expected: all current targets build successfully.

### Task 2: Add Debug Launch Configurations

**Files:**
- Create: `/Users/dmartavoi/Projects/cpp26/.vscode/launch.json`

- [ ] **Step 1: Create the VS Code launch file**

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug ranges_demo",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/src/ranges_demo/ranges_demo",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "osx": {
        "MIMode": "lldb"
      },
      "preLaunchTask": "Build ranges_demo"
    },
    {
      "name": "Debug concepts_demo",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/src/concepts_demo/concepts_demo",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "osx": {
        "MIMode": "lldb"
      },
      "preLaunchTask": "Build concepts_demo"
    },
    {
      "name": "Debug cpp26_demo",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/src/cpp26_demo/cpp26_demo",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "osx": {
        "MIMode": "lldb"
      },
      "preLaunchTask": "Build cpp26_demo"
    }
  ]
}
```

- [ ] **Step 2: Validate the JSON file syntax**

Run: `python3 -m json.tool ".vscode/launch.json" >/dev/null`

Expected: exit code `0` and no output.

- [ ] **Step 3: Verify the executable paths used by the launch configurations**

Run: `cmake --build build --target ranges_demo concepts_demo cpp26_demo`

Expected: all three targets build successfully with no missing target errors.

- [ ] **Step 4: Verify the `ranges_demo` launch path target directly**

Run: `./build/src/ranges_demo/ranges_demo`

Expected output:

```text
Even squares: 4 16 36
```

- [ ] **Step 5: Verify the `concepts_demo` launch path target directly**

Run: `./build/src/concepts_demo/concepts_demo`

Expected output:

```text
square(12) = 144
square(1.5) = 2.25
```

- [ ] **Step 6: Verify the `cpp26_demo` launch path target directly**

Run: `./build/src/cpp26_demo/cpp26_demo`

Expected output:

```text
Parsed digit: 7
Could not parse input
```

### Task 3: Final Workspace Verification

**Files:**
- Modify: `/Users/dmartavoi/Projects/cpp26/.vscode/tasks.json`
- Modify: `/Users/dmartavoi/Projects/cpp26/.vscode/launch.json`

- [ ] **Step 1: Re-run a clean configure from the workspace root**

Run: `rm -rf build && cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`

Expected: clean configure succeeds and recreates `build/`.

- [ ] **Step 2: Rebuild the full project**

Run: `cmake --build build`

Expected: `ranges_demo`, `concepts_demo`, and `cpp26_demo` all build successfully.

- [ ] **Step 3: Re-validate both VS Code JSON files**

Run: `python3 -m json.tool ".vscode/tasks.json" >/dev/null && python3 -m json.tool ".vscode/launch.json" >/dev/null`

Expected: exit code `0` and no output.

- [ ] **Step 4: Re-run all three executables from their launch paths**

Run: `./build/src/ranges_demo/ranges_demo && ./build/src/concepts_demo/concepts_demo && ./build/src/cpp26_demo/cpp26_demo`

Expected output:

```text
Even squares: 4 16 36
square(12) = 144
square(1.5) = 2.25
Parsed digit: 7
Could not parse input
```
