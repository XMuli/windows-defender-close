; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!
#pragma parseroption -p
#define MyAppName "Microsoft-Defender-Close"
#define MyAppVersion "2.0.0"
#define MyAppPublisher "Wengeng Zhang"
#define MyAppCompany "XMuli Team"
#define MyAppURL "https://Microsoft-Defender-Close.xmuli.tech/"
#define MyAppExeName "Microsoft-Defender-Close.exe"                     
#define MyArchitecture "x64"   ; x64    x86
#define MySrc "."
#define MyBinDir "bin"         ;
#define MyOutputDir "./"
#define MyGUID "{1EA12F61-7E72-4A66-A0C8-5BFA2B9A1BD9}"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{#MyGUID}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
;LicenseFile={#MyBinDir}\resources\licenses\License.md
;InfoBeforeFile={#MyBinDir}\resources\licenses\PrivacyAndDataProtection.md
;InfoAfterFile={#MyBinDir}\resources\licenses\other.md
UninstallDisplayIcon={app}\{#MyAppExeName}
UninstallDisplayName={#MyAppName}

; Uncomment the following line to run in non-administrative install mode (install for current user only.)
; PrivilegesRequiredOverridesAllowed 
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=commandline   
OutputDir={#MyOutputDir}
OutputBaseFilename={#MyAppName}_setup_{#MyAppVersion}_{#MyArchitecture}
;SetupIconFile={#MyBinDir}\resources\logo\logo.ico
Password=
Compression=lzma
SolidCompression=yes
WizardStyle=modern

;SetupLogging=yes


[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "armenian"; MessagesFile: "compiler:Languages\Armenian.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "bulgarian"; MessagesFile: "compiler:Languages\Bulgarian.isl"
Name: "catalan"; MessagesFile: "compiler:Languages\Catalan.isl"
Name: "chinesesimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"
Name: "chinesetraditional"; MessagesFile: "compiler:Languages\ChineseTraditional.isl"
Name: "corsican"; MessagesFile: "compiler:Languages\Corsican.isl"
Name: "czech"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "danish"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "finnish"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "hebrew"; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: "hungarian"; MessagesFile: "compiler:Languages\Hungarian.isl"
Name: "icelandic"; MessagesFile: "compiler:Languages\Icelandic.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "norwegian"; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "portuguese"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "slovak"; MessagesFile: "compiler:Languages\Slovak.isl"
Name: "slovenian"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "turkish"; MessagesFile: "compiler:Languages\Turkish.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "{#MyBinDir}\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBinDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Registry]
Root: HKCU; Subkey: "Software\{#MyAppCompany}\{#MyAppName}"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
function VC2017RedistNeedsInstall: Boolean;
var 
  szArchitecture, szAppName, szExecutable, szParam, Version: string;
  bFileExists: boolean;
  nRetCode: Integer;
   
begin
  szArchitecture := '{#MyArchitecture}';
  if szArchitecture  = 'x64' then
    begin
      szAppName := 'vc_redist.x64.exe';
    end
  else
    begin
      szAppName := 'vc_redist.x86.exe';
    end;

  szExecutable := ExpandConstant('{app}\') + szAppName;
  bFileExists := FileExists(szExecutable);
  
  if RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Wow6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x64', 'Version', Version) then
  begin
    // Is the installed version at least 14.14 ? 
    Log('VC Redist Version check : found ' + Version);
    Result := (CompareStr(Version, 'v14.14.26429.03')<0);
  end
  else 
  begin
    // Not even an old version installed
    Result := True;
  end;

  if (Result) then
    begin
		Log('VC_redist is install');
		szParam := '/install /quiet /norestart';
		Exec(szExecutable, szParam, '', SW_HIDE, ewWaitUntilTerminated, nRetCode);
		Sleep(100);
    end;
	
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  logfilepathname, logfilename, newfilepathname: string;
begin
  if CurStep = ssPostInstall then
  begin
    VC2017RedistNeedsInstall();
    Log('RESULTCODE=0');
  end;

  logfilepathname := ExpandConstant('{log}');
  logfilename := ExtractFileName(logfilepathname);
  newfilepathname := ExpandConstant('{app}\') + logfilename;

  if CurStep = ssDone then
  begin
    FileCopy(logfilepathname, newfilepathname, false);
  end;
end;
