#include "htmlbutcher-locale-files.iss"

[Setup]
AppName=HTMLButcher DEMO
AppVerName=HTMLButcher DEMO 1.1
DefaultDirName={pf}\HTMLButcher DEMO
DefaultGroupName=HTMLButcher DEMO
SourceDir=..\..
OutputBaseFilename=htmlbutcher_1.1_demo_win64
OutputDir=setup\win64
VersionInfoVersion=1.1.0.28
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64

[Files]
Source: "bin\Release\htmlbutcherdemo.exe"; DestDir: "{app}"
Source: "doc\docbook\htmlhelp\htmlbutcher.chm"; DestDir: "{app}"
Source: "m:\prog\src\x64\FreeImage\Dist\FreeImage.dll"; DestDir: "{app}"
Source: "setup\win64\redist\vcredist_x64.exe"; DestDir: "{tmp}"; Flags: deleteafterinstall
#emit LocaleFiles

[Icons]
Name: "{group}\HTMLButcher"; Filename: "{app}\htmlbutcherdemo.exe"; WorkingDir: "{app}"
Name: "{group}\HTMLButcher manual"; Filename: "{app}\htmlbutcher.chm"; WorkingDir: "{app}"
Name: "{group}\Uninstall HTMLButcher"; Filename: "{uninstallexe}"

[Run]
Filename: "{tmp}\vcredist_x64.exe"; Parameters: "/q"; Description: "Visual Studio Redistributable"; StatusMsg: "Installing Visual Studio Redistributable..."

