#include <iostream>
#include <winbgim.h>
#include <cstring>
#include "stack.h"
#include "queue.h"
#include "lib.h"
//#include<graphics.h>
#include <fstream>
////////////////////////
using namespace std;

// ---------------------------------------------------------------------------
//                          const valriable
// ---------------------------------------------------------------------------
const int MAX_IN = 1000;
const int TIME1 = 1000;
const int TIME = 500;
const int infinite = 9999;
const int MAX_NUT = 15;
// ---------------------------------------------------------------------------
//                          DOTHI dt
// ---------------------------------------------------------------------------
struct Node
{
	int x, y;
	char name[2];
};
struct DoThi{
	Node *node;
	int TongSo_Dinh;
	int **MT;
	/*		------method------		*/
	void delete_Data()
	{
		delete[] node;
		for (int i = 0; i <= TongSo_Dinh; i++)	delete[] MT[i];
		delete[] MT;
	}
	void init_Node(int t, int ind)
	{
		if (t == 1)
		{ // TANG NODE LEN 1
			Node *node2 = new Node[TongSo_Dinh+1];
			for (int i = 0; i <= TongSo_Dinh-1; i++)
				node2[i] = node[i];
			delete[] node;
			node = node2;
		}
		if (t == -1)
		{ //GIAM NODE XUONG 1
			Node *node2 = new Node[TongSo_Dinh+1];
			for (int i = 0; i <= ind-1; i++)	node2[i] = node[i];
			for (int i = ind; i <= TongSo_Dinh; i++)	node2[i] = node[i+1];
			delete[] node;
			node = node2;
		}
		if (t == 0)
		{ // CAP NHAT NODE KHI OPEN FILE
			Node *node2 = new Node[TongSo_Dinh+1];
			delete[] node;
			node = node2;
		}
	}
	void reset_MT(int **MT1)
	{
		for (int i = 0; i <= TongSo_Dinh; i++)
			for (int j = 0; j <= TongSo_Dinh; j++)
				MT1[i][j] = 0;
	}

	void init_MT(int t, int ind)
	{
		if (t == 1)
		{ //TANG MT LEN 1
			int **MT2 = new int *[TongSo_Dinh+1];
			for (int i = 0; i <= TongSo_Dinh; i++)	MT2[i] = new int[TongSo_Dinh+1];
			
			reset_MT(MT2);
			
			for (int i = 0; i <= TongSo_Dinh-1; i++)
				for (int j = 0; j <= TongSo_Dinh-1; j++)
					MT2[i][j] = MT[i][j];
					
			for (int i = 0; i <= TongSo_Dinh-1; i++)	delete[] MT[i];
				
			delete[] MT;
			MT = MT2;
		}
		if (t ==-1)
		{ //GIAM MT DI 1
			int **MT2 = new int *[TongSo_Dinh+1];
			for (int i = 0; i <= TongSo_Dinh; i++)	MT2[i] = new int[TongSo_Dinh+1]; //sao chep cac dinh cua MT sang MT2

			for (int j = 0; j <= TongSo_Dinh+1; j++){//Xoa Dinh ind
				MT[ind][j] = 0;
				MT[j][ind] = 0;
			}
			
			for (int x1 = 0; x1 <=TongSo_Dinh+1; x1++)
				for (int y1 = 0; y1 <=TongSo_Dinh+1; y1++)	if (MT[x1][y1] != 0){//ton tai dinh
						int t1, t2;
						if(x1>ind)	 t1= x1-1; 
						else	t1=x1;
					
						if(y1>ind) t2=y1-1;
						else	 t2=y1;
						
						if (x1>ind||y1>ind){
							MT[t1][t2] = MT[x1][y1];
							MT[x1][y1] = 0;
						}
					}

			reset_MT(MT2);
			for (int i = 0; i <= TongSo_Dinh; i++)
				for (int j = 0; j <= TongSo_Dinh; j++)
					MT2[i][j] = MT[i][j];
					
			for (int i = 0; i <=TongSo_Dinh+1; i++)	delete[] MT[i];			
			delete[] MT;
			MT = MT2;
		}
		if (t == 0)
		{ //KHOI TAO LAI MT KHI OPEN FILE
			for (int i = 0; i <= ind; i++)	delete[] MT[i];
				
			delete[] MT;
			int **MT2 = new int *[TongSo_Dinh + 1];
			for (int i = 0; i <= TongSo_Dinh; i++)	MT2[i] = new int[TongSo_Dinh + 1];
			
			reset_MT(MT2);
			MT = MT2;
		}
	}
	~DoThi()
	{
		delete[] node;
		for (int i = 0; i <= TongSo_Dinh; i++)
			delete[] MT[i];
		delete[] MT;
	}
} dt;





/////////////////////////////////
/////////////// init MT
int** init_tmp(){
	int **tmp= new int*[dt.TongSo_Dinh+1];
	for(int i=0;i<= dt.TongSo_Dinh;i++)
		tmp[i]=new int[dt.TongSo_Dinh+1];
	return tmp;
}
/////copy MT
void Copy_MT(int **tmp){
	for(int i= 0;i<=dt.TongSo_Dinh; i++){
		for (int j=0;j<=dt.TongSo_Dinh; j++){
			dt.MT[i][j]>0?tmp[i][j]=1:tmp[i][j]=0;
		}
	}
}
///////////////Co lap dinh
void Co_Lap_Dinh(int **tmp,int pos){
	//
	Copy_MT(tmp);
	for (int i=0;i<=dt.TongSo_Dinh; i++){
		tmp[pos][i]=0;
		tmp[i][pos]=0;
	}
}
//xoa canh 
void Xoa_Canh(int **tmp,int u,int v){
	Copy_MT(tmp);
	tmp[u][v]=0;
}//////////////////////////////////////////
// ---------------------------------------------------------------------------
//                          Variables
// ---------------------------------------------------------------------------
int x, y, tam, dem = 0, tam1 = -1, tam2 = -1;
bool working = false;
char tenfile[50];
bool kt_Del = false, kt_Add = false, kt_Move = false, kt_Connect = false, kt_Rename = false, kt_New = false, kt_Save = false, kt_Open = false;
bool kt_DFS = false, kt_BFS = false, kt_DThat = false, kt_XY = false, kt_TPLT = false, kt_Ham = false, kt_Dtru = false, kt_Eurle = false, kt_CCau = false;

// ---------------------------------------------------------------------------
//                          Algorithm
// ---------------------------------------------------------------------------
void DFS(int);
void BFS(int);
void Dijkstra(); //xy

// ---------------------------------------------------------------------------
//                          Input/putput
// ---------------------------------------------------------------------------
void openfile();
void save();

// ---------------------------------------------------------------------------
//                          Button
// ---------------------------------------------------------------------------

////////////////////////////////////////////
void set_true(bool &a)
{
	kt_Del = false;
	kt_Add = false;
	kt_Move = false;
	kt_Connect = false;
	kt_Rename = false;
	kt_DFS = false;
	kt_BFS = false;
	kt_DThat = false;
	kt_XY = false;
	kt_TPLT = false;
	kt_Ham = false;
	kt_Dtru = false;
	kt_Eurle = false;
	kt_CCau = false;
	a = true;
}

void set_false()
{
	kt_Del = false;
	kt_Add = false;
	kt_Move = false;
	kt_Connect = false;
	kt_Rename = false;
	kt_DFS = false;
	kt_BFS = false;
	kt_DThat = false;
	kt_XY = false;
	kt_TPLT = false;
	kt_Ham = false;
	kt_Dtru = false;
	kt_Eurle = false;
}

void clearMouse()
{
	clearmouseclick(WM_LBUTTONDOWN);
}

bool kiemtra_word(char s)
{
	if (int(s) == 0)
	{
		s = getch(); //lay ki tu ban phim
		return true; //khong nhan so 0
	}
	if (!(('0' <= s and s <= '9') or ('a' <= s and s <= 'z') or ('A' <= s and s <= 'Z') or s == '_' or s == 13 or s == 8 or s == 27))
		return true; //s!= phim _, backspace, enter
	return false;
}

void Link_File(char linkFile[], char fileName[])
{
	strcpy(linkFile, "./saves/"); //sao ch�p chuoi k� tu tu . / s a v e s / toi chuoi linkFile.
	strcat(linkFile, fileName);	  //noi chuoi fileName phia sau linkFile
	strcat(linkFile, ".graph");	  //noi chuoi .graph phia sau linkFile
}

void updateChar(int i, char s[])
{
	for (; i < 50; i++)
		s[i] = '\0';
}

void reset_tenfile(char tenfile[])
{
	for (int i = 0; i <= 50; i++)
		tenfile[i] = '\0';
}

void Del_Node(int i)
{
	setcolor(15);
	setfillstyle(1, 15);
	pieslice(dt.node[i].x  , dt.node[i].y  , 0, 360, 21);
}
void draw_Node(int i, int color)
{
	setcolor(color);
	setfillstyle(1, color);
	pieslice(dt.node[i].x  , dt.node[i].y  , 0, 360, 21);
	setbkcolor(color);
	settextstyle(8, HORIZ_DIR, 1);

	setcolor(4);
	outtextxy(dt.node[i].x - 14, dt.node[i].y - 10, dt.node[i].name);/////////////////////////////
	setbkcolor(15);

	setcolor(0);
	circle(dt.node[i].x  , dt.node[i].y  , 21);
}

void load_node()
{
	work_screen();
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		tao_node(dt.node[i].x  , dt.node[i].y  , 1);
		settextstyle(8, HORIZ_DIR, 2);
		setbkcolor(15);
		outtextxy(dt.node[i].x - 14, dt.node[i].y - 10, dt.node[i].name);///////////////////////
	}
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
		{
			if (dt.MT[i][j] != 0 and dt.MT[j][i] == 0)
				Create_Arrow(dt.node[i].x  , dt.node[i].y  , dt.node[j].x  , dt.node[j].y  , dt.MT[i][j], 4);
			if (dt.MT[i][j] != 0 and dt.MT[j][i] != 0 and i <= j)
				Create_Arrow(dt.node[i].x  , dt.node[i].y  , dt.node[j].x  , dt.node[j].y  , dt.MT[i][j], 4);
			if (dt.MT[i][j] != 0 and dt.MT[j][i] != 0 and i > j)
				Create_CurvedLine(dt.node[i].x  , dt.node[i].y  , dt.node[j].x  , dt.node[j].y  , dt.MT[i][j], 4);
		}
}

void Show_Link(int i, int j)
{ //i->j
	if (dt.MT[i][j] != 0 and dt.MT[j][i] == 0)
		Create_Arrow(dt.node[i].x  , dt.node[i].y  , dt.node[j].x  , dt.node[j].y  , dt.MT[i][j], 5);
	else if (dt.MT[i][j] != 0 and dt.MT[j][i] != 0 and i <= j)
		Create_Arrow(dt.node[i].x  , dt.node[i].y  , dt.node[j].x  , dt.node[j].y  , dt.MT[i][j], 5);
	else if (dt.MT[i][j] != 0 and dt.MT[j][i] != 0 and i > j)
		Create_CurvedLine(dt.node[i].x  , dt.node[i].y  , dt.node[j].x  , dt.node[j].y  , dt.MT[i][j], 5);
}

void load_matrix()
{
	matrix_screen();
	if (dt.TongSo_Dinh >= 0)
	{
		setcolor(4);
		for (int i = 0; i <= dt.TongSo_Dinh; i++)
		{
			for (int j = 0; j <= dt.TongSo_Dinh; j++)
				if (dt.MT[i][j] != 0)
				{
					char a[14];
					itoa(dt.MT[i][j], a, 14);
					settextstyle(0, 0, 1);
					outtextxy(93 + 20 * j + 1, 105 + 20 * i + 3, a);
				}
		}

		for (int i = 0; i <= dt.TongSo_Dinh; i++)
		{ //stt dinh theo chieu ngang
			setcolor(4);
			settextstyle(0, 0, 1);
			outtextxy(87 + 20 * i + 3, 90, dt.node[i].name);
		}
		for (int i = 0; i <= dt.TongSo_Dinh; i++)
		{ //stt dinh theo chieu doc
			setcolor(4);
			outtextxy(70, 105 + 20 * i + 3, dt.node[i].name);
		}

		for (int i = 0; i <= dt.TongSo_Dinh + 2; i++)
		{
			setcolor(2);
			line(68 + 20 * i, 83, 68 + 20 * i, 83 + 20 * (dt.TongSo_Dinh + 2));
		}
		for (int i = 0; i <= dt.TongSo_Dinh + 2; i++)
		{
			setcolor(2);
			line(68, 83 + 20 * i, 68 + 20 * (dt.TongSo_Dinh + 2), 83 + 20 * i);
		}
	}
}

void Open_DoThi(char fileName[])
{
	thongbao_screen();
	char linkFile[100];
	Link_File(linkFile, fileName);
	ifstream openFile;
	openFile.open(linkFile, ios::in);
	if (!openFile.good())
	{
		setcolor(4);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(468, 620, "File khong ton tai");
		delay(TIME1);
		kt_Save = false;
		thuattoan_tool_screen();
		return;
	}
	////////////
	int t = dt.TongSo_Dinh;
	//////////
	openFile >> dt.TongSo_Dinh;
	dt.init_Node(0, 0);
	dt.init_MT(0, t);
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
			dt.MT[i][j] = 0;
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		openFile >> dt.node[i].name >> dt.node[i].x >> dt.node[i].y;
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
		{
			openFile >> dt.MT[i][j];
		}
	}
	openFile.close();
	load_node();
}

void openfile()
{
	thongbao_screen();
	settextstyle(8, HORIZ_DIR, 2);
	for (int i = 0; i <= 50; i++)
		tenfile[i] = '\0';
	setcolor(0);
	outtextxy(468, 570, "Nhap ten file (Do dai ten file <=30):  ");
	char s[50]; //s[50]='\0';
	for (int i = 0; i <= 50; i++)
		s[i] = '\0';
	int d = 0;
	while (true)
	{
		setcolor(4);
		s[d] = getch();
		if (kiemtra_word(s[d]))
			continue;
		if (s[d] == 27)
		{
			setcolor(4);
			outtextxy(468, 610, "Thoat mo file !!");
			delay(TIME1);
			break;
		}
		if (d == 30)
		{
			if (s[d] != 13 and s[d] != 8)
			{
				s[29] = s[30]; //neu chuoi dai hon 30 thi gan ki tu s[30] vua nhap vao cho vi tri s[29] cho phep cuoi cung
				d--;
				goto tuongne;
			}
		}
		if (s[d] == 13)
		{
			for (int i = 0; i <= 50; i++)
				tenfile[i] = '\0';
			for (int i = 0; i <= d - 1; i++)
				tenfile[i] = s[i];
			work_screen();
			break;
		}
		else if (s[d] == 8)
		{
			d--;
			thongbao_screen();
			settextstyle(8, HORIZ_DIR, 2);
			setcolor(0);
			outtextxy(468, 570, "Nhap ten file (Do dai ten file <=30):  ");
			updateChar(d, s);
			setcolor(2);
			outtextxy(1000, 570, s);
			if (d == -1)
				d = 0;
			continue;
		}

		else
		{
		tuongne:
			updateChar(d + 1, s);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(1000, 570, s); //hien thi ky tu theo thoi gian thuc
		}
		d++;
	}
	Open_DoThi(tenfile);
}

void Save_DoThi(char fileName[])
{
	char linkFile[100];
	Link_File(linkFile, fileName);
	ofstream saveFile;
	saveFile.open(linkFile, ios::out | ios::trunc);
	saveFile << dt.TongSo_Dinh << "\n";
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		saveFile << dt.node[i].name << " " << dt.node[i].x << " " << dt.node[i].y;
		saveFile << "\n";
	}
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
		{
			saveFile << dt.MT[i][j] << " ";
		}
		saveFile << "\n";
	}
	saveFile.close();
}

void Save()
{
	thongbao_screen();
	setcolor(0);
	settextstyle(8, HORIZ_DIR, 2);
	outtextxy(468, 570, "Nhap ten file (Do dai ten file <=30): ");
	setcolor(0);
	settextstyle(8, HORIZ_DIR, 2);
	fflush(stdin); //xoa bo dem, vd bo ky tu ENTER
	char s[50];
	s[50] = '\0';
	reset_tenfile(tenfile);
	int d = 0;
	while (true)
	{
		s[d] = getch();
		if (kiemtra_word(s[d]))
			continue;
		if (d == 30) //???
		{
			if (s[d] != 13 && s[d] != 8)
			{
				s[29] = s[30]; //neu chuoi dai hon 30 thi gan ki tu s[30] vua nhap vao cho vi tri s[29] cho phep cuoi cung
				d--;
				goto inrathoigianthuc;
			}
		}
		if (s[d] == 13)
		{
			if (d == 0)
				goto nhaplai; //neu chuoi rong =>enter thi nhaplai

			for (int i = 0; i <= 50; i++)
				tenfile[i] = '\0';
			for (int i = 0; i <= d - 1; i++)
			{
				tenfile[i] = s[i];
				tenfile[d] = '\0';
			}
			thongbao_screen();
			setcolor(4);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468, 570, "Da luu file");
			delay(TIME1);
			thongbao_screen();
			break;
		}
		else if (s[d] == 8)
		{
		nhaplai:
			d--;
			thongbao_screen();
			setcolor(0);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468, 570, "Nhap ten file (Do dai ten file <=30): ");
			setcolor(1);
			settextstyle(8, HORIZ_DIR, 2);
			updateChar(d, s);
			outtextxy(1000, 570, s);
			if (d == -1)
				d = 0;
			continue;
		}
		else
		{
		inrathoigianthuc:
			setcolor(1);
			updateChar(d + 1, s);
			outtextxy(1000, 570, s);
		}
		d++;
	}
	kt_Save = true;
	Save_DoThi(tenfile);
}

void Save2()
{
	Save_DoThi(tenfile);
	thongbao_screen();
	setcolor(4);
	settextstyle(8, HORIZ_DIR, 2);
	outtextxy(468, 570, "Da luu file");
	delay(TIME1);
	thongbao_screen();
}

void start()
{
	dt.TongSo_Dinh = -1;
	thongbao_screen(); //mh_noidung
	thuattoan_screen();
	matrix_screen();
	work_screen();
	kt_Save = false;
	load_node();
	load_matrix();
}

////////////////////////////////////////////////THUAT TOAN/////////////////////////////////////////////

void DFS(int root) //O(max(n, m)) n so dinh, m so canh
{
	//if(dt.TongSo_Dinh==-1) return ;
	Queue ds_ke[dt.TongSo_Dinh + 1]; //tao danh sach ke cho moi dinh
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
			if (dt.MT[i][j] > 0)
				ds_ke[i].push(j);
	/////////////////
	bool visited[dt.TongSo_Dinh + 1];
	Stack stack; //LIFO: Last In First Out
	//khoi tao gia tri cho check
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		visited[i] = false;
	}
	stack.push(root);
	visited[root] = true;
	int dem = 1;
	thongbao_screen();
	settextstyle(0, 0, 2);
	setcolor(2);
	outtextxy(468, 570, "DFS:");
	settextstyle(8, HORIZ_DIR, 2);
	setcolor(4);
	outtextxy(468, 610, dt.node[root].name);
	Del_Node(root);
	draw_Node(root, 10);
	delay(TIME);
	// duyet dfs theo phuong phap bieu dien danh sach ke
	while (!stack.empty()) //while 1
	{
		int root_tmp; //lan 1: root_tmp = root.,lan 2: root_tmp = i.,
		stack.pop(root_tmp);
		while (!ds_ke[root_tmp].empty()) //while 2
		{
			int i;
			ds_ke[root_tmp].pop(i); //xoa dinh ke i dang xet ra khoi hang doi ds_ke[root_tmp]
			if (visited[i] == false)
			{
				stack.push(root_tmp);
				stack.push(i); //stack = [i,root]
				visited[i] = true;
				Show_Link(root_tmp, i);
				delay(TIME);
				Del_Node(i);
				setcolor(2);
				settextstyle(8, HORIZ_DIR, 1);
				outtextxy(468 + 30 * dem, 610, "->");
				dem++;
				setcolor(4);
				settextstyle(8, HORIZ_DIR, 2);
				outtextxy(468 + 30 * dem, 610, dt.node[i].name);
				draw_Node(i, 10);
				delay(TIME);
				dem++;
				break; //ket thuc vong lap while 2
			}
		}
	}
	settextstyle(8, HORIZ_DIR, 2);
	setcolor(2);
	outtextxy(468, 640, "DA DUYET XONG !");
}

void BFS(int root)
{
	Queue ds_ke[dt.TongSo_Dinh + 1]; //tao danh sach ke cho moi dinh ,. FIFO: First In First Out
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
			if (dt.MT[i][j] > 0)
				ds_ke[i].push(j);

	Queue queue;
	int *back = new int[MAX_NUT];
	bool visited[dt.TongSo_Dinh + 1];
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		visited[i] = false;
	}
	queue.push(root); //queue = [root]
	visited[root] = true;
	int dem = 0;
	thongbao_screen();
	settextstyle(0, 0, 2);
	setcolor(2);
	outtextxy(468, 570, "BFS :");
	settextstyle(8, HORIZ_DIR, 2);
	setcolor(4);
	outtextxy(468, 600, dt.node[root].name);
	Del_Node(root);
	draw_Node(root, 10);
	// duyet bfs theo phuong phap bieu dien danh sach ke
	while (!queue.empty())
	{
		int root_tmp;
		queue.pop(root_tmp); //queue = [x root_tmp]; =>queue=NULL;
		settextstyle(8, HORIZ_DIR, 2);
		if (root_tmp == root)
		{
			Del_Node(root);
			draw_Node(root, 14);
			delay(TIME);
		}

		else //queue = [i2,i1,i0]; root_tmp= i0;
		{
			Show_Link(back[root_tmp], root_tmp);
			delay(TIME);
			Del_Node(root_tmp);
			draw_Node(root_tmp, 14);
			setcolor(2);
			settextstyle(8, HORIZ_DIR, 1);
			outtextxy(500 + 30 * dem, 600, "->");
			dem++;
			setcolor(4);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(500 + 30 * dem, 600, dt.node[root_tmp].name);
			delay(TIME);
			dem++;
		}
		while (!ds_ke[root_tmp].empty())
		{
			int i;					//cac dinh ke cua root_tmp
			ds_ke[root_tmp].pop(i); //xoa dinh ke i dang xet ra khoi hang doi ds_ke[root_tmp]
			if (visited[i] == false)
			{
				queue.push(i); //lan1: queue = [i0].,lan2: queue = [i1,i0].,lan3: queue = [i2,i1,i0]...
				visited[i] = true;
				back[i] = root_tmp;
			}
		}
	}
	settextstyle(8, HORIZ_DIR, 2);
	setcolor(2);
	outtextxy(468, 640, "DA DUYET XONG !");
}

void Dijkstra(int start, int finish)
{
	Queue ds_ke[dt.TongSo_Dinh + 1]; //tao danh sach ke cho tung dinh ,. FIFO: First In First Out
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
			if (dt.MT[i][j] > 0)
				ds_ke[i].push(j);

	int TrongSo[dt.TongSo_Dinh + 1][dt.TongSo_Dinh + 1];
	int distance[dt.TongSo_Dinh + 1];
	int back[dt.TongSo_Dinh + 1]; //diem truoc CONNECT voi diem dang xet

	bool visited[dt.TongSo_Dinh + 1];

	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		visited[i] = false;

	visited[start] = true;
	int dem = 1; // so luong nut i visited[i]=true
	distance[start] = 0;
	//khoi tao TrongSo dua vao ma tran trong so
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
		{
			if ((dt.MT[i][j] > 0)) //neu 2 dinh i,j ke nhau
			{
				TrongSo[i][j] = dt.MT[i][j];
			}
			else
			{
				TrongSo[i][j] = infinite; //9999
			}
		}
	}
	//khoi tao distance, back
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		if (visited[i] == false)
		{
			distance[i] = TrongSo[start][i];
			back[i] = start;
		}
	}
	//////run
	while (dem <= dt.TongSo_Dinh + 1)
	{
		//tim NUT co TRONG SO den nut start NHO NHAT so voi cac nut con lai
		int min_nut = 0, min_trongso = infinite;

		for (int i = 0; i <= dt.TongSo_Dinh; i++)
		{
			if (visited[i] == false)
			{
				if (distance[i] < min_trongso)
				{
					min_trongso = distance[i];
					min_nut = i;
				}
			}
		}

		if (min_nut == finish)
		{
			break; //thoat khoi vong lap while ngay lap tuc
		}

		visited[min_nut] = true;
		dem++;

		while (!ds_ke[min_nut].empty())
		{
			int i;				   //cac dinh ke cua min_nut
			ds_ke[min_nut].pop(i); //xoa dinh ke i dang xet ra khoi hang doi ds_ke[min_nut]

			if (visited[i] == false)
			{
				if (distance[min_nut] + TrongSo[min_nut][i] < distance[i])
				{
					distance[i] = distance[min_nut] + TrongSo[min_nut][i]; //cap nhat lai distance[i]
					back[i] = min_nut;									   //cap nhat lai back[i]
				}
			}
		}
	}

	if (distance[finish] < infinite)
	{ //sau khi break;
		thongbao_screen();
		settextstyle(8, HORIZ_DIR, 2);
		setcolor(4);
		outtextxy(468, 630, "Tong trong so :");
		settextstyle(8, HORIZ_DIR, 2);
		char ss[10];
		itoa(distance[finish], ss, 10); //char *itoa ( int value, char * str, int co so );
		setcolor(2);
		outtextxy(668, 630, ss);
	}
	else
	{
		thongbao_screen();
		settextstyle(8, HORIZ_DIR, 2);
		setcolor(4);
		setbkcolor(15);
		outtextxy(468, 570, "Khong co duong di tu ");
		outtextxy(775, 570, "->");
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(748, 570, dt.node[start].name);
		outtextxy(800, 570, dt.node[finish].name);
		return;
	}
	Stack st;
	int f = finish;
	st.push(finish);
	while (f != start)
	{
		f = back[f];
		st.push(f);
	}
	int previous;
	int d = 0; // dem,
	while (!st.empty())
	{
		settextstyle(0, 0, 2);
		setcolor(2);
		outtextxy(468, 570, "DIJKSTRA :");

		int tmp;
		st.pop(tmp);
		/////
		if (d == 0)
		{
			previous = tmp;
			setcolor(4);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468 + 50 * d, 600, dt.node[tmp].name);
			Del_Node(tmp);
			draw_Node(tmp, 14);
		}
		else
		{
			Show_Link(previous, tmp);

			delay(TIME);

			Del_Node(tmp);
			if (st.empty()) //dinh cuoi
				draw_Node(tmp, 7);
			else //chua phai dinh cuoi
				draw_Node(tmp, 10);
			setcolor(4);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468 + 50 * d, 600, dt.node[tmp].name);
			setcolor(2);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468 + 50 * d - 25, 600, "->");
			previous = tmp;
			delay(TIME);
		}
		d++;
	}
	settextstyle(8, HORIZ_DIR, 2);
	setcolor(2);
	outtextxy(468, 660, "DA DUYET XONG !");
}

///////////////////////////////////////////hamilton///////////////////////////////////
bool *visited;
int *chu_trinh;
bool kt_ham;
void run()
{
	kt_ham = true;
	thongbao_screen();
	load_node();
	settextstyle(8, HORIZ_DIR, 2);
	setcolor(0);
	outtextxy(468, 570, "Chu trinh hamilton:");
	settextstyle(8, HORIZ_DIR, 2);
	setcolor(4);
	outtextxy(700, 570, dt.node[chu_trinh[0]].name); //nut start
	Del_Node(chu_trinh[0]);
	draw_Node(chu_trinh[0], 10);
	delay(TIME);
	//
	for (int i = 1; i <= dt.TongSo_Dinh + 1; i++)
	{
		setcolor(2);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(700 + i * 50 - 25, 570, "->");
		setcolor(4);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(700 + i * 50, 570, dt.node[chu_trinh[i]].name);

		int back = chu_trinh[i - 1], tam = chu_trinh[i];
		Show_Link(back, tam);
		delay(TIME / 2);
		if (i != dt.TongSo_Dinh + 1)
		{
			Del_Node(chu_trinh[i]);
			draw_Node(chu_trinh[i], 14);
			delay(TIME);
		}
	}
}
void xuly(int i)
{
	for (int j = 0; j <= dt.TongSo_Dinh; j++)
	{
		if (dt.MT[chu_trinh[i - 1]][j] > 0 && !kt_ham)
		{
			if (i == dt.TongSo_Dinh + 1 && j == chu_trinh[0])
			{
				chu_trinh[i] = j;
				run();
				delay(TIME);
			}
			else if (visited[j] == false)
			{
				chu_trinh[i] = j;
				visited[j] = true;
				xuly(i + 1);
				visited[j] = false;
			}
		}
	}
}
void Hamilton(int start)
{
	visited = new bool[dt.TongSo_Dinh + 1]; //
	chu_trinh = new int[dt.TongSo_Dinh + 2];
	kt_ham = false;
	for (int j = 0; j <= dt.TongSo_Dinh; j++)
		visited[j] = false;

	chu_trinh[0] = start;
	visited[start] = true;
	xuly(1);
	if (!kt_ham)
	{
		thongbao_screen();
		settextstyle(8, HORIZ_DIR, 2);
		setcolor(4);
		setbkcolor(15);
		outtextxy(20, 570, "Khong co chu trinh Hamilton");
	}
	delete[] visited;
	delete[] chu_trinh;
}

////////////////////////////////////////////////THUAT TOAN/////////////////////////////////////////////

int *TPLT_Num, *TPLT_Low, cnt, TPLT_Count;
bool *TPLT_Connect;
Queue TPLT_a[MAX_NUT + 2];
Stack st;
void TPLT_dfs(int u)
{
	TPLT_Low[u] = TPLT_Num[u] = cnt++; //gan thoi gian, dinh danh
	st.push(u);						   //
	TPLT_Connect[u] = true;
	while (!TPLT_a[u].empty())
	{
		int v;
		TPLT_a[u].pop(v);
		if (TPLT_Num[v] == -1)
			TPLT_dfs(v);	 // duyet dfs
		if (TPLT_Connect[v]) // neu v con trong ngan xep -> cap nhap lai dinh danh TPLT_Low[u]
			TPLT_Low[u] = min(TPLT_Low[u], TPLT_Low[v]);
	}
	if (TPLT_Num[u] == TPLT_Low[u])
	{ // u la goc cua mot thanh phan lien thong manh chua u
		TPLT_Count++;
		// lay cac dinh la thanh phan lien thong manh trong ngan xep ra
		while (!st.empty())
		{
			int v;
			st.pop(v);
			TPLT_Connect[v] = false;
			if (u == v)
				break;
		}
	}
}
int TPLT(int **tmp, bool f = true)
{
	////kouuu
	cnt = 0; // thoi gian
	TPLT_Count = 0;
	TPLT_Num = new int[MAX_NUT + 2];	  // TPLT_Num[u] danh dau thoi gian diem ma u duoc tham -> Dinh Danh
	TPLT_Low = new int[MAX_NUT + 2];	  // TPLT_Low[u] dinh danh cua w bat ki. co duong di tu u -> w, w thuoc TPLT
	TPLT_Connect = new bool[MAX_NUT + 2]; //TPLT_Connect[u] kiem tra dinh u con trong do thi hay khong
	bool check[MAX_NUT + 2];
	// tao danh sach ke
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
		{
			if (tmp[i][j] > 0)
			{
				TPLT_a[i].push(j);
			}
		}
	}
	// reset gia tri
	for (int i = 0; i <= MAX_NUT + 1; i++)
	{
		TPLT_Num[i] = -1;
		TPLT_Low[i] = 0;
		TPLT_Connect[i] = false;
		check[i] = true;
	}
	///// run
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		if (TPLT_Num[i] == -1)
			TPLT_dfs(i);
	}
	/////// In ra man hinh
	int d = -1;
	if (f)
	{
		int Color[] = {3, 7, 9, 10, 11, 14, 13, 1, 2, 5, 6, 8, 12, 0};
		int sizeColor = sizeof(Color) / sizeof(Color[0]);
		int Color_name = 0;
		for (int i = 0; i <= TPLT_Count - 1; i++)
		{
			settextstyle(8, HORIZ_DIR, 2);
			setcolor(0);
			d++;
			outtextxy(478 + (30 * d) - 15, 590, "(");
			d--;
			for (int j = 0; j <= dt.TongSo_Dinh; j++)
			{
				if (check[j])
				{
					for (int k = j; k <= dt.TongSo_Dinh; k++)
					{
						if (TPLT_Low[k] == TPLT_Low[j])
						{
							//////
							check[k] = false;
							if (TPLT_Count <= sizeColor && Color_name < sizeColor)
							{
								Del_Node(k);
								draw_Node(k, Color[Color_name]);
							}
							setcolor(2);
							d++;
							outtextxy(478 + (30 * d), 590, dt.node[k].name);
							/////
						}
					}
					break;
				}
			}
			Color_name++;
			settextstyle(8, HORIZ_DIR, 2);
			setcolor(0);
			d++;
			outtextxy(478 + 30 * d, 590, ")");
			//d++;
		}
		thongbao_screen();
		settextstyle(8, HORIZ_DIR, 2);
		setcolor(0);
		outtextxy(468, 570, "So thanh phan lien thong la : ");
		setcolor(2);
		char xau[10];
		itoa(TPLT_Count, xau, 10);
		outtextxy(810, 570, xau);
	}
	/////// clear
	delete[] TPLT_Num;
	delete[] TPLT_Low;
	delete[] TPLT_Connect;
	////
	return TPLT_Count;
}
//////////////
bool check_duongdi_LT(int u, int v, int **tmp, bool f = true)
{ // f=true => check duong di; f=false => check Lien Thong do thi vo huong
	//bfs
	Queue queue;
	bool check[dt.TongSo_Dinh + 1];
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		check[i] = false;
	}
	queue.push(u);
	check[u] = true;
	while (!queue.empty())
	{
		int s;
		queue.pop(s);
		////////
		for (int i = 0; i <= dt.TongSo_Dinh; i++)
		{
			if (!check[i] && tmp[s][i] > 0)
			{
				queue.push(i);
				check[i] = true;
				if (v == i && f)
					return true;
			}
		}
	}
	if (!f)
	{
		for (int i = 0; i <= dt.TongSo_Dinh; i++)
			if (!check[i])
				return false;
		return true;
	}
	return false;
}


///////////////////////////////////////////
void Dinh_That(int u,int v){/// u dinh dau, v la dinh cuoi
///init
	int **tmp=init_tmp();
	int count=0;
	int t=0;
//////	
	Copy_MT(tmp);//copy MT
	if (!check_duongdi_LT(u,v,tmp)){
		//
		////////////delete 

		
		thongbao_screen();
		settextstyle(8, HORIZ_DIR, 2);//loixxxx
		outtextxy(20,570,"Khong co dinh that giua 2 dinh "); outtextxy(390,570,"->");
		settextstyle(8, HORIZ_DIR, 1);//loixxx
		outtextxy(360,570,dt.node[u].name);		outtextxy(420,570,dt.node[v].name);
		
		
		for(int i=0;i<=dt.TongSo_Dinh; i++)
			delete[] tmp[i];
		delete[] tmp;
		return;
	}
	for(int i=0;i<=dt.TongSo_Dinh; i++){
		if (i!=u&&i!=v){
			Co_Lap_Dinh(tmp,i);
			if (!check_duongdi_LT(u,v,tmp)){
				count++;
				/// draw node
				Del_Node(i);
				draw_Node(i,14);////////////////////
				settextstyle(8, HORIZ_DIR, 1);
				outtextxy(20+50*t,590,dt.node[i].name);
				t++;
			}
		}
			
	}
	if (count == 0 ){
		thongbao_screen();
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(20,570,"Khong co dinh that giua 2 dinh "); outtextxy(390,570,"->");
		settextstyle(8, HORIZ_DIR, 1);
		outtextxy(360,570,dt.node[u].name);		outtextxy(420,570,dt.node[v].name);
	}
	////////////delete
	for(int i=0; i<=dt.TongSo_Dinh; i++)	delete[] tmp[i];
	
	delete[] tmp;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dinh_tru(){
	//init
	int **tmp=init_tmp();
	Copy_MT(tmp);
	int Min = TPLT(tmp,false),count=0;
	thongbao_screen();		
	settextstyle(8, HORIZ_DIR, 2);
	for(int i=0; i<=dt.TongSo_Dinh; i++){
		Co_Lap_Dinh(tmp,i);
		if(TPLT(tmp,false)-1>Min){
			count++;
			Del_Node(i);
			draw_Node(i,14);
			settextstyle(8, HORIZ_DIR, 2);
			setcolor(4);	outtextxy(650+count*50,570,dt.node[i].name);
		}					
	}					

	if(count==0){
		thongbao_screen();
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(468,570,"Khong co dinh tru");
	}
	else{
		setcolor(0);	outtextxy(468,570,"Co ");
		settextstyle(8, HORIZ_DIR, 2);
		char xau[10];   itoa(count,xau,10);
		setcolor(12);
		outtextxy(500,570,xau);
		setcolor(0);    outtextxy(520,570,"dinh tru :");///////////////////////
	}
	for(int i=0; i<=dt.TongSo_Dinh; i++)	delete[] tmp[i];
	delete[] tmp;
}
////////////////////////////////////////////////////////////////////////////////////
void Canh_Cau(){
	//init
	int **tmp=init_tmp();
	Copy_MT(tmp);
	int Min = TPLT(tmp,false),count=0;
	thongbao_screen();			
	settextstyle(8, HORIZ_DIR, 2);
	for(int i=0; i<=dt.TongSo_Dinh; i++)
		for(int j=0; j<=dt.TongSo_Dinh; j++){
		Xoa_Canh(tmp,i,j);
		if(TPLT(tmp,false)>Min){
			count++;
			Show_Link(i,j);
		}					
	}					

	if(count==0){
		thongbao_screen();
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(468,570,"Khong co canh cau");
	}
	else{
		setcolor(0);	outtextxy(468,570,"Co ");
		settextstyle(8, HORIZ_DIR, 2);
		char xau[10];   itoa(count,xau,10);
		setcolor(12);
		outtextxy(500,570,xau);
		setcolor(0);    outtextxy(520,570,"canh cau :");///////////////////////
	}
	for(int i=0; i<=dt.TongSo_Dinh; i++)	delete[] tmp[i];
	delete[] tmp;
}


int main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	setlinestyle(0, 0, 1);
	working = false;
	start_screen();
	dt.TongSo_Dinh = -1;
	for (int i = 0; i <= 50; i++)
		tenfile[i] = '\0';
	while (1)
	{
		int x = mousex();
		int y = mousey();
		if (ismouseclick(WM_LBUTTONDOWN))
		{
			getmouseclick(WM_LBUTTONDOWN, x, y);
			if (kt_rect(10, 8, 100, 40, x, y)) //new
			{
				thuattoan_tool_screen();
				Nnew(true);
				set_false();
				start();
				working = true; ///////////////1
				kt_Save = false;
			}

			if (kt_rect(120, 8, 220, 40, x, y)) //open
			{
				thuattoan_tool_screen();
				Nopen(true);
				set_false();
				start();
				kt_Save = true; ////////////////////
				working = true; ///////////////2
				settextstyle(0, 0, 2);
				setbkcolor(15);
				openfile();
				load_matrix();
				load_node();
				thongbao_screen();
				thuattoan_tool_screen();
				Nopen(true);
			} //open

			if (kt_rect(240, 6, 350, 40, x, y)) //save
			{
				thuattoan_tool_screen();
				settextstyle(0, 0, 2);
				Nsave(true);
				load_node();
				set_false();
				if (working == false || dt.TongSo_Dinh == -1)
				{ //chi SAVE duoc khi da NEW do thi hoac OPEN do thi
					thongbao_screen();
					settextstyle(8, HORIZ_DIR, 2);
					setbkcolor(15);
					setcolor(4);
					outtextxy(468, 570, "Chua co do thi");
					delay(TIME1);
					thongbao_screen();
				}
				else
				{
					if (kt_Save == false)
						Save();
					else
						Save2(); //khi da OPEN do thi => kt_Save2 =true
				}
				thuattoan_tool_screen();
			}

			if (kt_rect(320, 475, 430, 525, x, y)) //topo
			{
				thuattoan_tool_screen();
				work_screen();
				thongbao_screen();
				settextstyle(0, 0, 2);
				Top(true);
			}

			if (kt_rect(25, 540, 140, 590, x, y)) //euler
			{
				thuattoan_tool_screen();
				work_screen();
				thongbao_screen();
				settextstyle(0, 0, 2);
				Euler(true);
			}

////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (kt_rect(25, 705, 215, 775, x, y)) //dinhthat
			{
				thuattoan_tool_screen();
				thongbao_screen();
				load_node();
				settextstyle(0, 0, 2);
				Dinhthat(true);
				if (working == false)
				{
					thongbao_screen();
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					setcolor(0);
					outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc");
					continue;
				}
				if (dt.TongSo_Dinh < 1)
				{
					thongbao_screen();
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					setcolor(0);
					outtextxy(468, 570, "Do thi phai co it nhat 2 dinh");
					set_false();
					continue;
				}

				dem = 0;
				thongbao_screen();
				setbkcolor(15);
				settextstyle(8, HORIZ_DIR, 2);
				setcolor(0);
				outtextxy(468, 570, "Click vao dinh bat ki de chon dinh bat dau");
				set_true(kt_DThat);				
			}
			if (kt_DThat && kt_rect(460, 60, getmaxx() - 10, 560, x, y))
				{
					if (dem == 0)
					{
						for (int i = 0; i <= dt.TongSo_Dinh; i++)
							if (kc(x, y, dt.node[i].x  , dt.node[i].y  ) <= 20)
							{
								tam = i;
								dem = 1;
							}
						if (dem == 1)
						{
							tao_node(dt.node[tam].x , dt.node[tam].y , 2);///
							thongbao_screen();
							setbkcolor(15);
							settextstyle(8, HORIZ_DIR, 2);
							setcolor(0);
							outtextxy(468, 570, "Click vao dinh khac de chon dinh ket thuc");
						}
					}
					else
					{
						int t;
						for (int i= 0;i<= dt.TongSo_Dinh;i++)
							if (kc(x, y, dt.node[i].x  , dt.node[i].y  ) <= 20 && i != tam)
							{
								tao_node(dt.node[i].x  , dt.node[i].y , 2);
								dem = 0;
								t = i;
							}
						if (dem == 0)
						{
							thongbao_screen();
							settextstyle(8, HORIZ_DIR, 1);
							outtextxy(468, 570, "Dinh that giua 2 dinh ");
							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(270, 570, dt.node[tam].name);
							outtextxy(748, 570, "-->");
							outtextxy(400, 570, dt.node[t].name);
							settextstyle(8, HORIZ_DIR, 1);
							outtextxy(840, 570, ":");
							Dinh_That(tam, t);
							kt_DThat = false;
						}
					}
				}
		
			////////////////////////////////////////////////////////////////////////////////////////////////////////
				if (kt_rect(400, 8, 500, 40, x, y) == true)
				{ //add
					load_node();
					thuattoan_tool_screen();
					settextstyle(0, 0, 2);
					Add(true);
					if (dt.TongSo_Dinh > MAX_NUT - 2)
					{
						thongbao_screen();
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(0);
						outtextxy(468, 570, "DA DU       DINH!!");
						char chuoi[10];			  //
						itoa(MAX_NUT, chuoi, 10); //char *itoa ( int value, char * str, int cos so );
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(550, 570, chuoi);
						set_false();
						continue;
					}
					thongbao_screen();
					if (working) ////chi ADD duoc khi da NEW do thi hoac OPEN do thi
					{
						thongbao_screen();
						set_true(kt_Add);
						settextstyle(8, HORIZ_DIR, 2);
						setbkcolor(15);
						setcolor(0);
						outtextxy(468, 570, "Click chuot trai vao khoang trong de tao dinh moi");
						outtextxy(468, 590, "So dinh <= 15");
					}
					else
					{
						setbkcolor(15);
						setcolor(4);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc");
					}
				}

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				if (kt_Add && kt_rect(480, 80, getmaxx() - 30, 550, x, y)) //add++
				{
					if (dt.TongSo_Dinh > MAX_NUT - 2) //
					{
						thongbao_screen();
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(4);
						outtextxy(468, 570, "Da du dinh, nen khong the them !!");
						char chuoi[10];			  //
						itoa(MAX_NUT, chuoi, 10); //char *itoa ( int value, char * str, int cos so );
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(550, 570, chuoi);
						continue;
					}

					bool kiemtra = false;
					for (int i = 0; i <= dt.TongSo_Dinh; i++)// kiem tra khoang cach den cac diem con lai
						if (kc(x, y, dt.node[i].x  , dt.node[i].y  ) <= 45)
						{
							kiemtra = true;
							break;
						}
					if (kiemtra == true)
						continue;
					tao_node(x, y, 4);
					thongbao_screen();
					settextstyle(8, HORIZ_DIR, 2);
					setcolor(0);
					outtextxy(468, 570, "Nhap ten dinh :");
					outtextxy(468, 590, "(Ten dinh theo dinh dang: 00->99 va khac ten da ton tai)");
					outtextxy(468, 610, "So dinh <=15");
					setcolor(4);
					char s[2];
					for (int i = 0; i <= 2; i++)
						s[i] = '\0'; //ngat chuoi o ki tu thu 2
					int d = 0;
					char xx;
					while (true) //vd nhap 36
					{
						xx = getch(); //xx1=3		,xx2=9  , xx3=6
						if (!(('0' <= xx && xx <= '9') || xx == 13 || xx == 8))
							continue;
						if (d == 0)
						{
							if ('0' <= xx && xx <= '9')
							{
								s[0] = xx;
								d = 1; //s[0]=3
							}
						}
						else if (d == 1)
						{
							if ('0' <= xx && xx <= '9')
							{
								s[1] = xx; //s[1]==xx2=9
								d = 2;
							}

							else if (xx == 8)
							{ //sau khi nhan phim backspace thi ngat chuoi
								for (int i = 0; i <= 2; i++)
								{
									s[i] = '\0';
								} //s[0]='\0' =>>xoa luon phan tu dau
								d = 0;
							}
						}
						else if (d == 2)
						{
							if (xx == 8)
							{ //sau khi nhan phim backspace thi ngat chuoi
								for (int i = 1; i <= 2; i++)
								{
									s[i] = '\0'; //s[0]=3, s[1]='\0'
								}
								d = 1;
							}

							else if ('0' <= xx && xx <= '9')
							{
								s[1] = xx; //s[1]=6 update s[1] cho moi lan nhan getch() moi
								d = 2;
							}

							else if (xx == 13)
							{
								bool kt2 = true;
								for (int i = 0; i <= dt.TongSo_Dinh; i++)
									if (dt.node[i].name[0] == s[0] && dt.node[i].name[1] == s[1])
									{
										kt2 = false;
										setcolor(4);
										outtextxy(468, 630, "Ten dinh da ton tai, moi nhap ten khac !!");
										delay(TIME1);
									}
								if (kt2)
								{
									dt.TongSo_Dinh++;
									dt.init_Node(1, 1);
									dt.init_MT(1, 1);
									dt.node[dt.TongSo_Dinh].x = x  ; //dua vi tri x=x  cho node [TongSo_Dinh].x
									dt.node[dt.TongSo_Dinh].y = y  ;	 //dua vi tri y=y  cho node [TongSo_Dinh].y
									dt.node[dt.TongSo_Dinh].name[0] = s[0];
									dt.node[dt.TongSo_Dinh].name[1] = s[1];
									dt.node[dt.TongSo_Dinh].name[2] = '\0';
									load_node();
									load_matrix();
									thongbao_screen();
									break;
								}
							}
						}
						thongbao_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(0);
						outtextxy(468, 570, "Nhap ten dinh :");
						outtextxy(468, 590, "(Ten dinh theo dinh dang: 00->99 va khac ten da ton tai)");
						outtextxy(468, 610, "So dinh <=15");
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(12);
						outtextxy(768, 570, s);
					}
				}
				if (kt_rect(520, 8, 620, 40, x, y)) //delete
				{
					thongbao_screen();
					load_node();
					thuattoan_tool_screen();
					settextstyle(0, 0, 2);
					Delete(true);
					settextstyle(8, HORIZ_DIR, 2);
					if (working) //chi DELETE duoc khi da NEW do thi hoac OPEN do thi
					{
						thongbao_screen();
						set_true(kt_Del);
						settextstyle(8, HORIZ_DIR, 2);
						if (dt.TongSo_Dinh == -1)
						{
							thongbao_screen();
							setcolor(4);	setbkcolor(15);
							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(468, 590, "Ban phai mo do thi hoac tao do thi moi truoc");
							outtextxy(468, 570, "Do thi khong co dinh => khong the xoa");
							set_false();
						}
						else
						{
							settextstyle(8, HORIZ_DIR, 2);
							setbkcolor(15);
							setcolor(4);
							outtextxy(468, 570, "Click chuot trai vao dinh can xoa");
						}
					}
					else
					{
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(4);
						outtextxy(468, 570, "Ban can mo hoac tao do thi moi truoc");
					}
				}

				if (kt_Del && kt_rect(460, 60, getmaxx() - 10, 560, x, y)) //delete++
				{
					if (dt.TongSo_Dinh == -1)
					{
						thongbao_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(4);	outtextxy(468, 570, "Do thi khong co dinh => khong the xoa");
						continue;
					}
					for (int i = 0; i <= dt.TongSo_Dinh; i++)
						if (kc(x,y,dt.node[i].x,dt.node[i].y)<=20){
							dt.init_Node(-1, i);
							dt.TongSo_Dinh--;
							dt.init_MT(-1, i);
							break;
						}
				load_node();
				load_matrix();
				}

				if (kt_rect(640, 8, 740, 40, x, y)) //move
				{
					thongbao_screen();
					load_node();
					thuattoan_tool_screen();
					thongbao_screen();
					settextstyle(0, 0, 2);
					Move(true);
					if (working) //chi MOVE duoc khi da NEW haoc OPEN
					{
						if (dt.TongSo_Dinh == -1)
						{
							thongbao_screen();
							settextstyle(8, HORIZ_DIR, 2);
							setbkcolor(15);
							setcolor(4);
							outtextxy(468, 570, "Do thi khong co dinh");
							set_false();
						}
						else
						{
							thongbao_screen();
							set_true(kt_Move);
							settextstyle(8, HORIZ_DIR, 2);
							setcolor(0);
							setbkcolor(15);
							outtextxy(468, 570, "Click chuot trai lan 1 vao dinh de chon dinh!!");
							outtextxy(468, 600, "Click chuot trai lan 2 vao khoang trong de di chuyen dinh toi toa do moi!!");
						}
					}
					else
					{
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(4);
						setbkcolor(15);
						outtextxy(468, 570, "Ban can mo do thi hoac tao do thi moi truoc");
					}
				}

				if (kt_Move && kt_rect(460, 60, getmaxx() - 10, 560, x, y))
				{
					if (dem == 0)
					{
						for (int i = 0; i <= dt.TongSo_Dinh; i++)
						{
							if (kc(x, y, dt.node[i].x  , dt.node[i].y  ) <= 20) //vitri mouse so vs vitri nut [i] can di chuyen khong qua 20pixel
							{
								dem = 1;
								tam = i;
								tao_node(dt.node[tam].x  , dt.node[tam].y  , 12);
								draw_Node(i, 14);
								break;
							}
						}
					}
					else if (dem == 1) //da chon duoc nut can MOVE
					{
						bool kiemtra = true;
						for (int i = 0; i <= dt.TongSo_Dinh; i++)
						{
							if (kc(x, y, dt.node[i].x  , dt.node[i].y  ) <= 20)
								kiemtra = false; //neu khoang cach vi tri chuot( vitri nut moi) so vs nut cu >=45pixel =>cho tao nut co vitri moi
						}
						if (kiemtra == true)
						{
							dt.node[tam].x = x  ; //gan vitri nut cu (x ) cho nut moi
							dt.node[tam].y = y  ;  //gan vitri nut cu (y ) cho nut moi
							load_node();
							dem = 0;
						}
					}
				}

				if (kt_rect(760, 8, 880, 40, x, y))
				{ //connect
					thongbao_screen();
					load_node();
					thuattoan_tool_screen();
					settextstyle(0, 0, 2);
					Connect(true);
					settextstyle(8, HORIZ_DIR, 2);
					if (working)
					{
						dem = 0;
						if (dt.TongSo_Dinh < 1)
						{
							thongbao_screen();
							settextstyle(8, HORIZ_DIR, 2);
							set_false();
						}
						else
						{
							thongbao_screen();
							set_true(kt_Connect);
							settextstyle(8, HORIZ_DIR, 2);
							setbkcolor(15);
							setcolor(0);
							outtextxy(468, 570, "Click chuot trai lan luot vao A va B de tao cung");
							outtextxy(468, 600, "Neu cung AB da ton tai, click lan 2 de xoa hoac chinh sua");
						}
					}
					else
					{
						setbkcolor(15);
						setcolor(4);
						outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc");
					}
				}
				if (kt_Connect == true && dt.TongSo_Dinh >= 1 && kt_rect(460, 60, getmaxx() - 10, 560, x, y) == true) //co it nhat 2 nut
				{
					settextstyle(8, HORIZ_DIR, 2);
					if (dem == 0)
					{
						for (int i = 0; i <= dt.TongSo_Dinh; i++)
						{
							if (kc(x, y, dt.node[i].x  , dt.node[i].y  ) <= 20)
							{
								dem = 1;
								tam1 = i;
								tao_node(dt.node[tam1].x  , dt.node[tam1].y  , 12);
								draw_Node(i, 14);
								break;
							}
						}
					}
					else if (dem == 1)
					{
						bool kt1 = false, kt2 = false, kt3 = false; //kt1: kiemtra xem diem B co trung voi diem A khong, KHONG=>kt1=true;

						for (int i = 0; i <= dt.TongSo_Dinh; i++)
						{
							if (kc(x, y, dt.node[i].x  , dt.node[i].y  ) <= 20 && i != tam1)
							{
								kt1 = true;
								tam2 = i;
								tao_node(dt.node[tam2].x  , dt.node[tam2].y  , 12);
								draw_Node(i, 14);
								break;
							}
						}

						if (kt1 == false)
							continue;

						for (int i = 0; i <= dt.TongSo_Dinh; i++)
						{
							for (int j = 0; j <= dt.TongSo_Dinh; j++)
							{
								if (dt.MT[tam1][tam2] != 0)
									kt2 = true; //kt2: kiem tra co cung tu A->B khong
							}
						}
						for (int i = 0; i <= dt.TongSo_Dinh; i++)
						{
							for (int j = 0; j <= dt.TongSo_Dinh; j++)
							{
								if (dt.MT[tam1][tam2] != 0)
									kt3 = true; //kt2: kiem tra co cung tu B->A khong
							}
						}
						if (kt1 == true && kt2 == false && kt3 == true) //A khong trung B, khong co A->B, co B->A
						{
							dem = 0;
							thongbao_screen();
							setcolor(0);
							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(468, 570, "Nhap trong so cung: ");
							outtextxy(468, 590, "0 < Trong so < 1000");

							setcolor(2);
							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(800, 570, dt.node[tam1].name);

							settextstyle(8, HORIZ_DIR, 1);
							outtextxy(840, 570, "->");

							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(880, 570, dt.node[tam2].name);

							settextstyle(8, HORIZ_DIR, 1);
							outtextxy(920, 570, ":");
							char s[20];
							int d = 0;
							setcolor(4);
							while (true)
							{
								s[d] = getch();
								if (!(('0' <= s[d] && s[d] <= '9') || s[d] == 13 || s[d] == 8))
									continue;
								if (d == 3)
								{
									if (s[d] != 13 && s[d] != 8)
									{
										s[2] = s[3];
										d--;
									}
								} //d<=2
								if (s[d] == 13)
								{
									if (d == 0)
										continue;
									else
									{ //d=1 or 2
										settextstyle(8, HORIZ_DIR, 2);
										char xau[d];
										for (int i = 0; i <= d - 1; i++)
										{
											xau[i] = s[i];
											xau[d] = '\0';
										}
										int t = atoi(xau); //atoi: transform str to int
										settextstyle(8, HORIZ_DIR, 2);
										dt.MT[tam1][tam2] = t;
										thongbao_screen();
										load_node();
										load_matrix();
										break;
									}
								}
								else if (s[d] == 8)
								{
									setfillstyle(1, 15);
									bar(950 + 10 * (d - 1), 570, 950 + 10 * (d), 590);
									d--;
									if (d == -1)
										d = 0;
									continue;
								}
								else
									setcolor(1);
								OutPut(950 + 10 * d, 570, s[d]);
								d++;
							}
						}
						if (kt1 == true && kt2 == false && kt3 == false) //A khong trung B, khong co A->B, khong co B->A
						{
							dem = 0;
							thongbao_screen();
							setcolor(0);
							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(468, 570, "Nhap trong so cung: ");
							outtextxy(468, 590, "0 < Trong so < 1000");

							setcolor(2);
							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(800, 570, dt.node[tam1].name);

							settextstyle(8, HORIZ_DIR, 1);
							outtextxy(840, 570, "->");

							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(880, 570, dt.node[tam2].name);

							settextstyle(8, HORIZ_DIR, 1);
							outtextxy(920, 570, ":");
							setcolor(4);
							char s[20];
							int d = 0;
							fflush(stdin); //xoa bo dem, vd bo ky tu ENTER
							while (true)
							{
								s[d] = getch();
								if (!(('0' <= s[d] && s[d] <= '9') || s[d] == 13 || s[d] == 8))
									continue;
								if (d == 3)
								{
									if (s[d] != 13 && s[d] != 8)
									{
										s[2] = s[3];
										d--;
									}
								} //d<=2
								if (s[d] == 13)
								{
									if (d == 0)
										continue;
									else
									{ //d=1 or 2
										settextstyle(8, HORIZ_DIR, 2);
										char xau[d];
										for (int i = 0; i <= d - 1; i++)
										{
											xau[i] = s[i];
											xau[d] = '\0'; //ket thuc chuoi
										}
										int t = atoi(xau); //atoi: transform str to int
										settextstyle(8, HORIZ_DIR, 2);
										dt.MT[tam1][tam2] = t;
										thongbao_screen();
										load_node();
										load_matrix();
										break;
									}
								}
								else if (s[d] == 8)
								{
									setfillstyle(1, 15);
									bar(950 + 10 * (d - 1), 570, 950 + 10 * (d), 590);
									d--;
									if (d == -1)
										d = 0;
									continue;
								}
								else
									setcolor(1);
								OutPut(950 + 10 * d, 570, s[d]);
								d++;
							}
						}
						bool kt_Change = false, kt_Remove = false, kt_Back = false;
						dem = 0;
						if (kt1 == true && kt2 == true) //A khong trung B, khong co B->A
						{
							thongbao_screen();
							settextstyle(8, HORIZ_DIR, 2);
							setcolor(2);
							outtextxy(468, 570, "0  Quay lai");
							outtextxy(468, 590, "1  Xoa cung");
							outtextxy(468, 610, "2  Chinh sua trong so");
							outtextxy(468, 630, "nhap lua chon :");
							setcolor(4);
							int s[2];
							fflush(stdin);
							while (true)
							{
								s[0] = getch();

							BI_TRUNG_LUA_CHON:
								if (!(s[0] == '0' || s[0] == '1' || s[0] == '2'))
									continue;
								OutPut(654, 630, s[0]);

							NHAP_SAI_PHIM:
								s[1] = getch();
								if (!(s[1] == '0' || s[1] == '1' || s[1] == '2' || s[1] == 13))
									goto NHAP_SAI_PHIM;

								if (s[1] == 13)
								{
									settextstyle(0, 0, 1);
									if (s[0] == '0')
										kt_Back = true; //quay lai
									else if (s[0] == '1')
										kt_Remove = true; //xoa cung
									else if (s[0] == '2')
										kt_Change = true; //chinh trong so
									break;
								}
								else
								{
									s[0] = s[1];
									goto BI_TRUNG_LUA_CHON;
								}
							}
						}
						if (kt_Back == true)
						{
							load_node();
							load_matrix();
							thongbao_screen();
						}
						if (kt_Remove == true)
						{
							dt.MT[tam1][tam2] = 0; //xoa cung dang xet
							load_node();
							load_matrix();
							thongbao_screen();
						}
						if (kt_Change)
						{
							tao_node(dt.node[tam1].x  , dt.node[tam1].y  , 4);
							draw_Node(tam1, 14);
							tao_node(dt.node[tam2].x  , dt.node[tam2].y  , 4);
							draw_Node(tam2, 14);
							thongbao_screen();

							setcolor(0);
							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(468, 570, "Nhap trong so cung: ");
							outtextxy(468, 590, "0 < Trong so < 1000");

							setcolor(2);
							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(800, 570, dt.node[tam1].name);

							settextstyle(8, HORIZ_DIR, 1);
							outtextxy(840, 570, "->");

							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(880, 570, dt.node[tam2].name);

							settextstyle(8, HORIZ_DIR, 1);
							outtextxy(920, 570, ":");

							setcolor(4);
							char s[15];
							int d = 0;
							fflush(stdin);
							while (true)
							{
								s[d] = getch();
								if (!(('0' <= s[d] && s[d] <= '9') || s[d] == 13 || s[d] == 8))
									continue;
								if (d == 3)
								{
									if (s[d] != 13 && s[d] != 8)
									{
										s[2] = s[3];
										d--;
									}
								}
								if (s[d] == 13)
								{
									if (d == 0)
										continue;
									else
									{ //d=1 or 2
										settextstyle(8, HORIZ_DIR, 2);
										char xau[d];
										for (int i = 0; i <= d - 1; i++)
										{
											xau[i] = s[i];
											xau[d] = '\0'; //ket thuc chuoi
										}
										int t = atoi(xau); //atoi: transform str to int
										settextstyle(8, HORIZ_DIR, 2);
										thongbao_screen();
										dt.MT[tam1][tam2] = t;
										load_node();
										load_matrix();
										break;
									}
								}
								else if (s[d] == 8)
								{
									setfillstyle(1, 15);
									bar(950 + 10 * (d - 1), 570, 950 + 10 * (d), 590);
									d--;
									if (d == -1)
										d = 0;
									else
										continue;
								}
								else
									setcolor(1);
								OutPut(950 + 10 * d, 570, s[d]);
								d++;
							}
						}
					}
					{
						thongbao_screen();
						set_true(kt_Connect);
						settextstyle(8, HORIZ_DIR, 2);
						setbkcolor(15);
						setcolor(0);
						outtextxy(468, 570, "Click chuot trai lan luot vao A va B de tao cung");
						outtextxy(468, 600, "Neu cung AB da ton tai, click lan 2 de xoa hoac chinh sua");
					}
				}

				if (kt_rect(900, 8, 1015, 40, x, y))
				{ //Rename
					thongbao_screen();
					load_node();
					thuattoan_tool_screen();
					settextstyle(0, 0, 2);
					setbkcolor(15);
					Rename(true);
					if (working) //chi Rename duoc khi da NEW do thi hoac OPEN do thi
					{
						if (dt.TongSo_Dinh == -1)
						{
							thongbao_screen();
							settextstyle(8, HORIZ_DIR, 2);
							setcolor(4);
							setbkcolor(15);
							outtextxy(468, 570, "Do thi khong co dinh => khong the doi ten");
							set_false();
							continue;
						}
						thongbao_screen();
						set_true(kt_Rename);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(0);
						outtextxy(468, 570, "Click chuot trai vao dinh de doi ten");
					}
					else
					{
						setbkcolor(15);
						setcolor(4);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Ban can mo hoac tao do thi moi truoc");
					}
				}

				if (kt_Rename && kt_rect(460, 60, getmaxx() - 10, 560, x, y))
				{
					for (int i = 0; i <= dt.TongSo_Dinh; i++)
						if (kc(x, y, dt.node[i].x  , dt.node[i].y  ) <= 20)
						{
							thongbao_screen();
							tao_node(dt.node[i].x  , dt.node[i].y  , 4);
							draw_Node(i, 14);

							settextstyle(8, HORIZ_DIR, 2);
							setcolor(0);
							outtextxy(468, 570, "Nhap ten dinh :");
							outtextxy(468, 590, "(Ten dinh theo dinh dang: 00->99 va khac ten da ton tai)");
							outtextxy(468, 610, "So dinh <=15");
							setcolor(4);
							char s[2];
							for (int j = 0; j <= 2; j++)
								s[j] = '\0'; //ngat chuoi o ki tu thu 2
							int d = 0;
							char xx;
							while (true) //vd nhap 36
							{
								xx = getch(); //xx1=3		,xx2=9  , xx3=6
								if (!(('0' <= xx && xx <= '9') || xx == 13 || xx == 8))
									continue;

								if (d == 0)
								{
									if ('0' <= xx && xx <= '9')
									{
										s[0] = xx;
										d = 1; //s[0]=3
									}
								}

								else if (d == 1)
								{
									if ('0' <= xx && xx <= '9')
									{
										s[1] = xx; //s[1]==xx2=9
										d = 2;
									}

									else if (xx == 8)
									{ //sau khi nhan phim backspace thi ngat chuoi
										for (int i = 0; i <= 2; i++)
										{
											s[i] = '\0';
										} //s[0]='\0' =>>xoa luon phan tu dau
										d = 0;
									}
								}

								else if (d == 2)
								{
									if (xx == 8)
									{ //sau khi nhan phim backspace thi ngat chuoi
										for (int i = 1; i <= 2; i++)
										{
											s[i] = '\0'; //s[0]=3, s[1]='\0'
										}
										d = 1;
									}

									else if ('0' <= xx && xx <= '9')
									{
										s[1] = xx; //s[1]=6 update s[1] cho moi lan nhan getch() moi
										d = 2;
									}

									else if (xx == 13)
									{
										bool kt2 = true;
										for (int i = 0; i <= dt.TongSo_Dinh; i++)
											if (dt.node[i].name[0] == s[0] && dt.node[i].name[1] == s[1])
											{
												kt2 = false;
												setcolor(4);
												outtextxy(468, 630, "Ten dinh da ton tai, moi nhap ten khac !!");
												delay(TIME1);
											}
										if (kt2)
										{
											dt.node[i].name[0] = s[0];
											dt.node[i].name[1] = s[1];
											dt.node[i].name[2] = '\0';
											load_node();
											thongbao_screen();
											load_matrix();
											break;
										}
									}
								}
								thongbao_screen();
								settextstyle(8, HORIZ_DIR, 2);
								setcolor(0);
								outtextxy(468, 570, "Nhap ten dinh :");
								outtextxy(468, 590, "(Ten dinh theo dinh dang: 00->99 va khac ten da ton tai)");
								outtextxy(468, 610, "So dinh <=15");
								settextstyle(8, HORIZ_DIR, 2);
								setcolor(12);
								outtextxy(768, 570, s);
							}
						}
				}
				////////////////////////////////////////////////////////////////////////////////DFS//////////////////////////////////////////////////////////////////////
				if (kt_rect(25, 475, 140, 525, x, y))
				{
					thuattoan_tool_screen();
					settextstyle(0, 0, 2);
					DFS(true);
					load_node();
					if (working == false)
					{
						thongbao_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(4);
						setbkcolor(15);
						outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc");
						continue;
					}
					if (dt.TongSo_Dinh < 0)
					{
						thongbao_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(4);
						setbkcolor(15);
						outtextxy(468, 570, "Do thi chua co dinh");
						set_false();
						continue;
					}
					set_true(kt_DFS);
					thongbao_screen();
					settextstyle(8, HORIZ_DIR, 2);
					setcolor(0);
					setbkcolor(15);
					outtextxy(468, 570, "Click vao dinh bat ki de duyet DFS");
				}
				if (kt_DFS == true && kt_rect(460, 60, getmaxx() - 10, 560, x, y) == true)
				{
					int temp = -1;
					for (int i = 0; i <= dt.TongSo_Dinh; i++)
					{
						if (kt_circle(x, y, dt.node[i].x  , dt.node[i].y  ))
							temp = i;
					}
					if (temp != -1)
					{
						load_node();
						DFS(temp);
						kt_DFS = false;
					}
				}

				////////////////////////////////////////////////////////////////////////////////BFS//////////////////////////////////////////////////////////////////////
				if (kt_rect(170, 475, 290, 525, x, y))
				{
					thuattoan_tool_screen();
					settextstyle(0, 0, 2);
					BFS(true);
					load_node();
					if (working == false)
					{
						thongbao_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(4);
						setbkcolor(15);
						outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc");
						continue;
					}
					if (dt.TongSo_Dinh < 0)
					{
						thongbao_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(4);
						setbkcolor(15);
						outtextxy(468, 570, "Do thi chua co dinh");
						set_false();
						continue;
					}
					set_true(kt_BFS);
					thongbao_screen();
					settextstyle(8, HORIZ_DIR, 2);
					setcolor(0);
					setbkcolor(15);
					outtextxy(468, 570, "Click vao dinh bat ki de duyet DFS");
				}

				if (kt_BFS == true && kt_rect(460, 60, getmaxx() - 10, 560, x, y) == true)
				{
					int temp = -1;
					for (int i = 0; i <= dt.TongSo_Dinh; i++)
					{
						if (kt_circle(x, y, dt.node[i].x  , dt.node[i].y  ))
							temp = i;
					}
					if (temp != -1)
					{
						load_node();
						BFS(temp);
						kt_BFS = false;
					}
				}
				///////////////////////////////////////////////////////////////////XY///////////////////////////////////////////////////////////////////
				if (kt_rect(320, 540, 430, 590, x, y))
				{
					thuattoan_tool_screen();
					settextstyle(0, 0, 2);
					Xy(true);
					load_node();
					if (working == false)
					{
						thongbao_screen();
						setbkcolor(15);
						setcolor(4);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc");
						continue;
					}
					if (dt.TongSo_Dinh < 1)
					{
						thongbao_screen();
						setcolor(4);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Do thi phai co it nhat 2 dinh");
						set_false();
						continue;
					}
					dem = 0;
					thongbao_screen();
					setcolor(0);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Click vao dinh bat ki de chon dinh bat dau");
					set_true(kt_XY);
				}

				if (kt_XY == true && kt_rect(460, 60, getmaxx() - 10, 560, x, y) == true)
				{
					int start;
					int finish;
					if (dem == 0)
					{
						for (int i = 0; i <= dt.TongSo_Dinh; i++)
						{
							if (kt_circle(x, y, dt.node[i].x  , dt.node[i].y  ))
							{
								start = i;
								dem++;
							}
						}
						tao_node(dt.node[start].x  , dt.node[start].y  , 4);
						draw_Node(start, 14);
						thongbao_screen();
						setcolor(0);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Click vao dinh bat ki de chon dinh ket thuc");
					}
					else
					{
						for (int i = 0; i <= dt.TongSo_Dinh; i++)
						{
							if (kt_circle(x, y, dt.node[i].x  , dt.node[i].y  ) and i != start)
							{
								tao_node(dt.node[i].x  , dt.node[i].y  , 4);
								draw_Node(i, 14);
								dem = 0; //da co du 2 dinh start va finish
								finish = i;
							}
						}
						if (dem == 0) //da co du 2 dinh start va finish
						{
							thongbao_screen();
							setcolor(4);
							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(468, 570, "Duong di ngan nhat tu ");
							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(748, 570, dt.node[start].name);
							settextstyle(8, HORIZ_DIR, 1);
							outtextxy(775, 570, "->");
							outtextxy(830, 570, ":");
							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(800, 570, dt.node[finish].name);
							Dijkstra(start, finish);
							kt_XY = false;
						}
					}
				}
				if (kt_rect(170, 540, 290, 590, x, y)) //hamilton
				{
					thuattoan_tool_screen();
					load_node();
					settextstyle(0, 0, 2);
					Ham(true);
					if (working == false)
					{
						thongbao_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setbkcolor(15);
						setcolor(4);
						outtextxy(468, 570, "Ban can phai mo do thi hoac tao do thi moi truoc!");
						continue;
					}
					if (dt.TongSo_Dinh < 0)
					{
						thongbao_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setbkcolor(15);
						setcolor(4);
						outtextxy(468, 570, "Do thi chua co dinh!");
						set_false();
						continue;
					}
					thongbao_screen();
					set_true(kt_Ham);
					thongbao_screen();
					settextstyle(8, HORIZ_DIR, 2);
					setbkcolor(15);
					setcolor(4);
					outtextxy(468, 570, "Click vao dinh muon bat dau!");
				}
				if (kt_Ham && kt_rect(460, 60, getmaxx() - 10, 560, x, y))
				{
					for (int i = 0; i <= dt.TongSo_Dinh; i++)
						if (kt_circle(x, y, dt.node[i].x  , dt.node[i].y  ))
						{
							load_node();
							Hamilton(i);
							break;
						}
				}

//////////////////////////////////////////////THANH PHAN LIEN THONG/////////////////////////////////////
				if (kt_rect(25, 620, 215, 690, x, y))
				{
					thuattoan_tool_screen();
					work_screen();
					thongbao_screen();
					settextstyle(0, 0, 2);
					Lienthong(true);
					load_node();
					if (working == false)
					{
						thongbao_screen();
						setcolor(0);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc");
						continue;
					}
					if (dt.TongSo_Dinh < 0)
					{
						thongbao_screen();
						setcolor(0);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Do thi chua co dinh");
						set_false();
						continue;
					}
					thongbao_screen();
					TPLT(dt.MT);
					set_false();
				}
/////////////////////////////////////////////DINH TRU/////////////////////////////////////
				if (kt_rect(240,620,430,690,x,y)){
				 	thuattoan_tool_screen();
				 	work_screen();
				 	load_node();
					settextstyle(0, 0, 2);		 	
				 	Dinhtru( true );				
					if(working==false){
						thongbao_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(4);
						setbkcolor(15);
						outtextxy(468,570,"Ban phai mo do thi hoac tao do thi moi truoc")	;
						continue;
					}
					if(dt.TongSo_Dinh<0){
						thongbao_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(4);
						setbkcolor(15);
						outtextxy(468,570,"Do thi chua co dinh");
						set_false();
							continue;										
					}	
					dinh_tru();
					set_false();			
				}
/////////////////////////////////////////////CANH CAU/////////////////////////////////////				
				if (kt_rect(240,705,430,775,x,y)){
					thuattoan_tool_screen();
					work_screen();
					load_node();
					if(working==false){
						thongbao_screen();
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468,570,"Ban phai mo do thi hoac tao do thi moi truoc")	;
						continue;
					}
					if(dt.TongSo_Dinh<0){
						thongbao_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(4);
						setbkcolor(15);
						outtextxy(468,570,"Do thi phai co it nhat 2 dinh de tao thanh canh! ");	
						set_false();
						continue;										
					}		
					thongbao_screen();	
					Canh_Cau();		
					set_false();
				}
			}
			clearMouse();
			while (kbhit())
			{
				char s = getch();
				if (s == 27)
				{
					dt.delete_Data();
					return 0;
				}
			}
			delay(0.0001);
		}
		return 0;
	}
