# Viy
Note taking featuring Neovim, pictures and screenshots
Reddit announcement: https://github.com/meatich/Viy

##The idea 
Have a single folder where you keep all your textual and visual notes. This folder is called "library". Provide an easy way to take and access screenshots.
###Images
Images are kept inside folder called "images". On each program's start-up, these images are autoloaded by the program, which prevents lag when encountering one in a text file.
Now, how do you embed an image into a text file? You put a tag which looks as follows:
```
<|ImageName.png|image size|image fragment|>
```
So, an actual tag could look like this:
```
<|NiceComment.png|4|0|>
```
Then, you put your cursor on the tag and expand it by :call PicExpand() or using a mapping like one offered in the "Neovim settings" section. The tag expands to the following:
```
<|PinkFloyd.png|4|0|>
<|PinkFloyd.png|4|1|>
<|PinkFloyd.png|4|2|>
<|PinkFloyd.png|4|3|>
```
The program, upon encountering one of such tags, replaces it with a picture fragment.

Tags for all images can be viewed in library/images/ImageList.txt
###Navigator
The navigator is opened by default when the program starts. Only the files inside the library are scanned and so only they may be viewed in the navigator.
Also, you can search library by pressing <F2>.
To prevent Navigator to show up on program's launch, remove the following line from build_windows/gui_bridge.py:
```
self._ui.switch_to_navigator()
```
### Screenshots
To take a screenshot, press F5. Select the necessary area and press Enter. The screenshot will be saved at library/images and a tag will be added to library/images/ImageList.txt from where you can copy it.

##Neovim settings
neosfml.vim should be put into autoload directory or sourced from init.vim
Here is what you could include in init.vim:
```
"RPC for GUI
"Switch to built-in navigator
nmap <Leader>n :call rpcrequest(g:NeoSFMLGUIChannelID, "switchToNavigator")<CR>
"Insert a picture tag
nmap <Leader>pi i<\|Picture.png\|10\|0\|>
"Expand a picture tag under cursor
nmap <Leader>pe :call neosfml#PicExpand()<Return>
"Find and expand all folded picture tags
nmap <Leader>pa :call neosfml#PicExpandAll()<Return>
```

##Shortcuts
* F5  Take screenshot
* F11 Fullscreen

##Installation instructions:
1. Install NeoSFML (follow the first four steps if you don't want NeoSFML itself)
2. Download https://github.com/meatich/Viy/releases/tag/0.6 and run it

##Compilation
Similar to NeoSFML, but also need to include Lua 5.1
