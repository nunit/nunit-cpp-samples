## Build Scripts

Each of the sample solutions can be built individually within Visual Studio. To build and test all solutions, a **Cake** (http://cakebuild.net) script is provided.
The primary script that controls this is build.cake. We modify build.cake when we need to add new 
targets or change the way the build is done. Normally build.cake is not invoked directly but through
build.ps1. (Due to lack of mono support for C++, there is currently only a Windows build process.) 
This script is provided by the Cake project and ensures that Cake is properly installed before trying to run the cake script. This helps the
build to work on CI servers using newly created agents to run the build. We generally run it
the same way on our own machines.

The build.cmd script is provided as an easy way to run build.ps1, from the command line.
In addition to passing the arguments through to build.cake, it can supply added arguments
through the CAKE_ARGS environment variable. The rest of this document will assume use of this script.

There is one case in which use of the CAKE_ARGS environment variable will be essential, if not necessary.
If you are running builds on a 32-bit Windows system, you must always supply the -Experimental argument
to the build. Use set CAKE_ARGS=-Experimental to ensure this is always done and avoid having to type
it out each time.

Key arguments to build.cmd / build:
 * -Target, -t <task>                 The task to run - see below.
 * -Configuration, -c [Release|Debug] The configuration to use (default is Debug).
 * -Experimental, -e                  Use the experimental build of Roslyn.

The build.cake script contains a number of interdependent tasks. The most 
important top-level tasks to use are listed here:

```
 * Build               Builds everything. This is the default if no target is given.
 * Rebuild             Cleans the output directory and builds everything.
 * Test                Runs all tests. Dependent on Build.
```