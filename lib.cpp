//#include<graphics.h>
#include<cmath>
#include<cstring>
#include<iostream>
#include <winbgim.h> 


int kc(int x1,int y1,int x2,int y2){
	return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

bool kt_rect(int x1,int y1,int x2,int y2,int x,int y)
{
	if(x1<x&&x<x2&&y1<y&&y<y2) return true; else return false;
}
	
bool kt_circle(int x,int y,int x1,int y1){
	if((x1-x)*(x1-x)+(y1-y)*(y1-y)<=20*20) return true;else return false;
}


void Create_Triangle(int x1, int y1, int x2, int y2, int color){
	setfillstyle(1, color);
	int a[8];
	a[0] = x1 + y2 - y1;
	a[1] = y1 + x1 - x2;
	a[2] = x1 - y2 + y1;
	a[3] = y1 - x1 + x2;
	a[4] = x2; a[5] = y2;
	a[6] = a[0]; a[7] = a[1];
	setcolor(color);
	fillpoly(4,a);//ve da giac dac(mang so nguyen chua toa do cac diem,so cap diem)
}

void Create_Arrow(int x1, int y1, int x2, int y2,int s, int color){
	// BK = Ban kinh node
	int BK = 20;
	float corner = atan(float(abs(y1 - y2)) / abs(x1 - x2));
	float Ry = BK * sin(corner);
	float Rx = BK * cos(corner);
	int x11 = x1 + Rx;
	int y11 = y1 + Ry;
	int x22 = x2 - Rx;
	int y22 = y2 - Ry;
	if (x1 > x2){
		x11 = x1 - Rx;
		x22 = x2 + Rx;
	}
	if (y1 > y2){
		y11 = y1 - Ry;
		y22 = y2 + Ry;
	}
	//setlinestyle(0, 1, 2);
	setlinestyle(0, 0, 1);
	setcolor(color);
	line(x11, y11, x22, y22);
	int ox=(x1+x2)/2,oy=(y1+y2)/2;//d=length_int(s);
	char xau[10];
	itoa(s,xau,10);
	settextstyle(8,0,1);
	setcolor(0);
	outtextxy(ox-10,oy,xau);
	Create_Triangle(2 * x22 - (x2 + x22) / 2, 2 * y22 -(y22 + y2) / 2, x22, y22, color);
	
}
void drawCurvedLine2(int x1, int y1, int x2, int y2, int color, float w)
{
	int BK=20;
	setcolor(color);
	float xO = (x1 + x2) / 2 + (y1 - y2) / M_SQRT2; // or +
	float yO = (y1 + y2) / 2 + (x2 - x1) / M_SQRT2; // or +
	float r = sqrt(pow(xO - x1, 2) + pow(yO - y1, 2));
	float d = sqrt(pow(x1 - xO, 2) + pow(y1 - yO, 2));
	float a = (pow(r, 2) - pow(BK, 2) + pow(d ,2)) / (2 * d);
	float h = sqrt(r * r - a * a);								//					 
	float tmpx1 = xO + a * (x1 - xO) / d;						//					   xT		  
	float tmpx2 = xO + a * (x2 - xO) / d;						//        *  * x3             x4*  *
	float tmpy1 = yO + a * (y1 - yO) / d;						//     *        *            *        *
	float tmpy2 = yO + a * (y2 - yO) / d;						//    *    x1    *          *     x2   *
	int x3 = tmpx1 - h* (y1 - yO) / d;   // -h					//    *          *          *          *
	int x4 = tmpx2 + h* (y2 - yO) / d;   // +h					//     *        *            *        *
	int y3 = tmpy1 + h * (x1 - xO) / d;  // 					//        *  *                  *  *
	int y4 = tmpy2 - h * (x2 - xO) / d;  // 					//				 	   xO          
	float stangle, endangle;									//					 
	float angle1 = float(x1 - xO) / r;
	float angle2 = 1 - float(pow(BK, 2)) / (2 * pow(r, 2));
	if (angle1 < -1 || angle1 > 1) angle1 = int(angle1);
	if (angle2 < -1 || angle2 > 1) angle2 = int(angle2);
	angle1 = acos(angle1) * 180 / M_PI;
	angle2 = acos(angle2) * 180 / M_PI;
	if (y1 >= yO) angle1 = 360 - angle1;
	stangle = angle1 + angle2;
	angle1 = acos(1 - pow(sqrt(pow(x3 - x4, 2) + pow(y3 - y4, 2)), 2) / (2 * pow(r, 2)));
	angle1 = angle1 * 180 / M_PI;
	stangle = stangle - angle1 - 2 * angle2;
	endangle = stangle + angle1;
	float theta = atan2((y1 + y2) / 2 - yO, (x1 + x2) / 2 - xO);
	float xT = xO + r * cos(theta);
	float yT = yO + r * sin(theta);
	setcolor(color);
	setlinestyle(0, 0, 1);
	arc(xO, yO, stangle, endangle, r);
	Create_Triangle(2 * x4 - (x2 + x4) / 2, 2 * y4 - (y2 + y4) / 2, x4, y4, color); // FINALY
	setcolor(0);
	int t=10;//length_int(w);
	char s[t];
	itoa(w,s,10);
	if(w==0)outtextxy(xT, yT,"0");else outtextxy(xT, yT,s);	
}
void Create_CurvedLine(int x1, int y1, int x2, int y2, int w,int color)
{	int BK=20;
	setcolor(color);
	float xO = (x1 + x2) / 2 + (y1 - y2) / -M_SQRT2; 
	float yO = (y1 + y2) / 2 + (x2 - x1) / -M_SQRT2; 
	float r = sqrt(pow(xO - x1, 2) + pow(yO - y1, 2));
	float d = sqrt(pow(x1 - xO, 2) + pow(y1 - yO, 2));
	float a = (pow(r, 2) - pow(BK, 2) + pow(d ,2)) / (2 * d);
	float h = sqrt(r * r - a * a);												 
	float tmpx1 = xO + a * (x1 - xO) / d;					
	float tmpx2 = xO + a * (x2 - xO) / d;				
	float tmpy1 = yO + a * (y1 - yO) / d;			
	float tmpy2 = yO + a * (y2 - yO) / d;					
	int x3 = tmpx1 + h* (y1 - yO) / d;   					
	int x4 = tmpx2 - h* (y2 - yO) / d;   			
	int y3 = tmpy1 - h * (x1 - xO) / d;  				
	int y4 = tmpy2 + h * (x2 - xO) / d;  					          
	float stangle, endangle;												 
	float angle1 = float(x1 - xO) / r;
	float angle2 = 1 - float(pow(BK, 2)) / (2 * pow(r, 2));
	if (angle1 < -1 || angle1 > 1) angle1 = int(angle1);
	if (angle2 < -1 || angle2 > 1) angle2 = int(angle2);
	angle1 = acos(angle1) * 180 / M_PI;
	angle2 = acos(angle2) * 180 / M_PI;
	if (y1 >= yO) angle1 = 360 - angle1;
	stangle = angle1 + angle2;
	angle1 = acos(1 - pow(sqrt(pow(x3 - x4, 2) + pow(y3 - y4, 2)), 2) / (2 * pow(r, 2)));
	angle1 = angle1 * 180 / M_PI;
	endangle = stangle + angle1; 
	float theta = atan2((y1 + y2) / 2 - yO, (x1 + x2) / 2 - xO);
	float xT = xO + r * cos(theta);
	float yT = yO + r * sin(theta);
	if (xT <= 340 || xT >= getmaxx()-40 || yT <= 30 || yT >= 500-20)
		return drawCurvedLine2(x1, y1, x2, y2, color, w);
	//setlinestyle(0, 1, 2);
	setlinestyle(0, 0, 1);
	arc(xO, yO, stangle, endangle, r);
	Create_Triangle(2 * x4 - (x2 + x4) / 2, 2 * y4 - (y2 + y4) / 2, x4, y4, color); 
	setcolor(0);
	int t=10;//length_int(w);
	char s[t];
	itoa(w,s,10);
	if(w==0)outtextxy(xT, yT,"0");else outtextxy(xT, yT,s);	
}

void OutPut(int x,int y,char a){
	if(a=='1') outtextxy(x,y,"1");
	if(a=='2') outtextxy(x,y,"2");
	if(a=='3') outtextxy(x,y,"3");
	if(a=='4') outtextxy(x,y,"4");
	if(a=='5') outtextxy(x,y,"5");
	if(a=='6') outtextxy(x,y,"6");
	if(a=='7') outtextxy(x,y,"7");
	if(a=='8') outtextxy(x,y,"8");
	if(a=='9') outtextxy(x,y,"9");
	if(a=='0') outtextxy(x,y,"0");
}

void nut(int x1,int y1, int x2, int y2,int color,int bgcolor, int thickness)
{ 
	setfillstyle(1,bgcolor);
	setcolor(color);
	setbkcolor(bgcolor); 
	bar(x1,y1,x2,y2);
	for (int i=0; i<=thickness; i++){
		rectangle(x1+i,y1+i,x2+i,y2+i);
	}
}

void tongang(int x1,int y,int x2, int color, int thickness)
{
	setcolor(color);
	for (int i=0; i<=thickness; i++){
		line(x1,y+i,x2,y+i);
		
	}
}
void todoc(int x1,int y1, int y2,int color, int thickness)
{
	setcolor(color);
	for (int i=0; i<=thickness; i++)
	{
		line(x1+i,y1,x1+i,y2);
	}
}

void hcn_day(int x1,int y1, int x2, int y2,int color, int thickness)
{
	settextstyle(0,0,1);
	setcolor(color);
	for (int i=0; i<=thickness; i++)
	{
		rectangle(x1+i,y1+i,x2+i,y2+i);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Nnew(bool test)
{
	if (test == true)
		nut(10,8,100,40,0,10,1);
	else
		nut(10,8,100,40, 1,3,1);
		setcolor(0);
		outtextxy(25,17,"New");
}


void Nopen(bool test)
{
	if ( test == true)
        nut(120,8,220,40,0,10,1);
    else
	    nut(120,8,220,40,1,3,1);
		setcolor(0);
		outtextxy(135,17,"Open");
}

void Nsave(bool test)
{
    if (test == true )
        nut(240,6,350,40,0,10,1);
    else
	    nut(240,6,350,40,1,3,1);
		setcolor(0);
		outtextxy(265,17,"Save");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Add(bool test)
{
    if (test == true )
        nut(400,8,500,40,0,10,1);
    else
        nut(400,8,500,40,1,3,1);
		setcolor(0);
		outtextxy(420,17,"Add");
		setlinestyle(0,1,1);//(line style, color, thickness)
}


void Delete(bool test)
{
    if (test == true )
        nut(520,8,625,40,0,10,1);
    else
    	nut(520,8,625,40, 1,3,1);
		setcolor(0);	
		outtextxy(526,17,"Delete");
		setlinestyle(0,1,1);
}
void Move(bool test)
{
    if (test == true)
        nut(640,8,740,40,0,10,1);
    else
	    nut(640,8,740,40, 1,3,1);
		setcolor(0);
		outtextxy(659,17,"Move");
		setlinestyle(0,1,1);
}


void Connect(bool test)
{
    if (test == true)
   	 nut(760,8,880,40,0,10,1);
    else
		nut(760,8,880,40, 1,3,1);
		setcolor(0);
		outtextxy(765,18,"Connect");
		setlinestyle(0,1,1);
}


void Rename(bool test)
{
    if (test == true)
    	nut(900,8,1015,40,0,10,1);
    else
    	nut(900,8,1015,40, 1,3,1);
		setcolor(0);
		outtextxy(910,17,"Rename");
		setlinestyle(0,1,1);
}


void DFS(bool test)
{ 
    if (test == true)
	    nut(25,475,140,525,0,10,1);
    else
	    nut(25,475,140,525, 1,3,1);
		setcolor(0);
		outtextxy(60,492,"DFS");
}


void BFS(bool test)
{
    if (test == true)
	    nut(170,475,290,525,0,10,1);
    else
	    nut(170,475,290,525, 1,3,1);
		setcolor(0);
		outtextxy(205,492,"BFS");
}

void Top(bool test)
{
    if (test == true)
	    nut(320,475,430,525,0,10,1);
    else
	    nut(320,475,430,525, 1,3,1);
		setcolor(0);
		outtextxy(345,492,"TOP");
}


void Euler(bool test)
{
    if (test == true)
	    nut(25,540,140,590,0,10,1);
    else
	    nut(25,540,140,590, 1,3,1);
		setcolor(0);
		outtextxy(47,557,"Euler");
}


void Ham(bool test)
{
    if (test == true)
	    nut(170,540,290,590,0,10,1);
    else
	    nut(170,540,290,590, 1,3,1);
		setcolor(0);
		outtextxy(205,557,"Ham");
}


void Xy(bool test)
{
    if (test == true)
	    nut(320,540,430,590,0,10,1);
    else
	    nut(320,540,430,590, 1,3,1);
		setcolor(0);
		outtextxy(345,557,"X->Y");
}

void Lienthong(bool test)
{
    if (test == true)
	    nut(25,620,215,690,0,10,1);
    else
	    nut(25,620,215,690, 1,3,1);
		setcolor(0);
		outtextxy(45,645,"Lien Thong");	
}


void Dinhtru(bool test)
{
    if (test == true)
	    nut(240,620,430,690,0,10,1);
    else
	    nut(240,620,430,690, 1,3,1);
		setcolor(0);
		outtextxy(277,645,"Dinh Tru");
}


void Dinhthat(bool test)
{
    if (test == true)
	    nut(25,705,215,775,0,10,1);
    else
	    nut(25,705,215,775, 1,3,1);
		setcolor(0);
		outtextxy(55,730,"Dinh That");
}

void Canhcau(bool test)
{
    if (test == true)
        nut(240,705,430,775,0,10,1);
    else
    	nut(240,705,430,775, 1,3,1);
		setcolor(0);
		outtextxy(270,730,"Canh Cau");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void tao_node(int x, int y, int color)
{
	setlinestyle (0,0,1);
	setcolor(color);
	setfillstyle(1,color);
	for (int i=0; i<=1;i++)
	{
		circle(x,y,21-i);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void toolbar(){
	settextstyle(0,0,2);
	Nnew(false);
	Nopen(false);
	Nsave(false);
	Add(false);
	Delete(false);
	Move(false);
	Connect(false);
	Rename(false);
}

void thuattoan_screen(){
	int color=1,bkcolor=3,thinkness=1;
	hcn_day(11,430,450,785,color,thinkness);//thuat toan
	settextstyle(1,0,2);  
	setcolor(2);
	setbkcolor(15);
	outtextxy(20,436,"Algorithm"); 
	settextstyle(0,0,2); 
	DFS(false);
	BFS(false);
	Top(false);
	Euler(false);
	Ham(false);
	Xy(false);
	Lienthong(false);
	Dinhtru(false);
	Dinhthat(false);
	Canhcau(false);
}

void thuattoan_tool_screen(){
	thuattoan_screen();
	toolbar();
}

void work_screen(){//hinh chu nhat lon nhat
	int color=1,bkcolor=3,thinkness=1;
	setfillstyle(1,15);
	bar(458,60,getmaxx()-10,550);
	setcolor(1);
	for (int i=0;i<=1;i++)
	{
		rectangle(458+i,59+i,getmaxx()-17-i,550-i);
	}
	//hcn_day(458,59,1394,550,color,thinkness);
}

void matrix_screen(){
	setfillstyle(1,15);
	bar(13,61,452,422);
	int color=1,bkcolor=3,thinkness=1;
	hcn_day(11,59,450,420,color,thinkness);
	setcolor(2);
	settextstyle(1,0,2);
	setbkcolor(15);  
	outtextxy(20,65,"Matrix");
} 

void thongbao_screen()
{
	int color=1,bkcolor=3,thinkness=1;
	setfillstyle(1,15); //to phia trong bar mau trong suot
	bar(458,560,1394,785); 
	setcolor(2);
	hcn_day(458,560,1394,785,color,thinkness);
}


void start_screen()
{
	int color=1,bkcolor=3,thinkness=1;
	initwindow(1420,815);
	int x,y;
	x=getmaxx();y=getmaxy();
	setbkcolor(15);
	cleardevice();	 
	thuattoan_screen();
	toolbar();
	matrix_screen();
	work_screen();
	
	setbkcolor(15);
	setcolor(12);
	settextstyle(1,0,2);
	outtextxy(472,575,"Guide:")	; 
	hcn_day(458,560,1394,785,color,thinkness);  
	settextstyle(8, HORIZ_DIR, 2);
	setcolor(0);

	outtextxy(472,620,"(New): Tao do thi moi                         (Open): Mo do thi da co");
	outtextxy(472,650,"(Save): Luu do thi                            (Add):  Them dinh");
	outtextxy(472,680,"(Delete): Xoa dinh                            (Move): Di chuyen dinh");
	outtextxy(472,710,"(Connect): Tao cung va chinh sua              (Rename): Doi ten dinh ");
	outtextxy(472,740,"(ESC): Thoat chuong trinh");
}


