#pragma once

char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut);
long downloadFile(char *url, char *filename);
