open-it(){
  if [[ $(uname -s) == 'Darwin' ]]; then
    open_cmd='open'
  else
    open_cmd='xdg-open'
  fi
  $open_cmd $@
}