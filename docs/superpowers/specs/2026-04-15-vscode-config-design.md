# VS Code Build And Debug Configuration Design

## Goal

Add minimal VS Code workspace configuration so the project can be configured, built, and debugged quickly from the editor without manually typing the CMake commands each time.

The configuration should stay explicit and simple, matching the current project structure and default Apple toolchain rather than introducing more automation layers or extra extension dependencies than necessary.

## Scope

The initial VS Code setup will add:

1. `.vscode/tasks.json`
2. `.vscode/launch.json`

The configuration will support the existing executables:

- `ranges_demo`
- `concepts_demo`
- `cpp26_demo`

## Non-Goals

- Do not introduce CMake Tools-specific workflows as a hard requirement.
- Do not replace the existing command-line build flow described in `README.md`.
- Do not add per-file debugging or active-file compilation tasks.
- Do not add test tasks because the project does not currently have an automated test suite.

## Tooling Baseline

- Editor: VS Code
- Debugger style: VS Code `C/C++` extension using `cppdbg`
- Debug backend on macOS: `lldb`
- Compiler toolchain: default Apple clang from Xcode
- Build layout assumption: single-config CMake build directory at `build/`

This matches the current project setup and keeps the VS Code configuration aligned with the commands that already work in the terminal.

## File Design

### `.vscode/tasks.json`

Purpose:
Provide one-click tasks for the common CMake actions needed by this small project.

Tasks to include:

- `Configure`
  - runs `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`

- `Build All`
  - runs `cmake --build build`

- `Build ranges_demo`
  - runs `cmake --build build --target ranges_demo`

- `Build concepts_demo`
  - runs `cmake --build build --target concepts_demo`

- `Build cpp26_demo`
  - runs `cmake --build build --target cpp26_demo`

Design notes:

- Tasks should be shell-based and explicit.
- Build tasks may depend on `Configure` so the project is generated before target builds are attempted.
- `Build All` should be marked as the default build task so `Run Build Task` remains a safe whole-project option.

### `.vscode/launch.json`

Purpose:
Provide straightforward debug launch entries for each executable in the project.

Launch configurations to include:

- `Debug ranges_demo`
- `Debug concepts_demo`
- `Debug cpp26_demo`

Each launch configuration should:

- use `type: "cppdbg"`
- use `request: "launch"`
- use `MIMode: "lldb"`
- set `cwd` to `${workspaceFolder}`
- point `program` at the matching executable under `build/src/...`
- use `preLaunchTask` to build the matching target before starting the debugger
- avoid unnecessary custom LLDB settings unless they are required

## Data Flow

The intended workflow is:

1. Run a task from VS Code, or press the debugger play button
2. VS Code runs the matching build task first
3. CMake builds the requested target into `build/`
4. VS Code launches LLDB against the resulting executable

This keeps the editor workflow consistent with the command-line workflow already documented in the repository.

## Error Handling

The configuration should fail in transparent ways:

- if `build/` has not been configured yet, the `Configure` task should establish it
- the initial configure task should explicitly request a `Debug` build so source-level debugging works predictably with the single-config generator flow used by this project
- if a build fails, the `preLaunchTask` should stop debugging from starting
- if the user switches to a multi-config CMake generator later, the documented executable paths may need updating

Because the current project uses a simple single-config setup, the first version of the VS Code config should optimize for that path rather than abstracting over every possible future generator layout.

## Documentation Impact

The `README.md` may optionally gain a short note later that `.vscode/tasks.json` and `.vscode/launch.json` are included for convenience, but that is not required for the first version.

## Future Extensions

Possible future improvements, only if needed later:

- add a `Clean Rebuild` task
- add compound launch configurations
- add support for multi-config generator layouts
- add test tasks once the project has an automated test suite
