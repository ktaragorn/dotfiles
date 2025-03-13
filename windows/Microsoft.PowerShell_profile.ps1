
# trying https://learn.microsoft.com/en-us/windows/terminal/tutorials/new-tab-same-directory 
# from https://github.com/dahlbyk/posh-git/issues/501
function prompt{
    $prompt = &$GitPromptScriptBlock
    $loc = $executionContext.SessionState.Path.CurrentLocation;

    $out = ""
    if ($loc.Provider.Name -eq "FileSystem") {
        $out += "$([char]27)]9;9;`"$($loc.ProviderPath)`"$([char]27)\"
    }
    
    return $out + $prompt
}
Import-Module posh-git
# 'C:\tools\poshgit\dahlbyk-posh-git-9bda399\src\posh-git.psd1'

# https://stackoverflow.com/a/62964720
Set-PSReadlineKeyHandler -Key Tab -Function MenuComplete

Set-PSReadlineKeyHandler -Key UpArrow -Function HistorySearchBackward
Set-PSReadlineKeyHandler -Key DownArrow -Function HistorySearchForward

Set-PSReadLineOption -HistorySearchCursorMovesToEnd
   
new-alias grep Select-String