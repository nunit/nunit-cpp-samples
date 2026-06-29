# GitHub Actions CI for C++/CLI NUnit Tests

This guide explains the CI workflow for building and testing the C++/CLI NUnit project on GitHub Actions, why a standard runner is not sufficient out of the box, and how each part of the workflow addresses that.

---

## Contents

1. [Why CI is non-trivial for C++/CLI](#why-ci-is-non-trivial-for-ccli)
2. [Runner and toolset requirements](#runner-and-toolset-requirements)
3. [The workflow file](#the-workflow-file)
4. [Step-by-step explanation](#step-by-step-explanation)
5. [VS 2026 Build Tools components](#vs-2026-build-tools-components)
6. [Expected run times](#expected-run-times)
7. [Troubleshooting CI failures](#troubleshooting-ci-failures)

---

## Why CI is non-trivial for C++/CLI

Running a standard `dotnet test` or `msbuild` job on a GitHub-hosted runner fails for this project for one specific reason: the project requires the **v145 platform toolset**, which ships with Visual Studio 2026. The standard `windows-latest` runner image ships Visual Studio 2022 (toolset v143), not VS 2026.

```mermaid
graph LR
    A["windows-latest runner<br/>(VS 2022 / v143)"]
    B["cpp-cli-syntax.vcxproj<br/>PlatformToolset = v145"]
    C["MSB8020: Build tools<br/>for v145 not found"]
    A -->|tries to build| B --> C

    style C fill:#ffcdd2,stroke:#c62828
```

The solution is to install the VS 2026 Build Tools at the start of the job. This adds time to each run but is reliable and self-contained — no custom runner images or self-hosted runners are needed.

---

## Runner and toolset requirements

| Requirement | What provides it | Notes |
|---|---|---|
| `windows-latest` runner | GitHub-hosted | Windows Server 2022, has VS 2022 pre-installed |
| v145 C++ compiler | VS 2026 Build Tools (installed in CI) | `cl.exe /clr:netcore` |
| C++/CLI support (`/clr:netcore`) | `VC.CLI.Support` component | Required for mixed-mode assembly |
| MSBuild v18 | VS 2026 Build Tools (installed in CI) | Set up via `microsoft/setup-msbuild@v2` |
| NuGet CLI | Pre-installed on runner | Used to restore packages |
| .NET 8 runtime | Pre-installed on runner | Required by testhost at test time |
| `vstest.console.exe` | VS 2022 (pre-installed on runner) | `test.cmd` finds VS 2022 path |

> **Note on `vstest.console.exe`:** The test runner (`test.cmd`) checks for VS 2026 first, then falls back to VS 2022. On the CI runner, VS 2022's vstest is used, which is fine — vstest compatibility is independent of the build toolset.

---

## The workflow file

Located at `.github/workflows/CI.yml`:

```yaml
name: CI

on:
  push:
    branches: [ master ]
  pull_request:
    branches: [ master ]

jobs:
  build-and-test:
    runs-on: windows-latest

    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Install VS 2026 Build Tools (v145 toolset with C++/CLI support)
        shell: pwsh
        run: |
          $installer = "$env:TEMP\vs_buildtools.exe"
          Invoke-WebRequest -Uri "https://aka.ms/vs/18/release/vs_buildtools.exe" -OutFile $installer
          $process = Start-Process -FilePath $installer -Wait -PassThru -ArgumentList @(
            "--quiet", "--wait", "--norestart", "--nocache",
            "--add", "Microsoft.VisualStudio.Workload.VCTools",
            "--add", "Microsoft.VisualStudio.Component.VC.CLI.Support",
            "--includeRecommended"
          )
          # 0 = success, 3010 = success but reboot required (normal in CI)
          if ($process.ExitCode -notin @(0, 3010)) {
            throw "VS 2026 Build Tools installation failed (exit code $($process.ExitCode))"
          }

      - name: Setup MSBuild (VS 2026 / v18)
        uses: microsoft/setup-msbuild@v2
        with:
          vs-version: '[18.0,19.0)'

      - name: Restore NuGet packages
        run: nuget restore syntax\cpp-cli-syntax.vcxproj

      - name: Build
        run: .\build.cmd

      - name: Test
        run: .\test.cmd
```

---

## Step-by-step explanation

### 1. Checkout

```yaml
- name: Checkout
  uses: actions/checkout@v4
```

Standard checkout. No special configuration needed — the project has no submodules and no LFS assets.

---

### 2. Install VS 2026 Build Tools

```yaml
- name: Install VS 2026 Build Tools (v145 toolset with C++/CLI support)
  shell: pwsh
  run: |
    $installer = "$env:TEMP\vs_buildtools.exe"
    Invoke-WebRequest -Uri "https://aka.ms/vs/18/release/vs_buildtools.exe" -OutFile $installer
    $process = Start-Process -FilePath $installer -Wait -PassThru -ArgumentList @(
      "--quiet", "--wait", "--norestart", "--nocache",
      "--add", "Microsoft.VisualStudio.Workload.VCTools",
      "--add", "Microsoft.VisualStudio.Component.VC.CLI.Support",
      "--includeRecommended"
    )
    if ($process.ExitCode -notin @(0, 3010)) {
      throw "VS 2026 Build Tools installation failed (exit code $($process.ExitCode))"
    }
```

**What it does:** Downloads the VS 2026 Build Tools bootstrapper from Microsoft's official short URL (`aka.ms/vs/18/release/vs_buildtools.exe`) and installs the minimum set of components needed to compile a C++/CLI .NET Core project. See [VS 2026 Build Tools components](#vs-2026-build-tools-components) for details on each component.

**The exit code check:** The VS installer returns different codes depending on outcome:

| Exit code | Meaning |
|---|---|
| `0` | Installation succeeded |
| `3010` | Succeeded, but a reboot is required — safe to ignore in CI |
| Any other | Installation failed |

**Why `Start-Process -Wait`:** The VS bootstrapper spawns a second installer process. Using `Start-Process -Wait -PassThru` waits for the entire installer chain to complete and captures the final exit code. Without `-Wait` the step would complete before installation finishes.

**Why `--nocache`:** Skips caching downloaded packages to disk, reducing disk usage on the ephemeral runner.

---

### 3. Setup MSBuild

```yaml
- name: Setup MSBuild (VS 2026 / v18)
  uses: microsoft/setup-msbuild@v2
  with:
    vs-version: '[18.0,19.0)'
```

The `microsoft/setup-msbuild` action uses `vswhere` to locate MSBuild and adds it to `PATH`. Without the `vs-version` constraint, it would find VS 2022's MSBuild (v17) first, since that is pre-installed on the runner. `[18.0,19.0)` is a NuGet-style version range meaning "VS 18.x only", which selects the newly installed VS 2026 Build Tools.

```mermaid
graph LR
    A[vswhere searches<br/>all VS installations]
    A --> B["VS 2022 / v17.x<br/>(pre-installed)"]
    A --> C["VS 2026 / v18.x<br/>(just installed)"]
    D["vs-version: [18.0,19.0)"]
    D -->|selects| C
    C --> E[MSBuild added to PATH]

    style C fill:#c8e6c9,stroke:#2e7d32
    style B fill:#eeeeee,stroke:#999
```

---

### 4. Restore NuGet packages

```yaml
- name: Restore NuGet packages
  run: nuget restore syntax\cpp-cli-syntax.vcxproj
```

Restores all `<PackageReference>` entries in the vcxproj into the NuGet cache (`%USERPROFILE%\.nuget\packages\`). This is required before building because the vcxproj's `<Reference>` HintPaths point into the NuGet cache:

```xml
<HintPath>$(USERPROFILE)\.nuget\packages\nunit\5.0.0-alpha.100.20\lib\net8.0\nunit.framework.dll</HintPath>
```

If the packages are not restored first, MSBuild cannot find the assemblies and compilation fails.

> **Why `nuget restore` and not `dotnet restore`?** This is a traditional `.vcxproj` file, not an SDK-style project. `dotnet restore` is designed for SDK-style projects (`.csproj` with `<Project Sdk="...">`). `nuget restore` with a vcxproj is the correct tool here.

---

### 5. Build

```yaml
- name: Build
  run: .\build.cmd
```

Runs `build.cmd` from the repo root, which calls MSBuild on `syntax\cpp-cli-syntax.vcxproj` and then unconditionally copies all required runtime DLLs to `syntax\Debug\`. The unconditional copy step in `build.cmd` is important: MSBuild's post-build event is skipped on incremental builds, but in CI the NuGet cache is cold and the output directory is empty, so the copies are always needed.

See [build.cmd](../build.cmd) and the [Setup Guide](cpp-cli-nunit-guide.md#post-build-copying-runtime-dependencies) for the full list of DLLs copied and why each is needed.

---

### 6. Test

```yaml
- name: Test
  run: .\test.cmd
```

Runs `test.cmd` from the repo root, which locates `vstest.console.exe` and runs it against `syntax\Debug\cpp-cli-syntax.dll`. On the CI runner, the VS 2026 path is checked first, then falls back to VS 2022. Since VS 2026 Build Tools (not the full IDE) are installed in CI, vstest comes from the pre-installed VS 2022.

A successful run looks like:

```
NUnit Adapter 1.0.0.0: Test execution started
Running all tests in ...cpp-cli-syntax.dll
   NUnit3TestExecutor discovered 34 of 34 NUnit test cases
  Passed AllItemsTests
  Passed AndOperator
  ...
Test Run Successful.
Total tests: 34
     Passed: 34
```

---

## VS 2026 Build Tools components

Only two components (plus their recommended dependencies) are installed, keeping the install as lean as possible.

| Component ID | What it provides | Why it is needed |
|---|---|---|
| `Microsoft.VisualStudio.Workload.VCTools` | The full C++ build tools workload: v145 compiler (`cl.exe`), linker, headers, Windows SDK | Core requirement — without this there is no C++ compiler |
| `Microsoft.VisualStudio.Component.VC.CLI.Support` | C++/CLI language support for the v145 toolset | Enables `/clr:netcore` compilation mode for mixed-mode assemblies |
| `--includeRecommended` | Windows SDK, CMake, ATL/MFC headers (recommended sub-components) | Pulls in the Windows SDK which provides the OS headers needed even for .NET-targeted code |

Components that are **not** installed (and not needed):

| Component | Why omitted |
|---|---|
| Full Visual Studio IDE | Not needed for command-line builds |
| `Microsoft.VisualStudio.Workload.ManagedDesktop` | C# build support — not required for C++/CLI |
| VS 2022 toolset (v143) | Not needed; project targets v145 |

---

## Expected run times

| Step | Typical duration |
|---|---|
| Checkout | < 1 min |
| Install VS 2026 Build Tools | 8–15 min |
| Setup MSBuild | < 1 min |
| Restore NuGet packages | 1–3 min (cold cache) |
| Build | 1–2 min |
| Test | < 1 min |
| **Total** | **~12–22 min** |

The VS 2026 Build Tools installation dominates the total time. This cost is paid on every run because GitHub-hosted runners are ephemeral — nothing persists between jobs.

> **Reducing install time:** GitHub Actions supports caching via `actions/cache`. However, caching a VS Build Tools installation is complex (the install path contains many files and registry entries). For a project of this size the 10–15 minute overhead is acceptable. If build times become a concern, a self-hosted Windows runner with VS 2026 pre-installed eliminates this step entirely.

---

## Troubleshooting CI failures

### MSB8020: Build tools for v145 not found

```
error MSB8020: The build tools for Visual Studio 2026 (Platform Toolset = 'v145') cannot be found.
```

The VS 2026 Build Tools installation step either failed or the `setup-msbuild` action picked up VS 2022 instead of VS 2026. Check:

1. The installer step exit code — if non-zero, the download or install failed
2. That `vs-version: '[18.0,19.0)'` is present on the `setup-msbuild` step
3. The `microsoft/setup-msbuild@v2` action version — older versions may not support the `vs-version` input

### NuGet restore fails on pre-release package

```
Unable to find version '5.0.0-alpha.100.20' of package 'NUnit'.
```

The NuGet CLI on the runner may not be searching pre-release versions. Add a `nuget.config` at the repo root that explicitly targets nuget.org:

```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <packageSources>
    <add key="nuget.org" value="https://api.nuget.org/v3/index.json" protocolVersion="3" />
  </packageSources>
</configuration>
```

### vstest.console.exe not found

```
ERROR: Could not find vstest.console.exe
```

The `test.cmd` checks for VS 2026 and VS 2022 Enterprise paths. GitHub-hosted runners install VS 2022 **Enterprise**, so the path should exist. If the runner image changes the install location, update `test.cmd` to add the new path. You can find it on the runner with:

```powershell
vswhere -latest -find **\vstest.console.exe
```

### FileNotFoundException for runtime DLLs

```
Failed to load the test assembly ...
System.IO.FileNotFoundException: Could not load file or assembly 'Microsoft.Extensions.DependencyModel'
```

The DLL copy step in `build.cmd` did not run, or a NuGet package was not restored. This typically means the NuGet restore step ran but a package was missing from nuget.org (e.g., the alpha NUnit package was delisted). Check the restore step output.

See [Troubleshooting](troubleshooting.md) for the full list of required runtime DLLs and their source packages.
