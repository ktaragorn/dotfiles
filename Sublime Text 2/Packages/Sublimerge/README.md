Sublimerge
==========

Note
----
This is Free version. For more advanced features and better performance, please visit http://www.sublimerge.com and learn about Sublimerge Pro.


About
-----
Sublimerge is a Sublime Text 2/3 plugin which allows to diff and merge files DIRECTLY in the editor using graphical interface for that purpose.

If file is under GIT or SVN it is now possible to compare it with its other revisions. This is experimental feature and can be simply
turned off via package settings (vcs_support). Please report any problems with this feature. Requires svn or git to be installed.

![Sublimerge](http://cloud.github.com/downloads/borysf/Sublimerge/Screenshot2.png "Sublimerge")

Installation
------------

To install simply do the following command in ST2 Packages' directory (requires GIT to be installed first):  
`git clone -b master git://github.com/borysf/Sublimerge.git`

Alternatively you can download `master` branch as a ZIP and extract it into `Sublimerge` directory in ST Packages' directory.

Note for Sublime Text 3 users
-----------------------------
Sublimerge is now experimentally ported for ST 3. Installation instructions can be found in branch sublime-text-3: https://github.com/borysf/Sublimerge/tree/sublime-text-3 in README.md file.


Default key bindings (platform independent)
------------------------------------------

`[ctrl]+[alt]+[d]` - open files menu to select the file you wish diff the current file to 

`[ctrl]+[alt]+[,]` - merge the change from right to left  
`[ctrl]+[alt]+[.]` - merge the change from left to right  

`[ctrl]+[alt]+[/] + [ctrl]+[alt]+[,]` - merge all changes from right to left  
`[ctrl]+[alt]+[/] + [ctrl]+[alt]+[.]` - merge all changes from left to right  

`[ctrl]+[alt]+[=]` or `[ctrl]+[alt]+[pagedown]` - go to the next difference  
`[ctrl]+[alt]+[-]` or `[ctrl]+[alt]+[pageup]` - go to the previous difference  


Access from context menus
-------------------------

You can access Sublimerge from the following context menus:
- Side Bar: one selected file  
  ![Sublimerge](http://borysforytarz.pl/img/context-one-file.png "Sublimerge")

- Side Bar: two selected files  
  ![Sublimerge](http://borysforytarz.pl/img/context-two-files.png "Sublimerge")

- Editor: View  
  ![Sublimerge](http://borysforytarz.pl/img/context-text-area.png "Sublimerge")

- Editor: File's tab  
  ![Sublimerge](http://borysforytarz.pl/img/context-tab.png "Sublimerge")  

--
http://www.sublimerge.com/
