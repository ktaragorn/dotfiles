param([string]$userName)

# Needs admin window
new-item -path C:\Users\$userName\Documents\PowerShell\Microsoft.PowerShell_profile.ps1 -value C:\Users\$userName\dotfiles\windows\Microsoft.PowerShell_profile.ps1 -ItemType SymbolicLink -Force

new-item -path C:\Users\$userName\AppData\Local\Packages\Microsoft.WindowsTerminal_8wekyb3d8bbwe\LocalState\settings.json -value C:\Users\$userName\dotfiles\windows\settings.json  -ItemType SymbolicLink -Force  

new-item -path "C:\Users\$userName\AppData\Local\Microsoft\PowerToys\Keyboard Manager\default.json" -value C:\Users\$userName\dotfiles\windows\default.json  -ItemType SymbolicLink -Force

new-item -path C:\Users\$userName\AppData\Roaming\Code\User\settings.json -value C:\Users\$userName\dotfiles\windows\vscode\settings.json  -ItemType SymbolicLink -Force
new-item -path C:\Users\$userName\AppData\Roaming\Code\User\keybindings.json -value C:\Users\$userName\dotfiles\windows\vscode\keybindings.json  -ItemType SymbolicLink -Force
