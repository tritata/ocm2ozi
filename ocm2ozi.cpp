#include <windows.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include "http.h"

// one should register at thunderforest.com, obtain apikey and replace ### with it
const char urlfmt[]="https://tile.thunderforest.com/cycle/%d/%d/%d.png?apikey=#################################"; // zoom, x, y

bool signbit(float val){
	if(0x80000000&(unsigned int &)val)
		return true;
	else
		return false;
}

float frac(float val){
	if(signbit(val))
		return ceilf(val)-val;
	else
		return val-floorf(val);
}

void writeMap(char *filename, float cl, float ct, float cr, float cb, float mr, float mb){
	const float mpd=40075696.0f/360.0f;
	float mppx, mppy, mpp;
	mppx=fabs(mpd*(cr-cl)/255.0f);
	mppy=fabs(mpd*(cb-ct)/255.0f);
	mpp=mppx < mppy? mppx: mppy;
	int len=strlen(filename);
	strncpy(filename+len-3, "map", 3);
	char *name=strrchr(filename, '\\');
	if(!name)
		name=filename;
	else
		++name;
	if(strrchr(filename, '/') > name)
		name=strrchr(filename, '/')+1;

	FILE *file=fopen(filename, "wt");
	strncpy(filename+len-3, "png", 3);
	fputs("OziExplorer Map Data File Version 2.2\n", file);
	fprintf(file, "%s\n", name);
	fprintf(file, "%s\n", filename);
	fputs("1 ,Map Code,\n", file);
	fputs("WGS 84,WGS 84, 0.0000, 0.0000, WGS 84\n", file);
	fputs("Reserved 1\n", file);
	fputs("Reserved 2\n", file);
	fputs("Magnetic Variation,,,E\n", file);
	fputs("Map Projection,Mercator,PolyCal,No,AutoCalOnly,No,BSBUseWPX,No\n", file);
	fprintf(file, "Point01, xy,   0,   0, in, deg, %d, %f, %c, %d, %f, %c, grid,,,, %c\n", 
		int(fabs(floorf(ct))), 60.0f*frac(ct), signbit(ct)? 'S': 'N',
		int(fabs(floorf(cl))), 60.0f*frac(cl), signbit(cl)? 'W': 'E',
		signbit(ct)? 'S': 'N');
	fprintf(file, "Point02, xy, 255,   0, in, deg, %d, %f, %c, %d, %f, %c, grid,,,, %c\n",
		int(fabs(floorf(ct))), 60.0f*frac(ct), signbit(ct)? 'S': 'N',
		int(fabs(floorf(cr))), 60.0f*frac(cr), signbit(cr)? 'W': 'E',
		signbit(ct)? 'S': 'N');
	fprintf(file, "Point03, xy,   0, 255, in, deg, %d, %f, %c, %d, %f, %c, grid,,,, %c\n",
		int(fabs(floorf(cb))), 60.0f*frac(cb), signbit(cb)? 'S': 'N',
		int(fabs(floorf(cl))), 60.0f*frac(cl), signbit(cl)? 'W': 'E',
		signbit(ct)? 'S': 'N');
	fprintf(file, "Point04, xy, 255, 255, in, deg, %d, %f, %c, %d, %f, %c, grid,,,, %c\n",
		int(fabs(floorf(cb))), 60.0f*frac(cb), signbit(cb)? 'S': 'N',
		int(fabs(floorf(cr))), 60.0f*frac(cr), signbit(cr)? 'W': 'E',
		signbit(ct)? 'S': 'N');
	fputs("Projection Setup,,,,,,,,,,\n", file);
	fputs("Map Feature = MF ; Map Comment = MC     These follow if they exist\n", file);
	fputs("Track File = TF      These follow if they exist\n", file);
	fputs("Moving Map Parameters = MM?    These follow if they exist\n", file);
	fputs("MM0,Yes\n", file);
	fputs("MMPNUM,4\n", file);
	fputs("MMPXY,1,0,0\n", file);
	fputs("MMPXY,2,256,0\n", file);
	fputs("MMPXY,3,256,256\n", file);
	fputs("MMPXY,4,0,256\n", file);
	fprintf(file, "MMPLL,1, %f, %f\n", cl, ct);
	fprintf(file, "MMPLL,2, %f, %f\n", mr, ct);
	fprintf(file, "MMPLL,3, %f, %f\n", mr, mb);
	fprintf(file, "MMPLL,4, %f, %f\n", cl, mb);
	fprintf(file, "MM1B,%f\n", mpp);
	fputs("MOP,Map Open Position,0,0\n", file);
	fputs("IWH,Map Image Width/Height,256,256\n", file);
	fclose(file);
}

int main(int argc, char* argv[]){
	if(argc != 7){
		puts("usage: ocm2ozi.exe p z l t r b");
		puts("\tp - path prefix");
		puts("\tz - zoom level [0; 22]");
		puts("\tl, r - left & right region boundaries [-180.0; 180.0], east positive");
		puts("\tt, b - top & bottom boundaries [-85.0511; 85.0511], north positive");
		return -1;
	}

	const float mlat=85.0511f, mlon=180.0f, d2r=M_PI/180.0f;
	char *prefix=argv[1];
	char url[256], filename[256];
	float fl, ft, fr, fb, x, y, n;
	int z, m, l, t, r, b;

	z=atoi(argv[2]);
	m=(1<<z)-1;
	n=float(1<<z);
	sscanf(argv[3], "%f", &fl);
	sscanf(argv[4], "%f", &ft);
	sscanf(argv[5], "%f", &fr);
	sscanf(argv[6], "%f", &fb);
	if(fl < -mlon || fl > mlon){
		puts("invalid left boundary value");
		return -1;
	}
	if(fr < -mlon || fr > mlon){
		puts("invalid right boundary value");
		return -1;
	}
	if(ft < -mlat || ft > mlat){
		puts("invalid left boundary value");
		return -1;
	}
	if(fb < -mlat || fb > mlat){
		puts("invalid right boundary value");
		return -1;
	}
	if(ft < fb){
		puts("invalid lattitude boundaries");
		return -1;
	}

	x=n*(fl+180.0f)/360;
	l=int(floorf(x));
	y=0.5f*n*(1-M_1_PI*logf(tanf(ft*d2r)+1/cosf(ft*d2r)));
	t=int(floorf(y));

	x=n*(fr+180.0f)/360;
	r=int(floorf(x));
	y=0.5f*n*(1-M_1_PI*logf(tanf(fb*d2r)+1/cosf(fb*d2r)));
	b=int(floorf(y));

	WSADATA wsaData;
	if(WSAStartup(0x101, &wsaData) != 0){
        puts("WSAStartup failed");
		return -1;
	}

	int total=(r > l? r-l+1: r-l+n)*(b-t+1), done=0;
	printf("bounding box: [[%d; %d], [%d; %d]]\n", l, t, r, b);
	printf("number of tiles: %d\n", total);
	for(int j=t; j <= b; j=j+1){
		for(int i=l; i <=r; i=i+1&m){
			float cl, ct, cr, cb, crr, cbb;
			x=float(i)+1.0f/512.0f;
			y=float(j)+1.0f/512.0f;
			cl=360.0f*x/n-180;
			ct=atanf(sinhf(M_PI-2*M_PI*y/n))/d2r;
			x=float(i+1&m)-1.0f/512.0f;
			y=float(j+1)-1.0f/512.0f;
			cr=360.0f*x/n-180;
			cb=atanf(sinhf(M_PI-2*M_PI*y/n))/d2r;
			x=float(i+1&m)+1.0f/512.0f;
			y=float(j+1)+1.0f/512.0f;
			crr=360.0f*x/n-180;
			cbb=atanf(sinhf(M_PI-2*M_PI*y/n))/d2r;

			sprintf(url, urlfmt, z, i, j);
			sprintf(filename, "%sy%dx%d.png", prefix, j, i);
			long len=downloadFile(url, filename);
			printf("\r%d/%d\t%s\t%d bytes", ++done, total, filename, len);
			writeMap(filename, cl, ct, cr, cb, crr, cbb);
		}
	}
	puts("\ncomplete\n");
	WSACleanup();
	return 0;
}

