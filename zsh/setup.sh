cd ~
ln -snf dotfiles/zsh/zshrc ./.zshrc

cd .oh-my-zsh/custom
ln -shf ~/dotfiles/zsh/plugins .
ln -snf ~/dotfiles/zsh/themes .

