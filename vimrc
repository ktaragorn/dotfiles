set nocompatible              " be iMproved, required
filetype off                  " required

" Setting up Vundle - the vim plugin bundler
    let iCanHazVundle=1
    let vundle_readme=expand('~/.vim/bundle/vundle/README.md')
    if !filereadable(vundle_readme)
        echo "Installing Vundle.."
        echo ""
        silent !mkdir -p ~/.vim/bundle
        silent !git clone https://github.com/gmarik/vundle ~/.vim/bundle/vundle
        let iCanHazVundle=0
    endif
    set rtp+=~/.vim/bundle/vundle/
    call vundle#rc()
    Plugin 'gmarik/vundle'

    "Add your bundles here
    Plugin 'Syntastic' "uber awesome syntax and errors highlighter
    Plugin 'altercation/vim-colors-solarized' "T-H-E colorscheme
    "Plugin 'https://github.com/tpope/vim-fugitive' "So awesome, it should be illegal 
    "...All your other bundles...
    Plugin 'Raimondi/delimitMate'
    Plugin 'ervandew/supertab'

    if iCanHazVundle == 0
        echo "Installing Bundles, please ignore key map error messages"
        echo ""
        :BundleInstall
    endif
" Setting up Vundle - the vim plugin bundler end

filetype plugin indent on    " required"
syntax on 
set mouse=a
"http://stackoverflow.com/questions/2514445/turning-off-auto-indent-when-pasting-text-into-vim
set pastetoggle=<F10>

" To set shift - tab to reverse tab, not working still with super tab
"http://stackoverflow.com/questions/4766230/map-shift-tab-in-vim-to-inverse-tab-in-vim
" for command mode
nmap <S-Tab> <<
" " for insert mode
imap <S-Tab> <Esc><<i
 
"http://nvie.com/posts/how-i-boosted-my-vim/
set autoindent    " always set autoindenting on
"set copyindent    " copy the previous indentation on autoindenting
set number        " always show line numbers
set incsearch     " show search matches as you type

"TODO
"Fix ctrl c ctrl v stuff
"Fix auto indent paste stuff http://stackoverflow.com/questions/2514445/turning-off-auto-indent-when-pasting-text-into-vim
"SHift tab?

"not working?
"http://rishabhsays.wordpress.com/2010/07/03/vim-for-lazy-people/
"CTRL-X is cut
vnoremap <C-X> "+x

"CTRL-C is copy
vnoremap <C-C> "+y

"CTRL-V is paste
vnoremap <C-V> "+gP


"autocomplete menu
set wildmenu
set wildmode=list:longest,full

"http://mirnazim.org/writings/vim-plugins-i-use/
set background=dark
let g:solarized_termtrans=1
let g:solarized_termcolors=256
let g:solarized_contrast="high"
let g:solarized_visibility="high"
colorscheme solarized
