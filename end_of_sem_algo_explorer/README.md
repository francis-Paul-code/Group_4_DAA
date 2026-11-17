# Group 4 - Algorithm Explorer

A C11 desktop application built with GTK4 and Cairo to make algorithm execution visible. The current GUI demonstrates Bubble Sort with proportional bars, highlighted comparisons, operation counters, single-step execution and automatic playback.

<!-- explorer-download:start -->
[Windows downloads](https://github.com/francis-Paul-code/Group_4_DAA/releases)

The first successful merge publication replaces this block with a direct ZIP download link.
<!-- explorer-download:end -->

## Run the Windows application

Download the published ZIP, extract **all** its contents, and open `Group_4_DAA.exe` in the extracted folder. Keep its DLLs and resource folders beside it. A development compiler is not required to run the portable package.

## Use the explorer

1. Leave **Bubble Sort** selected. Enter comma-separated integers or leave the input empty to use `64, 28, 85, 16, 47, 103, 28, 55`.
2. Click **Apply data / Reset** to load the bars and start a fresh run.
3. Click **Step** to perform one adjacent comparison and any resulting swap.
4. Click **Start** for automatic execution; click **Stop** to pause. Start resumes the current run.
5. Set **Step delay (ms)** before starting. A larger delay means slower playback; stop and restart to apply a changed delay.

The outlined bars identify the last compared pair. Comparisons counts value comparisons; Operations currently counts calls that perform an algorithm step, so these counters advance together for Bubble Sort.

## Current scope

Verified against PR #1, commit `8ab589aa94f47496cc06198d3ca78ef873f2ee0c`:

- Bubble Sort is integrated with the GUI.
- Fibonacci has a step function but is not connected to the GUI.
- Custom dynamic list, stack, queue, undirected graph and general tree implementations are present.
- Other dropdown algorithm entries currently change the description only. Apply still loads Bubble Sort regardless of those selections.
- Structure renderers and the shared input `List` refactor discussed during development are not yet present in this PR snapshot.
- Input-size and progress widgets are present but are not updated by the current run logic.

## Build from source

Requirements: C compiler, **CMake 4.1 or newer**, Ninja, pkg-config/pkgconf and GTK4 development libraries. The CSS call used by this snapshot requires **GTK 4.12 or newer**.

```sh
git clone https://github.com/francis-Paul-code/Group_4_DAA.git
cd Group_4_DAA
# Until PR #1 is merged, select the explorer branch:
git switch algorithm_explorer
```

On macOS, install Xcode Command Line Tools and Homebrew, then:

```sh
brew install gtk4 cmake ninja pkgconf
cmake -S . -B build -G Ninja
cmake --build build
./build/Group_4_DAA
```

On Windows, run the included setup script from PowerShell:

```powershell
.\windows_setup.ps1
```

It installs MSYS2 when needed through winget and installs the UCRT64 compiler, CMake, Ninja, pkgconf and GTK4. Open **MSYS2 UCRT64** in the repository directory, then:

```sh
cmake -S . -B build-ucrt64 -G Ninja
cmake --build build-ucrt64
./build-ucrt64/Group_4_DAA.exe
```

On Linux, install GCC, GTK4 development packages, pkg-config, Ninja and a sufficiently recent CMake, then use the same configure/build commands and run `./build/Group_4_DAA`. Check the version requirements above; distribution defaults may be older.

For CLion, open the root `CMakeLists.txt`, select the matching toolchain, and reload CMake. GTK is discovered through pkg-config. Use separate build directories for different operating systems and toolchains.

## Windows builds and releases

`.github/workflows/build-explorer.yml` builds PRs and attaches a portable Windows x64 artifact. The new `release-main.yml` uses the same tool installation, CMake build and dependency collection steps after a PR is merged into `main`. It publishes a release ZIP and automatically updates the download block in this README.

See [WORKFLOW_SETUP.md](WORKFLOW_SETUP.md) for installation and publication permissions. The direct ZIP link becomes available after the first successful release run.
