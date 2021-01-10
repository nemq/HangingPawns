### Intro
Simple tool to find C++ files with least number of dependencies - "HangingPawns". This files are good canditates to migrate beetween projects.

### Usage:
HangingPawns.exe C:\path\to\project\directory

### Dev usage:

1. Run Bootstrap.ps1 to download dependencies
2. Install "Test Adapter for Google Test"
3. In Visual Studio set Tool -> Options -> Test Adapter for Google Test -> Regex for test discovery = ".*Tests.*\.exe"

### Packaging dependencies

0. vcpkg.exe install  boost-algorithm:x64-windows gtest:x64-windows
1. vcpkg.exe export --raw boost-algorithm:x64-windows gtest:x64-windows boost-algorithm:x64-windows gtest:x64-windows
2. Zip
3. Upload to dropbox
4. Update Bootstrap.ps1 $URL