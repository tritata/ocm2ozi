# ocm2ozi
OpenCycleMap tiles downloader & OZI calibration data generator in order to use OpenCycleMap data offline.

This win32 console application based on SOCKET example from StackOverflow. Thus application able to establish connection to OpenCycleMap server on direct internet connections only. Connection through proxy server isn't supported.
Thunderforest's apikey required to compile the application. Anyone can obtain it by signing up at http://www.thunderforest.com/ then fill it into urlfmt string.

Example of usage:
1) obtain coordinates in degrees for bounding box of map you want to download, for example click on any place at maps.google.com and watch the box at the bottom where lattitude(y) and longtitude(x) are shown;
2) run the program suppling by command line arguments: filenames' prefix, zoom index and bounding box coordinates;
3) merge downloaded tiles into single map by free OziMapMerge utility (http://www.oziexplorer3.com/mapmerge/mapmerge.html);
4) load the resulting map within navigation application, for example free Androzic (http://androzic.com/).
