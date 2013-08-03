#include "htmlbutcher-locale-files.iss"

[Setup]
AppName=HTMLButcher
AppVerName=HTMLButcher 1.1
DefaultDirName={pf}\HTMLButcher
DefaultGroupName=HTMLButcher
SourceDir=..\..
OutputBaseFilename=htmlbutcher_1.1_win32
OutputDir=setup\win32
ChangesAssociations=yes
VersionInfoVersion=1.1.0.28
UninstallDisplayIcon={app}\htmlbutcher.exe
UninstallDisplayName=HTMLButcher

[Registry]
Root: HKCR; Subkey: ".hbp"; ValueType: string; ValueName: ""; ValueData: "HTMLButcher"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "HTMLButcher"; ValueType: string; ValueName: ""; ValueData: "HTMLButcher"; Flags: uninsdeletekey
Root: HKCR; Subkey: "HTMLButcher\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\htmlbutcher.exe,0"
Root: HKCR; Subkey: "HTMLButcher\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\htmlbutcher.exe"" ""%1"""

[Files]
Source: "bin\Release\htmlbutcher.exe"; DestDir: "{app}"
Source: "doc\docbook\htmlhelp\htmlbutcher.chm"; DestDir: "{app}"
Source: "m:\prog\src\FreeImage\Dist\FreeImage.dll"; DestDir: "{app}"
Source: "setup\win32\redist\vcredist_x86.exe"; DestDir: "{tmp}"; Flags: deleteafterinstall
#emit LocaleFiles

[Icons]
Name: "{group}\HTMLButcher"; Filename: "{app}\htmlbutcher.exe"; WorkingDir: "{app}"
Name: "{group}\HTMLButcher manual"; Filename: "{app}\htmlbutcher.chm"; WorkingDir: "{app}"
Name: "{group}\Uninstall HTMLButcher"; Filename: "{uninstallexe}"

[Run]
Filename: "{tmp}\vcredist_x86.exe"; Parameters: "/q"; Description: "Visual Studio Redistributable"; StatusMsg: "Installing Visual Studio Redistributable..."

