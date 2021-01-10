### Intro
Simple tool to find C++ files with least number of dependencies - "HangingPawns". This files are good canditates to migrate beetween projects.

### Usage:
HangingPawns.exe C:\path\to\project\directory

### Dev usage:
1. Run Bootstrap.ps1 to download dependencies
2. Install "Test Adapter for Google Test"
3. In Visual Studio set Tool -> Options -> Test Adapter for Google Test -> Regex for test discovery = ".*Tests.*\.exe"