#https://conemu.github.io/en/GlobalHotKeys.html#Using_F12_as_global_hotkey

$path='HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug';

Set-ItemProperty -Path $path -Name "UserDebuggerHotKey" -Value 0x00000013;