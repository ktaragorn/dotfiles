github-open(){
	url=$(git remote -v | grep origin |head -1 | cut -d ' '  -f 1 | cut -f2|sed 's/:/\//' |sed 's/git\@/http\:\/\//' )
	open-it $url
}
