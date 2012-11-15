# Simple svn & git bash prompt
# source: http://www.codeography.com/2009/05/26/speedy-bash-prompt-git-and-subversion-integration.html
# see also: http://superuser.com/questions/288438/bash-completion-for-commands-in-mac-os
#
# Dependencies:
#   brew install git              # to get __git_ps1 in git_completion.sh
#   brew install bash-completion  # to load git_completion.sh
scm_ps1() {
    local s=
    if [[ -d ".svn" ]] ; then
        s=\(r$(svn info | sed -n -e '/^Revision: \([0-9]*\).*$/s//\1/p' )\)
    else
        s=$(__git_ps1 "(%s)")
    fi
    echo -n "$s"
}

# Reset bash prompt
export PS1="\[\033[00;32m\]\u\[\033[00;32m\]@\[\033[00;32m\]\h:\[\033[0;37m\]\W \[\033[0;36m\]\$(scm_ps1)\[\033[00m\]$\[\033[00m\] "
