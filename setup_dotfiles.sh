DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#gitconfig
rm ../.gitconfig
ln -sfn $DIR/gitconfig $DIR/../.gitconfig

#bashrc
echo ". ~/dotfiles/bashrc.d/source_all" >> ~/.bashrc
ln -sfn ~/dotfiles/bashrc.d/inputrc ~/.inputrc

#byobu specific
ln -snf $DIR/tmux.conf $DIR/../.byobu/.tmux.conf

#rdebug
ln -snf ~/dotfiles/rdebugrc ~/.rdebugrc

#vim
ln -snf ~/dotfiles/vimrc ~/.vimrc

#ngrok
ln -sfn ~/dotfiles/ngrok ~/.ngrok
