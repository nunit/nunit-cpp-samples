# C++/CLI NUnit Documentation

Documentation for the `cpp-cli-syntax` sample project, which demonstrates NUnit testing of a C++/CLI (.NET Core) assembly.

## Articles

| Document | Description |
|---|---|
| [Setup Guide](docs/cpp-cli-nunit-guide.md) | Complete walkthrough: project configuration, package references, writing tests, build and test scripts |
| [Troubleshooting](docs/troubleshooting.md) | Solutions for the specific failures that occur with this setup — hanging testhost, missing DLLs, NUnit 4/5 API changes |
| [GitHub Actions CI](docs/github-actions-ci.md) | How to run CI on GitHub Actions: why VS 2026 Build Tools must be installed, the full annotated workflow, component IDs, and expected timings |

## Quick reference

```
build.cmd   →  compiles the project + copies runtime DLLs to Debug\
test.cmd    →  runs vstest.console.exe against Debug\cpp-cli-syntax.dll
```

All 34 tests should pass on a successful build.
