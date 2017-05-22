# ocm2ozi
OpenCycleMap tiles downloader & OZI calibration data generator in order to use OpenCycleMap data offline.

This win32 console application based on SOCKET example from StackOverflow.
Thunderforest's apikey required to compile the application. Anyone can obtain it by signing up at http://www.thunderforest.com/ then fill it into urlfmt string.

Example of usage:
1) obtain coordinates in degrees for bounding box of map you want to download, for example click on any place at maps.google.com and watch the box at the bottom where lattitude(y) and longtitude(x) are shown;
2) run the program and supply filenames prefix, zoom index and bounding box coordinates from command line;
3) merge downloaded tiles into single map by free OziMapMerge utility (http://www.oziexplorer3.com/mapmerge/mapmerge.html);
4) load the resulting map within navigation application, for example free Androzic (http://androzic.com/).
