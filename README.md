# Microsoft-Defender-Close



Microsoft Defender control, Disable Microsoft Defender.

亦极简的电脑管家，一键关闭 Windows Defender (Anti-Virus)。



 <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202502211954146.gif" width="80%"/>



## Support

Windows 10 / 11 +, Other windows versions not tested.  

Supports Qt5 and Qt6 compilation builds, successfully compiled in both qt5.15 and qt6.7.3.



<br>

![](https://img.shields.io/github/v/release/XMuli/microsoft-defender-close?style=flat&color=birightgreen) [![alt text](https://img.shields.io/badge/IM-XMuli-ff69b4)](https://sighttp.qq.com/authd?IDKEY=31f3ef7312b39e2c8dc822ae2f4c3b3118e1a6f31cc83373) ![](https://img.shields.io/github/stars/XMuli/microsoft-defender-close?style=social) ![](https://img.shields.io/github/forks/XMuli/microsoft-defender-close?style=social&label=Fork)

![](https://img.shields.io/github/workflow/status/XMuli/microsoft-defender-close/Windows?style=flat&logo=windows)  ![](https://img.shields.io/github/languages/code-size/XMuli/microsoft-defender-close)  ![](https://img.shields.io/github/downloads/XMuli/microsoft-defender-close/total)

<br>

## Validate the effect

- `gpedit.msc`

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2022/202208211542062.png" width="80%"/>

<br>

- Task Manager
  - `Services - WinDefend`  services  `Status` 
  - `Dedails - MsMpEng.exe`  process  `Status` 



## Exe signatures:

Check the signature of the file attributes to make sure you download the official file.

<img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202502211959316.png" width="50%"/>
