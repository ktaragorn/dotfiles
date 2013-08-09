DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#gitconfig
rm ../.gitconfig
ln -sn $DIR/gitconfig $DIR/../.gitconfig

#bashrc
echo ". dotfiles/bashrc.d/source_all" >> ~/.bashrc

