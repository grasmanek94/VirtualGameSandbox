This allows you to distribute big-ass games with many many files (which slow down transfer really much)
and to configure them before launch in a programatic way so it will always work.
Especially usefull for LAN parties. Can be extended for other applications.
Some games refuse to use the VHD appdata, programfiles, documents folders etc.. 
but that's what we have injectors for. (loaders)

Instructions:
Unpack .VHD to any directory and name it "game.vhd"
Place LaunchGame.exe where game.vhd is
Execute LaunchGame.exe
Open newly created drive
Put game files on VHD
Optional: Create configuration.cxx on root of newly created drive 
and use angelscript to script initial pre-game lauch config
Close the console window
Configure newly created config.cfg

License: zlib
