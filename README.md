# ninjhax2-dx
Based on: https://github.com/smealum/ninjhax2.x  

This is a fork of ninjhax2x that restores 1 QR scanning functionality for the latest 3DS firmwares. Due to Nintendo removing the http:C and soc:U services from Cubic Ninja in firmware 11.0 as a half-hearted anti-homebrew measure, developer Smealum crafted a clever workaround involving scanning up to 16 QRs in a single session to download the necessary 30KB payload file through the camera instead of online. 

This version aims to improve on that method by cramming the necessary home menu takover code into a single QR code and installing it into a tiny save file on Cubic Ninja. This exploited QR payload data further loads a menuhax ropbin from sdmc that finishes the hbmenu loading process (these are similar to the popular otherapp files).
# Directions
Prep: You need a USA, EUR, or JPN old/new 3DS and a copy of Cubic Ninja for your particular region. USA/JPN supports 11.7.0-XX -> latest firmware and EUR supports 11.10.0-XX -> latest firmware. If you are below these minimums, you can internet update (although check online if the latest firmware is still safe for homebrew. As of June 2022 and firm 11.15.0-XX, it is still safe). There are also a few cartridge games that can update you to the minimum firm level, but they are rare (nonexistant for Europe).

1) Download the following files and place them on your sdmc card where indicated:  
- sdmc:/Nintendo 3DS/  		(this already on your 3DS -- its just for your reference on where everything else goes)  
- sdmc:/Launcher.dat  		(from https://github.com/zoogie/ninjhax2-dx/releases)  
- sdmc:/boot.3dsx  		(from https://github.com/LumaTeam/Luma3DS/releases)  
- sdmc:/boot.firm  		(also from https://github.com/LumaTeam/Luma3DS/releases)  
- sdmc:/boot9strap/boot9strap.firm  (from https://github.com/SciresM/boot9strap/releases)  
- sdmc:/boot9strap/boot9strap.firm.sha  (also from https://github.com/SciresM/boot9strap/releases)  
- sdmc:/3DS/uloader.3dsx		(from https://github.com/zoogie/uloader/releases)  
- sdmc:/SafeB9Sinstaller.bin  	(from https://github.com/d0k3/SafeB9SInstaller/releases)  

2) Go to https://zoogie.github.io/web/nh2dx/ and select the proper QR in the table, then scan it with Cubic Ninja's level editor (from Menu, select Create -> QR Code -> Scan QR Code). The game should briefly freeze, quit and fall back to home menu if the exploit installed correctly (without errors). Now, just reboot Cubic Ninja, and repeat the QR scan instructions. The hbmenu should load. It may fail about 25% of the time, this is normal. Just turn the system off and try again if it freezes.  

3) Select the uloader.3dsx app from then homebrew menu. Follow the on screen prompts and you should then have full cfw!

4) Go to https://3ds.hacks.guide/finalizing-setup.html to get a more thorough setup on your 3DS. Enjoy!
<hr>
* BONUS - Optional Lite version: You can also run a version of Ninjhax2 DX that doesn't install anything to the savegame, it just boots the hbmenu once per scan. Use https://zoogie.github.io/web/nh2dx/lite.html for that option. The directions are the same except the hbmenu loads when the QR scans vs. the game falling back to home menu, and no need to reboot the game unless you want to run the exploit again.

* Note: If you encounter a situtation where the exploit doesn't work after repeated tries or you scan the wrong QR, you can delete Cubic Ninja's savegame. At the "Menu" screen, hold L+R+X+Y for a few seconds until a prompt comes up. Then select YES twice to uninstall the hax/savegame.

* Note2: If you run this exploit and encounter a black "An exception occurred" screen with a lot of numbers on both screens, then this means you already have boot9strap cfw, and you shouldn't be running userland (basic) homebrew exploits like Ninjhax.

# Thanks
* Smealum - for Ninjhax, libctru and many other cool 3DS things over the years
* Luigoalma - there's no obscure compiler error that this foxo can't fix!
* Everyone - best developer ever :p
