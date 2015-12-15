path "c:\projects\eclipse kepler 64";%path% 
eclipsec -nosplash -data workspace\HeadlessBuild -application org.eclipse.cdt.managedbuilder.core.headlessbuild -import FEC2 -import Common -import FEC2_BOOT -build FEC2/Release -build Fec2Bootloader/Debug 
  