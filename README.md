# ninjhax2-dx
Based on: https://github.com/smealum/ninjhax2.x  

This is a fork of ninjhax2x that restores 1 QR scanning functionality for the latest 3DS firmwares. Due to Nintendo removing the http:C and soc:U services from Cubic Ninja in firmware 11.0 as a half-hearted anti-homebrew measure, developer Smealum crafted a clever workaround involving scanning up to 16 QRs in a single session to download the necessary 30KB payload file through the camera instead of online. 

This version aims to improve on that method by cramming the necessary home menu takover code into a single QR code and immediately loading a *hax ropbin from the sd card (named Launcher.dat, remember those?). Although this hax method does not actually install anything to Cubic Ninja's savegame for repeated Ninjhax use, that's pretty much not an issue as you can just immediately install full, system-wide CFW by launching a .3dsx file such as ulauncher.3dsx. Then, you can run all the homebrew you like, and it's a lot more stable.
# Directions
Prep: You need a USA, EUR, or JPN old/new 3DS and a copy of Cubic Ninja for your particular region. USA/JPN supports 11.7.0-XX -> latest firmware and EUR supports 11.10.0-XX -> latest firmware. If you are below these minimums, you can internet update (although check online if the latest firmware is still safe for homebrew. As of June 2022 and firm 11.15.0-XX, it is still safe).

1) Download the following files and place them on your sdmc card where indicated:  
- sdmc:/Nintendo 3DS/  		(this already on your 3DS -- its just for your reference on where everything else goes)  
- sdmc:/Launcher.dat  		(from https://github.com/zoogie/ninjhax2-dx/releases)  
- sdmc:/boot.3dsx  		(from https://github.com/LumaTeam/Luma3DS/releases)  
- sdmc:/boot.firm  		(also from https://github.com/LumaTeam/Luma3DS/releases)  
- sdmc:/boot9strap/boot9strap.firm  (from https://github.com/SciresM/boot9strap/releases)  
- sdmc:/boot9strap/boot9strap.firm.sha  (also from https://github.com/SciresM/boot9strap/releases)  
- sdmc:/3DS/uloader.3dsx		(from https://github.com/zoogie/uloader/releases)  
- sdmc:/SafeB9Sinstaller.bin  	(from https://github.com/d0k3/SafeB9SInstaller/releases)  

2) Go to https://zoogie.github.io/web/nh2dx/ and select the proper QR in the table, then scan it with Cubic Ninja's level editor. The homebrew menu should load. It may fail about 25% of the time, just turn the system off and try again if it freezes.

3) Select the uloader.3dsx app from then homebrew menu. Follow the on screen prompts and you should then have full cfw!

4) Go to https://3ds.hacks.guide/finalizing-setup.html to get a more thorough setup on your 3DS. Enjoy!

# Thanks
* Smealum - for Ninjhax, libctru and many other cool 3DS things over the years
* Everyone - best developer ever :p
