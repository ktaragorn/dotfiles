DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#gitconfig
rm ../.gitconfig
ln -sn $DIR/gitconfig $DIR/../.gitconfig

#bashrc
echo ". ~/dotfiles/bashrc.d/source_all" >> ~/.bashrc
ln ~/dotfiles/bashrc.d/inputrc ~/.inputrc

#byobu specific
ln -sn $DIR/tmux.conf $DIR/../.byobu/.tmux.conf

#rdebug
ln -sn ~/dotfiles/rdebugrc ~/.rdebugrc

#vim
ln -sn ~/dotfiles/vimrc ~/.vimrc

#ngrok
ln -sn ~/dotfiles/ngrok ~/.ngrok
