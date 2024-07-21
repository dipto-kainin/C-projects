//day predict by date
#include<stdio.h>
void year(int);
void month(int);
int z,x,p;
void main() {
	int d,m,y,s,a;
	printf("Enter the date eg:[dd mm yyyy] =");
	scanf("%d%d%d",&d,&m,&y);
	year(y);
	month(m);
	s=d+p+z+x+(x/4);
	if(m<=2 && x%4==0)
		s=s-1;
	a=s%7;
	switch(a) {
		case 0:
			printf("the date was/is/will be saturday");
			break;
		case 1:
			printf("the date was/is/will be sunday");
			break;
		case 2:
			printf("the date was/is/will be monday");
			break;
		case 3:
			printf("the date was/is/will be tuesday");
			break;
		case 4:
			printf("the date was/is/will be wednesday");
			break;
		case 5:
			printf("the date was/is/will be thursday");
			break;
		case 6:
			printf("the date was/is/will be friday");
			break;
	}
}
void year(int y) {
	int original;
	original=y;
	int i,g,h,s;
	for (i=0; i<4; i++) {

		if (i==2) {
			h=y%10;
			y=y/10;
		} else if(i==3) {
			g=y%10;
			y=y/10;
		} else
			y=y/10;
	}
	z=(g*10)+h;
	y=original;
	for (i=0; i<4; i++) {

		if (i==0) {
			h=y%10;
			y=y/10;
			continue;
		} else if(i==1) {
			g=y%10;
			y=y/10;
			continue;
		} else
			y=y/10;
	}
	x=(g*10)+h;
	if((z-15)%4==0)
		z=0;
	else if((z-16)%4==0)
		z=6;
	else if((z-17)%4==0)
		z=4;
	else if((z-18)%4==0)
		z=2;
}
void month(int m) {
	switch(m) {
		case 1:
			p=1;
			break;
		case 2:
			p=4;
			break;
		case 3:
			p=4;
			break;
		case 4:
			p=0;
			break;
		case 5:
			p=2;
			break;
		case 6:
			p=5;
			break;
		case 7:
			p=0;
			break;
		case 8:
			p=3;
			break;
		case 9:
			p=6;
			break;
		case 10:
			p=1;
			break;
		case 11:
			p=4;
			break;
		case 12:
			p=4;
			break;
	}
}