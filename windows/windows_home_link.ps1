# Needs admin window

new-item -path C:\Users\karth\dotfiles -value  \\wsl.localhost\Ubuntu\home\karthikt\dotfiles -ItemType SymbolicLink -Force
new-item -path 'C:\Users\karth\AppData\Roaming\Sublime Text 3\Packages\User' -value 'C:\Users\karth\dotfiles\Sublime Text 3\Packages\User'  -ItemType SymbolicLink -Force

.\windows_link.ps1 karth