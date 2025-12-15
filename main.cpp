//头文件↓
#include"stdio.h"
#include"stdlib.h"
#include"graphics.h"
#include"time.h"
#include"conio.h"
#include"string"
#include"algorithm"

//宏定义↓
#define 食品 "shipin"
#define 化妆品 "huazhuangpin"
#define 日用品 "riyongpin"
#define 饮料 "yinliao"
#define HEIGHTONE 640
#define WIDTHONE 550
#define HEIGHTTWO 1024
#define WIDTHTWO 640
using namespace std;
//类名重定义↓
typedef unsigned int COLOR;
typedef unsigned int ZONG;

int weightNow = -1;//当前登录用户的身份标识码
string NAME;//当前登录用户的账号

struct GoodsMessage//商品信息结构体
{
	string name;//商品名
	int sum;//剩余库存  假设每个商品
	string brind;//品牌  假设品牌有a,b,c,d 4个品牌
	float price;//单价
	string bornland;//生产厂家   假设生产厂家只有A,B,C,D 4家
	string borndate;//生产时期   假设取值均在  2021年的6月1日至6月3日
	struct GoodsMessage* next;
};
struct Kind//商品种类结构体
{
	string classname;//商品类名
	int zsum;//该类商品种类个数  假设4类库存分别如下赋值（4，3，3，3）
 	struct GoodsMessage* next;
}kind[4] = { {食品,4,NULL},{化妆品,3,NULL},{日用品,3,NULL},{饮料,3,NULL}};
struct User//用户信息结构体
{
	char name[10];//用户账户名
	char passwd[10];//用户密码
	char weight;//0为普通用户，1为管理员
	struct User* next;
}*users = new struct User;

ZONG Zong = 0;//


//辅助函数↓
int _SORT(int, int);//sort排序的辅助函数

//商品信息操作函数
int GoodsSell(const char*, const char*,const char*,int&,float&,int);
void GoodsImportMenu();
void GoodsStart(int , string , int , string , float , string ,string);
int StoreMessage();//保存至本地
int ReadStore();//从本地读出
int TeamData(string*, int&, int);
int FindData(string*, string&);
int FillData(string*);

//用户信息操作函数↓
void UserStart(struct User** First);//仅调用一次
int UserCreat(const char*, const char*, const char*);
int UserChangePasswd(const char*);//普通用户修改密码 ，管理员重置普通用户密码

//窗口创建函数↓
int WindowOne();//登录界面
void WindowTwo();//主操作界面
void WindowThree();//添加用户界面
void WindowFour();//密码管理界面
void WindowFive();//销售功能界面
void WindowSix();//商品管理界面
void WindowSeven();//商品仓库界面

//鼠标操作函数↓
int MouseFuncOne(RECT &rect,MOUSEMSG &mouse );//检测鼠标当前位置是否在指定区域
void MouseFuncTwo(RECT& rect);//在指定区域画框

//窗口特效函数↓
void DrawStars();


int main()
{

	UserStart(&users);//初始化第一个用户（即admin 123456）
	GoodsImportMenu();
	//↑给超市添加货物（由于题中仅要求有4类但没有要求这4类分别有哪些商品,故在此自定义设置）

	srand((unsigned int)time(NULL));//置随机数种子
	WindowOne();//登录界面初始化
	return 0;
}
void GoodsImportMenu()
{
/*
GoodsStart参数：
	第一个参数：
		0表示食品类（shipin）  1表示化妆品类   3表示日用品类  4表示饮料类
	第二个参数：
		所属类的商品名；如 yurou （鱼肉）
	第三个参数：
		该商品的库藏量
	第四个参数：
		该商品所属品牌
	第五个参数：
		该商品单价
	第六个参数：
		该商品的产地
	第七个参数：
		该商品的生产日期
*/
	GoodsStart(0, "yurou", 30, "aa", 10.5, "AA","2021/6/1");
	GoodsStart(0, "yangrou", 60, "cc", 20.5, "CC","2021/6/2");
	GoodsStart(0, "baicai", 300, "aa", 1.5, "DD","2021/6/3");
	GoodsStart(0, "luobo", 200, "aa", 2.5, "AA", "2021/6/1");

	GoodsStart(1, "kouhong", 100, "bb", 1000.5, "CC","2021/6/2");
	GoodsStart(1, "yanzhi", 30, "aa", 11.5, "DD", "2021/6/2");
	GoodsStart(1, "xiangshui", 80, "cc", 1100.5, "BB", "2021/6/3");

	GoodsStart(2, "chabei", 150, "aa", 10.0, "BB", "2021/6/1");
	GoodsStart(2, "yusan", 80, "cc", 20.0, "AA", "2021/6/1");
	GoodsStart(2, "kuaizi", 0, "bb", 11.5, "CC", "2021/6/3");

	GoodsStart(3, "hongcha", 200, "bb", 4.0, "DD", "2021/6/2");
	GoodsStart(3, "lvcha", 192, "bb", 4.5, "BB", "2021/6/1");
	GoodsStart(3, "kele", 178, "aa", 3.5, "DD", "2021/6/3");
}
void GoodsStart(int x,string x1,int x2,string x3,float x4,string x5,string x6)//类别序号、商品名称、库存、品牌、单价、产地、生产日期
{
	if (kind[x].next == NULL)
	{
		kind[x].next = new struct GoodsMessage;
		kind[x].next->name = x1;
		kind[x].next->sum = x2;
		kind[x].next->brind = x3;
		kind[x].next->price = x4;
		kind[x].next->bornland = x5;
		kind[x].next->borndate = x6;
		kind[x].next->next= NULL;
		return;
	}
	struct GoodsMessage* p = kind[x].next;
	struct GoodsMessage* pre = p;
	while (p)
	{
		pre = p;
		p = p->next;
	}
	p = new struct GoodsMessage;
	p->name = x1;
	p->sum = x2;
	p->brind = x3;
	p->price = x4;
	p->bornland = x5;
	p->borndate = x6;
	p->next = NULL;
	pre->next = p;
}
void UserStart(struct User**First)
{
	sprintf((*First)->name, "%s", "admin");
	sprintf((*First)->passwd, "%s", "123456");
	(*First)->weight = 1;
	(*First)->next = NULL;
}
int WindowOne()
{
	char wwwww = '\0',www='\0';
	int p = 0, p1 = 0, p2 = 0, p3 = 0, p4 = 0;//工具变量
	char pp = '\0', P = '\0';//工具变量
	string name, passwd,passwdx;
	RECT rect1 = { 150,400,300,450 }, rect2 = { 350,400,500,450 };//登录，退出位置
	RECT rect3 = { 200,200,300,250 }, rect4 = { 200,300,300,350 };//账号，密码位置
	RECT rect5 = { 250,190,420,230 }, rect6 = { 250,290,420,330 };//账号输入框，密码输入框
	RECT rect7 = { 255,192,420,230 }, rect8 = { 255,292,420,330 };//账号输入位置，密码输入位置
	HWND hwndone = initgraph(HEIGHTONE, WIDTHONE);
	IMAGE bk;
	loadimage(&bk, "./images/7.jpg",HEIGHTONE,WIDTHONE);//7可以，
	SetWindowText(hwndone, "                                                             超市商品管理系统登录页面");//置窗口标题
	MOUSEMSG F;
	LOGFONT longfont;//字体类型
	gettextstyle(&longfont);
	while (1)
	{
		if (P!=1)
		{
			setbkmode(TRANSPARENT);
			putimage(0, 0, &bk);
			settextstyle(&longfont);
			BeginBatchDraw();//批量绘图开始
			setlinecolor(WHITE);
			drawtext("登录", &rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext("退出", &rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext("账号：", &rect3, NULL);
			drawtext("密码：", &rect4, NULL);
			if (p == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
			}
			if (p1 == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
			}
			if (p2 == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect5.left, rect5.top, rect5.right, rect5.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect5.left, rect5.top, rect5.right, rect5.bottom);
			}
			if (p3 == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect6.left, rect6.top, rect6.right, rect6.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect6.left, rect6.top, rect6.right, rect6.bottom);
			}

			MOUSEMSG F;
			if (MouseHit())//检测是否有鼠标消息
			{
				F = GetMouseMsg();//得到一条鼠标消息
				switch (F.uMsg)
				{
				case WM_MOUSEMOVE://如果鼠标消息时“鼠标移动”
				{
					if (MouseFuncOne(rect1, F) == 1 && p == 0)
						p = 1;
					else
						if (MouseFuncOne(rect1, F) == 0)
							p = 0;
					if (MouseFuncOne(rect2, F) == 1 && p == 0)
						p1 = 1;
					else
						if (MouseFuncOne(rect2, F) == 0)
							p1 = 0;
					if (MouseFuncOne(rect5, F) == 1 && p == 0)
						p2 = 1;
					else
						if (MouseFuncOne(rect5, F) == 0)
							p2 = 0;
					if (MouseFuncOne(rect6, F) == 1 && p == 0)
						p3 = 1;
					else
						if (MouseFuncOne(rect6, F) == 0)
							p3 = 0;

				}break;
				case WM_LBUTTONDOWN:
				{
					if (MouseFuncOne(rect6, F) == 1)
						wwwww = 1;
					else
						if (MouseFuncOne(rect1, F) == 1)
							goto LL;
					if (MouseFuncOne(rect2, F) == 1)
						exit(0);

				}; break;
				}
			}
			if (_kbhit())
			{
				settextcolor(WHITE);
				if (wwwww == '\0')
				{
					pp = _getch();
					if (pp == 13)
						wwwww = 1;
					else
						if (pp == VK_BACK)
						{
							if (name.length() != 0)
								name.erase(name.length() - 1, 1);
						}
						else
							name += pp;
				}
				if (wwwww == 1)
				{
					pp = _getch();
					if (pp == VK_BACK)
					{
						if (passwd.length() != 0)
						{
							passwd.erase(passwd.length() - 1, 1);
							passwdx.erase(passwdx.length() - 1, 1);
						}
					}
					else
					{
						passwd += pp;
						passwdx += '*';
					}
				}
				if (wwwww == 1 && pp == 13)
					LL:
				{
					struct User* usernow = users;
					while (usernow)
					{
						if (!strcmp(name.c_str(), usernow->name))
						{
							if (!strcmp(passwd.c_str(), usernow->passwd))
							{
								char linshi[10];
								www = 1;
								P = 1;
								if (usernow->weight == 0)
									weightNow = 0;
								else
									weightNow = 1;
								sprintf(linshi, "%s", usernow->name);
								NAME = linshi;
								break;
							}
							else
							{
							LLL:
								{
									HWND x = GetHWnd();
									MessageBox(x, "账号或密码错误！", "错误", MB_OK);
									www = 1;
									break;
								}
								
							}
						}
						usernow = usernow->next;
					}
					if (www == '\0')//这里判断方式是，先在库里寻找用户名，若找到则判断密码；这一句的用处是应对  库里没有找到用户名时
						goto LLL;
				};
			}
			settextstyle(30, 2, NULL);//设置文本大小
			if (wwwww == '\0')
				drawtext(name.c_str(), &rect7, DT_VCENTER | DT_SINGLELINE);//上下居中
			else
			{
				drawtext(name.c_str(), &rect7, DT_VCENTER | DT_SINGLELINE);
				drawtext(passwdx.c_str(), &rect8, DT_VCENTER | DT_SINGLELINE);
			}
			DrawStars();//“星空”初始化
			FlushBatchDraw();//结束批量绘图
		//	cleardevice();//清屏
		}
		else break;
	}
	closegraph();
	
	WindowTwo();
}
int MouseFuncOne(RECT &rect, MOUSEMSG &mouse)//检测鼠标当前位置是否在指定区域
{
	if (mouse.x > rect.left && mouse.x<rect.right && mouse.y>rect.top && mouse.y < rect.bottom)
		return 1;
	else
		return 0;
}
void MouseFuncTwo(RECT& rect)
{
	rectangle(rect.left, rect.top, rect.right, rect.bottom);
}
void DrawStars()
{
	static	struct Star
	{
		int x;
		int y;
		int step;
		COLOR color;
	}stars[2000];
	static char p = 0;
	
	if (p == 0)
	{
		for (int i = 0; i < 1000; i++)
		{
				stars[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
				stars[i].x = rand() % 1080;
				stars[i].y = rand() % 640;
				stars[i].step =1+rand() % 5;
		}
		p = 1;
	}
	for (int i = 0; i < 1500; i++)
	{
		setfillcolor(stars[i].color);
		solidcircle(stars[i].x, stars[i].y,1);
		stars[i].y -= stars[i].step;
		if (stars[i].y < 0)
		{
			stars[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
			stars[i].x = rand() % 1080;
			stars[i].y = rand() % 640;
			stars[i].step = 1+rand()%2;
		}
	}
}
void WindowTwo()
{
	time_t timep;//|||||||||||||||||||||||||||||||||||||||||||||||||||||||当前时间
	struct tm* pp;
	string Right;
	char Time[50];
	
	int P = 0;//是否总循环
	if (weightNow == 1)
		Right += "当前身份：管理员";
	else
		Right += "当前身份：普通用户";
	int p = 0, p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0, p6 = 0, p7 = 0, p8 = 0;//工具变量
	graphdefaults();//复位绘图设置
	HWND hwdn=initgraph(HEIGHTTWO, WIDTHTWO);
	RECT rect1 = { 200,140,450,195 }, rect2 = { 500,140,750,195 };//“添加用户” “管理用户”的位置
	RECT rect3 = { 200,245,450,300 }, rect4 = { 500,245,750,300 };//“销售功能” “商品管理”的位置
	RECT rect5 = { 200,350,450,405 }, rect6 = { 500,350,750,405 };//“商品仓库” “本地保存”的位置
	RECT rect7 = { 200,455,450,510 }, rect8 = { 500,455,750,510 };//“本地导入” “每日一汤”的位置
	RECT rect9 = { 200,50,750,130 },  rect10 = { 200,510,750,610 };//“当前身份”“时间显示”的位置
	RECT rect11 = { 850,550,920,600 };//返回
	IMAGE bk;
	loadimage(&bk, "images/7.jpg", HEIGHTTWO, WIDTHTWO);
	SetWindowText(hwdn, "超市管理系统/操作界面");
	settextstyle(30, 0, _T("华文楷体"));
	MOUSEMSG F;
	while (1)
	{
		if (P == 0)
		{
			time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
			pp = localtime(&timep);//用localtime将秒数转化为struct tm结构体
			sprintf(Time, "%d / %d / %d日 %02d : %02d : %02d", 1900 + pp->tm_year, 1 + pp->tm_mon, pp->tm_mday, pp->tm_hour, pp->tm_min, pp->tm_sec);
			setbkmode(TRANSPARENT);
			BeginBatchDraw();
			putimage(0, 0, &bk);
			char www[10];
			drawtext("★添加用户★", &rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext("★密码管理★", &rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext("★销售功能★", &rect3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext("★商品管理★", &rect4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext("★商品仓库★", &rect5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext("★本地保存★", &rect6, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext("★本地导入★", &rect7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext("★每日一汤★", &rect8, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext("返回", &rect11, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext(Right.c_str(), &rect9, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext(Time, &rect10, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (p == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
			}
			if (p1 == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
			}
			if (p2 == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect3.left, rect3.top, rect3.right, rect3.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect3.left, rect3.top, rect3.right, rect3.bottom);
			}
			if (p3 == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect4.left, rect4.top, rect4.right, rect4.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect4.left, rect4.top, rect4.right, rect4.bottom);
			}
			if (p4 == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect5.left, rect5.top, rect5.right, rect5.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect5.left, rect5.top, rect5.right, rect5.bottom);
			}
			if (p5 == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect6.left, rect6.top, rect6.right, rect6.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect6.left, rect6.top, rect6.right, rect6.bottom);
			}
			if (p6 == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect7.left, rect7.top, rect7.right, rect7.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect7.left, rect7.top, rect7.right, rect7.bottom);
			}
			if (p7 == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect8.left, rect8.top, rect8.right, rect8.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect8.left, rect8.top, rect8.right, rect8.bottom);
			}
			if (p8 == 0)
			{
				setlinecolor(WHITE);
				rectangle(rect11.left, rect11.top, rect11.right, rect11.bottom);
			}
			else
			{
				setlinecolor(RED);
				rectangle(rect11.left, rect11.top, rect11.right, rect11.bottom);
			}
			if (MouseHit())
			{
				F = GetMouseMsg();
				switch (F.uMsg)
				{
				case WM_MOUSEMOVE:
				{
					if (MouseFuncOne(rect1, F) == 1 && p == 0)
						p = 1;
					else
						if (MouseFuncOne(rect1, F) == 0)
							p = 0;
					if (MouseFuncOne(rect2, F) == 1 && p == 0)
						p1 = 1;
					else
						if (MouseFuncOne(rect2, F) == 0)
							p1 = 0;
					if (MouseFuncOne(rect3, F) == 1 && p == 0)
						p2 = 1;
					else
						if (MouseFuncOne(rect3, F) == 0)
							p2 = 0;
					if (MouseFuncOne(rect4, F) == 1 && p == 0)
						p3 = 1;
					else
						if (MouseFuncOne(rect4, F) == 0)
							p3 = 0;
					if (MouseFuncOne(rect5, F) == 1 && p == 0)
						p4 = 1;
					else
						if (MouseFuncOne(rect5, F) == 0)
							p4 = 0;
					if (MouseFuncOne(rect6, F) == 1 && p == 0)
						p5 = 1;
					else
						if (MouseFuncOne(rect6, F) == 0)
							p5 = 0;
					if (MouseFuncOne(rect7, F) == 1 && p == 0)
						p6 = 1;
					else
						if (MouseFuncOne(rect7, F) == 0)
							p6 = 0;
					if (MouseFuncOne(rect8, F) == 1 && p == 0)
						p7 = 1;
					else
						if (MouseFuncOne(rect8, F) == 0)
							p7 = 0;
					if (MouseFuncOne(rect11, F) == 1 && p == 0)
						p8 = 1;
					else
						if (MouseFuncOne(rect11, F) == 0)
							p8 = 0;

				}; break;
				case WM_LBUTTONDOWN:
				{
					if (MouseFuncOne(rect1, F) == 1)
					{
						if (weightNow != 1)
							MessageBox(hwdn, "您不是管理员，无权限操作", NULL, MB_OK);
						else
							WindowThree();
					}
					if (MouseFuncOne(rect2, F) == 1)
						WindowFour();
					if (MouseFuncOne(rect3,F)==1)
						WindowFive();
					if (MouseFuncOne(rect4, F) == 1)
						WindowSix();
					if (MouseFuncOne(rect5, F) == 1)//商品统计
					{
						if (weightNow != 1)
							MessageBox(hwdn, "您不是管理员，无权限操作", NULL, MB_OK);
						else
						WindowSeven();
					}
					if (MouseFuncOne(rect11, F) == 1)
						WindowOne();
					if (MouseFuncOne(rect7, F) == 1)
						if (weightNow != 1)
							MessageBox(hwdn, "您不是管理员，无权限操作", NULL, MB_OK);
						else
						{
							if (ReadStore())
								MessageBox(hwdn, "读取成功！", "成功", MB_OK);
							else
								MessageBox(hwdn, "读取失败！", "失败", MB_OK);
						}
					if(MouseFuncOne(rect8,F)==1)
						MessageBox(hwdn, "加油哦！", "每日一汤", MB_OK);
					if (MouseFuncOne(rect6, F) == 1)
						if (weightNow != 1)
							MessageBox(hwdn, "您不是管理员，无权限操作", NULL, MB_OK);
						else
						{
							if (StoreMessage())
								MessageBox(hwdn, "保存成功！", "成功", MB_OK);
							else MessageBox(hwdn, "保存失败！", "失败", MB_OK);
						}
				}; break;
				}//switch到此结束
			}
			//DrawStars();
			FlushBatchDraw();
			
		}
		else
		break;
	}
}
void WindowThree()
{
	char p='\0', p1='\0', p2='\0', p3='\0', p4='\0',w='\0',ww='\0',www='\0',wwww='\0';//工具变量
	string name, passwd, passwdx, repasswd,repasswdx;//用户名，密码，实际显示的密码，确认的密码，实际确认显示的密码
	graphdefaults();//复位绘图设置
	HWND hwnd = initgraph(HEIGHTONE, WIDTHONE);
	IMAGE bk;
	SetWindowText(hwnd, "超市商品管理系统/管理员/添加用户");
	loadimage(&bk, "images/7.jpg", HEIGHTONE, WIDTHONE);
	RECT rect1 = { 250,120,430,165 }, rect2 = { 250,200,430,245 }, rect6 = {250,280,430,325};//“添加用户的账号输入框” “添加用户的密码输入框”“确认密码输入框”
	RECT rect3 = { 190,130,300,180 }, rect4 = { 190,210,300,260 }, rect5 = { 148,290,300,330 };//“账号”“密码”“确认密码”
	RECT rect7 = { 150,370,300,420 }, rect8 = { 350,370,500,420 };//“确认”“取消”
	RECT rect9 = { 252,120,430,165 }, rect10 = { 252,200,430,245 }, rect11 = { 252,280,430,325 };//“账号显示”“密码显示”“确认密码显示”
	MOUSEMSG F;
	while (1)
	{
		BeginBatchDraw();
		setbkmode(TRANSPARENT);
		settextstyle(25, 0, _T("华文楷体"));
		putimage(0, 0, &bk);
		drawtext("账号：", &rect3,NULL);
		drawtext("密码：", &rect4,NULL);
		drawtext("确认密码：", &rect5,NULL);
		drawtext("确定", &rect7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("取消", &rect8, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(name.c_str(), &rect9, DT_VCENTER | DT_SINGLELINE);
		drawtext(passwdx.c_str(), &rect10, DT_VCENTER | DT_SINGLELINE);
		drawtext(repasswdx.c_str(), &rect11, DT_VCENTER | DT_SINGLELINE);
		if (p == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
		}
		if (p1 == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
		}
		if (p2 == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect6.left, rect6.top, rect6.right, rect6.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect6.left, rect6.top, rect6.right, rect6.bottom);
		}
		if (p3 == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect7.left, rect7.top, rect7.right, rect7.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect7.left, rect7.top, rect7.right, rect7.bottom);
		}
		if (p4 == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect8.left, rect8.top, rect8.right, rect8.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect8.left, rect8.top, rect8.right, rect8.bottom);
		}
		if (MouseHit())
		{
			F = GetMouseMsg();
			switch (F.uMsg)
			{
			case WM_MOUSEMOVE:
			{
				if (MouseFuncOne(rect1, F) == 1 && p == 0)
					p = 1;
				else
					if (MouseFuncOne(rect1, F) == 0)
						p = 0;
				if (MouseFuncOne(rect2, F) == 1 && p == 0)
					p1 = 1;
				else
					if (MouseFuncOne(rect2, F) == 0)
						p1 = 0;
				if (MouseFuncOne(rect6, F) == 1 && p == 0)
					p2 = 1;
				else
					if (MouseFuncOne(rect6, F) == 0)
						p2 = 0;
				if (MouseFuncOne(rect7, F) == 1 && p == 0)
					p3 = 1;
				else
					if (MouseFuncOne(rect7, F) == 0)
						p3 = 0;
				if (MouseFuncOne(rect8, F) == 1 && p == 0)
					p4 = 1;
				else
					if (MouseFuncOne(rect8, F) == 0)
						p4 = 0;
			}; break;
			case WM_LBUTTONDOWN:
			{
				if (MouseFuncOne(rect8, F) == 1)//按到取消
				WindowTwo();
				if (MouseFuncOne(rect7, F) == 1)
				{
					int ls = UserCreat(name.c_str(),passwd.c_str(),repasswd.c_str());
					if (ls == 2)//密码与确认密码不符
					{
						MessageBox(hwnd, "密码与确认密码不符", NULL, MB_OK);
					}
					else
						if (ls == 0)//账户名已存在
						{
							MessageBox(hwnd, "账户已经存在，无法再次添加", NULL, MB_OK);
						}
						else//添加成功
							MessageBox(hwnd, "添加成功", "成功", MB_OK);

				};
				if (MouseFuncOne(rect1, F) == 1)
				{
					ww = 1;
					www = 0;
					wwww = 0;
				}
				if (MouseFuncOne(rect2, F) == 1)
				{
					ww = 0;
					www = 1;
					wwww = 0;
				}
				if (MouseFuncOne(rect6, F) == 1)
				{
					ww = 0;
					www = 0;
					wwww = 1;
				}
			}
			}//switch到此结束
		}
		if (_kbhit())
		{
			w = _getch();
			if (ww == 1)
			{
				if (w != VK_BACK)
					name += w;
				else
					name.erase(name.length() - 1, 1);
			}
			if (www == 1)
			{
				if (w != VK_BACK)
				{
					passwd += w;
					passwdx += '*';
				}
				else
				{
					passwd.erase(name.length() - 1, 1);
					passwdx.erase(name.length() - 1, 1);
				}
			}
			if (wwww == 1)
			{
				if (w != VK_BACK)
				{
					repasswd += w;
					repasswdx += '*';
				}
				else
				{
					repasswd.erase(name.length() - 1, 1);
					repasswdx.erase(name.length() - 1, 1);
				}
			}
		}
		//DrawStars();
		FlushBatchDraw();
		cleardevice();
		
	}
	
}
void WindowFour()
{
	HWND hwnd = initgraph(HEIGHTONE, WIDTHONE);
	SetWindowText(hwnd, "超市商品管理系统/操作界面/用户管理");
	IMAGE image;
	char p = '\0', p1 = '\0', p2 = '\0', w = '\0', ww = '\0', www = '\0', wwww = '\0';//工具变量
	string name,namex;
	loadimage(&image, "images/7.jpg", HEIGHTONE, WIDTHONE);
	RECT rect1 = { 250,220,430,270 };//“账号输入框” 
	RECT rect2;
	RECT rect3 = { 150,370,300,420 }, rect4 = { 350,370,500,420 };//“重置”“取消”
	RECT rect5 = { 252,220,430,270 };
	setbkmode(TRANSPARENT);
	settextstyle(25, 0, _T("华文楷体"));
	MOUSEMSG F;
	string A, B;
	if (weightNow == 1)
	{
		A = "账号:";
		rect2 = { 190,235,300,275 };//“账号”
		B = "重置";
	}
	else
	{
		A = "新密码:";
		rect2 = { 170,235,300,275 };//“账号”
		B = "确认";
	}
	while (1)
	{
		BeginBatchDraw();
		putimage(0, 0, &image);
		drawtext(A.c_str(), &rect2, NULL);
		drawtext(B.c_str(), &rect3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("取消", &rect4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		if(weightNow==1)
		drawtext(name.c_str(), &rect5, DT_VCENTER | DT_SINGLELINE);
		else
			drawtext(namex.c_str(), &rect5, DT_VCENTER | DT_SINGLELINE);//显示*号
		if (p == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
		}
		if (p1 == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect3.left, rect3.top, rect3.right, rect3.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect3.left, rect3.top, rect3.right, rect3.bottom);
		}
		if (p2 == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect4.left, rect4.top, rect4.right, rect4.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect4.left, rect4.top, rect4.right, rect4.bottom);
		}
		if (MouseHit())
		{
			F = GetMouseMsg();
			switch (F.uMsg)
			{
			case WM_MOUSEMOVE:
			{
				if (MouseFuncOne(rect1, F) == 1 && p == 0)
					p = 1;
				else
					if (MouseFuncOne(rect1, F) == 0)
						p = 0;
				if (MouseFuncOne(rect3, F) == 1 && p == 0)
					p1 = 1;
				else
					if (MouseFuncOne(rect3, F) == 0)
						p1 = 0;
				if (MouseFuncOne(rect4, F) == 1 && p == 0)
					p2 = 1;
				else
					if (MouseFuncOne(rect4, F) == 0)
						p2 = 0;
			}; break;
			case WM_LBUTTONDOWN:
			{
				if (MouseFuncOne(rect3, F) == 1)
				{
					if (UserChangePasswd(name.c_str()))
					{
						if (weightNow == 1)
							MessageBox(hwnd, "重置成功", "成功", MB_OK);
						else
							MessageBox(hwnd, "修改成功","成功", MB_OK);
					}

					else
						MessageBox(hwnd, "用户名不存在", NULL, MB_OK);
				}
				if (MouseFuncOne(rect4, F) == 1)
					WindowTwo();
				if (MouseFuncOne(rect1, F) == 1)
					ww = 1;
			}; break;
			}//switch到此结束

		}
		FlushBatchDraw();
		if (_kbhit())
		{
			w = _getch();
			if (ww == 1)
			{
				if (w != VK_BACK)
				{
					name += w;
					namex += '*';
				}
				else
					if (name.length() != 0)
					{
						name.erase(name.length() - 1, 1);
						namex.erase(namex.length() - 1, 1);
					}
			}
		}

	}
}
void WindowFive()
{
	HWND hwnd = initgraph(HEIGHTONE, WIDTHONE);
	string _class, name, sum;
	char p = '\0', p1 = '\0', p2 = '\0', p3 = '\0', p4 = '\0',p5='\0',p6='\0', w = '\0', ww = '\0', www = '\0', wwww = '\0',wwwww='\0';//工具变量
	SetWindowText(hwnd, "超市商品管理系统/操作界面/销售功能");
	setbkmode(TRANSPARENT);
	IMAGE image;
	loadimage(&image, "images/7.jpg", HEIGHTONE, WIDTHONE);
	settextstyle(25, 0, _T("宋体"));
	MOUSEMSG F;
	RECT rect1 = { 250,160,430,205 }, rect2 = { 250,240,430,285 }, rect6 = { 250,320,430,365 }, rect12 = {250,400,430,445};//“添加用户的账号输入框” “添加用户的密码输入框”“确认密码输入框”
	RECT rect3 = { 150,170,250,220 }, rect4 = { 150,250,250,300 }, rect5 = { 150,330,250,380 }, rect13 = {150,410,250,450};//“账号”“密码”“确认密码”
	RECT rect7 = { 220,480,370,530 }, rect8 = { 410,480,560,530 }, rect19 = {50,480,180,530};//“确认”“取消”“查询该商品信息”
	RECT rect14 = { 100,5,600,200 };//商店菜单
	RECT rect15 = { 252,160,430,205 }, rect16 = { 252,240,430,285 }, rect17 = { 252,320,430,365 }, rect18 = { 252,400,430,445 };//四个框输入
	string menu = "本超市在售：\n    shipin:yurou、yangrou、baicai、luobo\n    huazhaungpin：konghong、yanzhi、xiangshui\n    riyongpin：chabei、yusan、kuaizi\n    yinliao：hongcha、lvccha、kele";
	int kucun = -1,suoxu;
	float danjia = -1,result;
	char linshi[40]="\0";
	char money[20] = "\0";
	int*jian;
	while (1)
	{
		BeginBatchDraw();
		putimage(0, 0, &image);
		drawtext("商品类别：", &rect3, NULL);
		drawtext("商品名称：", &rect4, NULL);
		drawtext("需求个数：", &rect5, NULL);
		drawtext("所需金额：", &rect13, NULL);
		drawtext("查询", &rect19, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("购买", &rect7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("取消", &rect8, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_class.c_str(), &rect15, DT_VCENTER | DT_SINGLELINE);
		drawtext(name.c_str(), &rect16, DT_VCENTER | DT_SINGLELINE);
		drawtext(sum.c_str(), &rect17, DT_VCENTER | DT_SINGLELINE);
		if(money[0]!='\0')
		drawtext(money, &rect18, DT_VCENTER | DT_SINGLELINE);
		settextstyle(25, 0, _T("宋体"));
		drawtext(menu.c_str(), &rect14, NULL);
		settextstyle(20, 0, _T("宋体"));
		if (p == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
		}
		if (p1 == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
		}
		if (p2 == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect6.left, rect6.top, rect6.right, rect6.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect6.left, rect6.top, rect6.right, rect6.bottom);
		}
		if (p3 == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect12.left, rect12.top, rect12.right, rect12.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect12.left, rect12.top, rect12.right, rect12.bottom);
		}
		if (p4 == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect7.left, rect7.top, rect7.right, rect7.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect7.left, rect7.top, rect7.right, rect7.bottom);
		}
		if (p5 == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect8.left, rect8.top, rect8.right, rect8.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect8.left, rect8.top, rect8.right, rect8.bottom);
		}
		if (p6 == 0)
		{
			setlinecolor(WHITE);
			rectangle(rect19.left, rect19.top, rect19.right, rect19.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect19.left, rect19.top, rect19.right, rect19.bottom);
		}
		if (MouseHit())
		{
			F = GetMouseMsg();
			switch (F.uMsg)
			{
			case WM_MOUSEMOVE:
			{
				if (MouseFuncOne(rect1, F) == 1 && p == 0)
					p = 1;
				else
					if (MouseFuncOne(rect1, F) == 0)
						p = 0;
				if (MouseFuncOne(rect2, F) == 1 && p == 0)
					p1 = 1;
				else
					if (MouseFuncOne(rect2, F) == 0)
						p1 = 0;
				if (MouseFuncOne(rect6, F) == 1 && p == 0)
					p2 = 1;
				else
					if (MouseFuncOne(rect6, F) == 0)
						p2 = 0;
				if (MouseFuncOne(rect12, F) == 1 && p == 0)
					p3 = 1;
				else
					if (MouseFuncOne(rect12, F) == 0)
						p3 = 0;
				if (MouseFuncOne(rect7, F) == 1 && p == 0)
					p4 = 1;
				else
					if (MouseFuncOne(rect7, F) == 0)
						p4 = 0;
				if (MouseFuncOne(rect8, F) == 1 && p == 0)
					p5 = 1;
				else
					if (MouseFuncOne(rect8, F) == 0)
						p5 = 0;
				if (MouseFuncOne(rect19, F) == 1 && p == 0)
					p6 = 1;
				else
					if (MouseFuncOne(rect19, F) == 0)
						p6 = 0;
			}; break;
			case WM_LBUTTONDOWN:
			{
				if (MouseFuncOne(rect8, F) == 1)//按到 取消
					WindowTwo();
				if (MouseFuncOne(rect7, F) == 1)//按到 购买
				{
					p= GoodsSell(_class.c_str(), name.c_str(), sum.c_str(),kucun,danjia,1);
					if(p==-1)
					MessageBox(hwnd, "非法输入！", NULL, MB_OK);
					if(p==0)
					MessageBox(hwnd, "本店不存在您所输入的商品类名！", NULL, MB_OK);
					if(p==1)
					MessageBox(hwnd, "您输入的商品名称有误！", NULL, MB_OK);
					if(p==2)
					MessageBox(hwnd, "您所需要的商品没有库存了！", NULL, MB_OK);
					if(p==3)
					MessageBox(hwnd, "您对该商品需求量已经超出该商品的库存！", NULL, MB_OK);
					if (p == 4)
					{
						//sscanf(sum.c_str(), "%d", &suoxu);
						//result = danjia * suoxu;
						//sprintf(money, "%s", result);
						MessageBox(hwnd, "购买成功！", "成功", MB_OK);
						money[0] = '\0';
					}
				}
				if (MouseFuncOne(rect19, F) == 1)//按到 查询
				{
					p = GoodsSell(_class.c_str(), name.c_str(), sum.c_str(), kucun, danjia,0);
					if (p == -1)
					{
						MessageBox(hwnd, "非法输入！", NULL, MB_OK);
						money[0] = '\0';
					}
					if (p == 0)
					{
						MessageBox(hwnd, "本店不存在您所输入的商品类名！", NULL, MB_OK);
						money[0] = '\0';
					}
					if (p == 1)
					{
						MessageBox(hwnd, "您输入的商品名称有误！", NULL, MB_OK);
						money[0] = '\0';
					}
					if (p == 2||p==3||p==4)
					{
						sprintf(linshi, "库存有：%d个；售价：%.2f元/单位", kucun, danjia);
						if (p == 4)
						{
							sscanf(sum.c_str(), "%d", &suoxu);
							result = danjia * suoxu;
							sprintf(money, "%.2f", result);
						}
						MessageBox(hwnd, linshi, "成功", MB_OK);
						fill(linshi, linshi + 25, 0);
					}
				}
				if (MouseFuncOne(rect1, F) == 1)
				{
					ww = 1;
					www = 0;
					wwww = 0;
				}
				if (MouseFuncOne(rect2, F) == 1)
				{
					ww = 0;
					www = 1;
					wwww = 0;
				}
				if (MouseFuncOne(rect6, F) == 1)
				{
					ww = 0;
					www = 0;
					wwww = 1;
				}
			
			}; break;
			}//switch到此结束
			
		}
		if (_kbhit())
		{
			w = _getch();
			if (ww == 1)
			{
				if (w != VK_BACK)
					_class += w;
				else
					_class.erase(_class.length() - 1, 1);
			}
			if (www == 1)
			{
				if (w != VK_BACK)
					name += w;
				else
					name.erase(name.length() - 1, 1);
			}
			if (wwww == 1)
			{
				if (w != VK_BACK)
					sum += w;
				else
					sum.erase(sum.length() - 1, 1);
			}
		}
		FlushBatchDraw();
	}

}
void WindowSix()
{
	HWND hwnd = initgraph(900, 390);
	char p = '\0', p1 = '\0', p2 = '\0',p3='\0',p4='\0',p5='\0',w[50]="\0",ww='\0',www='\0';
	string str[49],import;
	SetWindowText(hwnd, "超市商品管理系统/操作界面/商品管理");
	IMAGE image;
	loadimage(&image, "images/7.jpg", 900, 390);
	setbkmode(TRANSPARENT);
	MOUSEMSG F;
	settextstyle(20, 0, _T("宋体"));
	RECT rect1 = { 30,150,150,190 };//查找输入框
	RECT rect2 = { 30,220,150,250 }, rect3 = { 30,275,150,305 }, rect4 = { 30,330,150,360 }, rect5 = {30,40,150,90};
	//?“查找”“保存”“返回”“功能介绍”
	RECT rect6 = { 30,120,150,150 };//由下框输入
	RECT rect7 = { 170,40,870,360 };//信息框
	RECT rect8 = { 170,40,270,80 }, rect9 = { 270,40,370,80 }, rect10 = { 370,40,470,80 }, rect11 = { 470,40,570,80 }, rect12 = { 570,40,670,80 }, rect13 = { 670,40,770,80 }, rect14 = { 770,40,870,80 };
	//?表第一行
	LPRECT rect = (LPRECT)calloc(sizeof(RECT),49);

	char b='\0', c='\0';
	while (b!=7)//rect中每6个代表一行，与每六个str照应
	{
		for (char i = 0; i < 7; i++)
		{
			rect[c] = { 170 + i * 100,80 + 40 * b,170 + (i + 1) * 100,120 + 40 * b };
			c+=1;
		}
		b += 1;

	}
	settextcolor(LIGHTGRAY);
	while (1)
	{
		BeginBatchDraw();
		putimage(0, 0, &image);
		drawtext("由下框输入：",&rect6, DT_VCENTER | DT_SINGLELINE);
		drawtext("查找", &rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("保存",&rect3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("返回", &rect4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("功能介绍", &rect5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("商品名", &rect8, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("库存", &rect9, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("品牌", &rect10, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("单价", &rect11, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("产家", &rect12, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("类别", &rect13, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("生产日期", &rect14, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		for (char i = 0; i < 49;i++)
			drawtext(str[i].c_str(), &rect[i], DT_CENTER|DT_VCENTER | DT_SINGLELINE);
		drawtext(import.c_str(), &rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		setlinecolor(LIGHTGRAY);
		rectangle(rect7.left, rect7.top, rect7.right, rect7.bottom);//信息框界面
		
		rectangle(rect7.left + 100, rect7.top, rect7.left + 200, rect7.bottom);
		rectangle(rect7.left + 200, rect7.top, rect7.left + 300, rect7.bottom);
		rectangle(rect7.left + 300, rect7.top, rect7.left + 400, rect7.bottom);
		rectangle(rect7.left + 400, rect7.top, rect7.left + 500, rect7.bottom);
		rectangle(rect7.left + 500, rect7.top, rect7.left + 600, rect7.bottom);
		rectangle(rect7.left + 600, rect7.top, rect7.left + 700, rect7.bottom);
		//↑表格竖线
		//↓表格横线
		rectangle(rect7.left, rect7.top + 40, rect7.right, rect7.top + 80);
		rectangle(rect7.left, rect7.top + 120, rect7.right, rect7.top + 160);
		rectangle(rect7.left, rect7.top + 200, rect7.right, rect7.top + 240);
		rectangle(rect7.left, rect7.top + 280, rect7.right, rect7.top + 320);
		rectangle(rect7.left, rect7.top + 360, rect7.right, rect7.top + 400);
		rectangle(rect7.left, rect7.top + 400, rect7.right, rect7.top + 440);

		if (p == 0)
		{
			setlinecolor(LIGHTGRAY);
			rectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect1.left, rect1.top, rect1.right, rect1.bottom);
		}
		if (p1 == 0)
		{
			setlinecolor(LIGHTGRAY);
			rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
		}
		if (p2 == 0)
		{
			setlinecolor(LIGHTGRAY);
			rectangle(rect3.left, rect3.top, rect3.right, rect3.bottom);
		}
		else
		{
		
			setlinecolor(RED);
			rectangle(rect3.left, rect3.top, rect3.right, rect3.bottom);
		}
		if (p3 == 0)
		{
			setlinecolor(LIGHTGRAY);
			rectangle(rect4.left, rect4.top, rect4.right, rect4.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect4.left, rect4.top, rect4.right, rect4.bottom);
		}
		if (p4 == 0)
		{
			setlinecolor(LIGHTGRAY);
			rectangle(rect5.left, rect5.top, rect5.right, rect5.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect5.left, rect5.top, rect5.right, rect5.bottom);
		}
		if (MouseHit())
		{
			F = GetMouseMsg();
			switch (F.uMsg)
			{
			case WM_MOUSEMOVE:
			{
				if (MouseFuncOne(rect1, F) == 1 && p == 0)
					p = 1;
				else
					if (MouseFuncOne(rect1, F) == 0)
						p = 0;
				if (MouseFuncOne(rect2, F) == 1 && p1 == 0)
					p1 = 1;
				else
					if (MouseFuncOne(rect2, F) == 0)
						p1 = 0;
				if (MouseFuncOne(rect3, F) == 1 && p2 == 0)
					p2 = 1;
				else
					if (MouseFuncOne(rect3, F) == 0)
						p2 = 0;
				if (MouseFuncOne(rect4, F) == 1 && p3 == 0)
					p3 = 1;
				else
					if (MouseFuncOne(rect4, F) == 0)
						p3 = 0;
				if (MouseFuncOne(rect5, F) == 1 && p4 == 0)
					p4 = 1;
				else
					if (MouseFuncOne(rect5, F) == 0)
						p4 = 0;
				
			}; break;
			case WM_LBUTTONDOWN:
			{
				if (MouseFuncOne(rect5, F) == 1)//点击功能介绍
				MessageBox(hwnd, "功能介绍：\n  在下方输入框里输入您需要查找或修改的内容，然后你可以在右边表格进行信息修改，然后保存即可完成修改。\n  1.输入的内容可以为：\n  商品名称、商品类别名、产家名其中的任一个；\n  当输入商品名称时，表格会显示使用该名称的所有商品及它们对应的信息；\n  当输入商品类别名时，会显示该类别所有商品及对应信息；\n  当输入产家名时，会显示所有由该产家生成的商品及信息；\n  2.删除：将商品所对应的类名清空并保存即可删除该商品。\n  3.添加：在空行中输入信息并保存即可完成添加。\n  注意:由于设计要求中超市只有4类，故在对于类名的修改只能是清空或者修改为别的类名。\n  祝您使用愉快！", "功能介绍", MB_OK);
				else
					if (MouseFuncOne(rect1, F) == 1)
					{
						www = 1;
						fill(w, &w[49], 0);
					}
					else
						if (MouseFuncOne(rect4, F) == 1)//点击返回
							WindowTwo();
						else
							if (MouseFuncOne(rect2, F) == 1)//点击查询
							{
								for (char i = 0; i < 42; i++)
									str[i] = "";
								b=FindData(str,import);
								if(b==0)
									MessageBox(hwnd, "输入有误！", NULL, MB_OK);
							}
							else
								if (MouseFuncOne(rect3, F) == 1)//点到保存
								{
									if (weightNow != 1)
										MessageBox(hwnd, "您不是管理员，无权限操作", NULL, MB_OK);
									else
									{
										int pp = FillData(str);
										if (pp == 0)
											MessageBox(hwnd, "类名有误！", NULL, MB_OK);
										else
											MessageBox(hwnd, "保存成功！", "成功", MB_OK);
									}
								}
								else
									for (char i = 0; i < 42; i++)
									{
										if (MouseFuncOne(rect[i], F) == 1)
										{
											w[i] = 1;
											fill(&w[0], &w[i], 0);
											fill(&w[i] + 1, &w[49], 0);
											www = 0;
										}
									}
			}; break;

			}
		}
		FlushBatchDraw();
		if (_kbhit())
		{
			ww = _getch();
			if (www == 1)
			{
				if (ww != VK_BACK)
					import += ww;
				else
					if (import.length() != 0)
					import.erase(import.length() - 1, 1);
			}
			for (char i = 0; i < 44; i++)
			{
				if (w[i] == 1)
				{
					if (ww != VK_BACK)
						str[i] += ww;
					else
						if(str[i].length()!=0)
						str[i].erase(str[i].length() - 1, 1);
				}
			}
		}
	}
}
void WindowSeven()//
{
	HWND hwnd = initgraph(900, 390);
	char p = '\0', p1 = '\0', p2 = '\0', p3 = '\0', p4 = '\0', p5 = '\0',p6='\0',p7 ='\0',w[50] = "\0", ww = '\0', www = '\0';
	string str[98], import;
	SetWindowText(hwnd, "超市商品管理系统/操作界面/商品统计");
	IMAGE image;
	loadimage(&image, "images/7.jpg", 900, 390);
	setbkmode(TRANSPARENT);
	MOUSEMSG F;
	settextstyle(20, 0, _T("宋体"));
	RECT rect1 = { 30,150,150,190 };//查找输入框
	RECT rect5 = { 20,10,150,50 };//"总库存量"
	RECT rect2 = { 30,220,150,250 }, rect3 = { 30,275,150,305 }, rect4 = { 30,330,150,360 }, rect14 = { 30,165,150,195 }, rect15 = { 30,110,150,140 }, rect16 = {30,55,150,85};
	//“产地”“价格”“返回”“库存”"下一页" "上一页"
	RECT rect6 = { 30,120,150,150 };//由下框输入
	RECT rect7 = { 170,40,870,360 };//信息框
	RECT rect8 = { 170,40,270,80 }, rect9 = { 270,40,370,80 }, rect10 = { 370,40,470,80 }, rect11 = { 470,40,570,80 }, rect12 = { 570,40,670,80 }, rect13 = { 670,40,770,80 },rect18 = { 770,40,870,80 };
	//?表第一行
	LPRECT rect = (LPRECT)calloc(sizeof(RECT), 49);

	char b = '\0', c = '\0';
	while (b != 7)//rect中每6个代表一行，与每六个str照应
	{
		for (char i = 0; i < 7; i++)
		{
			rect[c] = { 170 + i * 100,80 + 40 * b,170 + (i + 1) * 100,120 + 40 * b };
			c += 1;
		}
		b += 1;
	}
	char ii = '\0';
	char ye = 1;
	string NN= "总库存量为：";
	int N;
	char linshi[20];
	settextcolor(LIGHTGRAY);
	while (1)
	{
		BeginBatchDraw();
		putimage(0, 0, &image);
		settextstyle(15, 0, _T("宋体"));
		drawtext(NN.c_str(), &rect5, DT_VCENTER | DT_SINGLELINE);
		settextstyle(20, 0, _T("宋体"));
		drawtext("上一页", &rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("下一页", &rect3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("返回", &rect4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("库存排序", &rect14, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("产地排序", &rect15, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("价格排序", &rect16, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		drawtext("商品名", &rect8, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("库存", &rect9, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("品牌", &rect10, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("单价", &rect11, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("产家", &rect12, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("类别", &rect13, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("生产日期", &rect18, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		if(ye==1)
		for (char i = 0; i < 49; i++)
			drawtext(str[i].c_str(), &rect[i], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		else
			for (char i = 42,ii=0; i < 98; i++,ii++)
				drawtext(str[i].c_str(), &rect[ii], DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		drawtext(import.c_str(), &rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		setlinecolor(LIGHTGRAY);
		rectangle(rect7.left, rect7.top, rect7.right, rect7.bottom);
		rectangle(rect7.left + 100, rect7.top, rect7.left + 200, rect7.bottom);
		rectangle(rect7.left + 200, rect7.top, rect7.left + 300, rect7.bottom);
		rectangle(rect7.left + 300, rect7.top, rect7.left + 400, rect7.bottom);
		rectangle(rect7.left + 400, rect7.top, rect7.left + 500, rect7.bottom);
		rectangle(rect7.left + 500, rect7.top, rect7.left + 600, rect7.bottom);
		rectangle(rect7.left + 600, rect7.top, rect7.left + 700, rect7.bottom);
		

		rectangle(rect7.left, rect7.top + 40, rect7.right, rect7.top + 80);
		rectangle(rect7.left, rect7.top + 120, rect7.right, rect7.top + 160);
		rectangle(rect7.left, rect7.top + 200, rect7.right, rect7.top + 240);
		rectangle(rect7.left, rect7.top + 280, rect7.right, rect7.top + 320);
		rectangle(rect7.left, rect7.top + 360, rect7.right, rect7.top + 400);
		rectangle(rect7.left, rect7.top + 440, rect7.right, rect7.top + 480);
		if (p == 0)
		{
			setlinecolor(LIGHTGRAY);
			rectangle(rect15.left, rect15.top, rect15.right, rect15.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect15.left, rect15.top, rect15.right, rect15.bottom);
		}
		if (p1 == 0)
		{
			setlinecolor(LIGHTGRAY);
			rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
		}
		if (p2 == 0)
		{
			setlinecolor(LIGHTGRAY);
			rectangle(rect3.left, rect3.top, rect3.right, rect3.bottom);
		}
		else
		{

			setlinecolor(RED);
			rectangle(rect3.left, rect3.top, rect3.right, rect3.bottom);
		}
		if (p3 == 0)
		{
			setlinecolor(LIGHTGRAY);
			rectangle(rect4.left, rect4.top, rect4.right, rect4.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect4.left, rect4.top, rect4.right, rect4.bottom);
		}
		if (p4 == 0)
		{
			setlinecolor(LIGHTGRAY);
			rectangle(rect14.left, rect14.top, rect14.right, rect14.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect14.left, rect14.top, rect14.right, rect14.bottom);
		}
		if (p5 == 0)
		{
			setlinecolor(LIGHTGRAY);
			rectangle(rect16.left, rect16.top, rect16.right, rect16.bottom);
		}
		else
		{
			setlinecolor(RED);
			rectangle(rect16.left, rect16.top, rect16.right, rect16.bottom);
		}
		if (MouseHit())
		{
			F = GetMouseMsg();
			switch (F.uMsg)
			{
			case WM_MOUSEMOVE:
			{
				if (MouseFuncOne(rect15, F) == 1 && p == 0)
					p = 1;
				else
					if (MouseFuncOne(rect1, F) == 0)
						p = 0;
				if (MouseFuncOne(rect2, F) == 1 && p1 == 0)
					p1 = 1;
				else
					if (MouseFuncOne(rect2, F) == 0)
						p1 = 0;
				if (MouseFuncOne(rect3, F) == 1 && p2 == 0)
					p2 = 1;
				else
					if (MouseFuncOne(rect3, F) == 0)
						p2 = 0;
				if (MouseFuncOne(rect4, F) == 1 && p3 == 0)
					p3 = 1;
				else
					if (MouseFuncOne(rect4, F) == 0)
						p3 = 0;
				if (MouseFuncOne(rect14, F) == 1 && p4 == 0)
					p4 = 1;
				else
					if (MouseFuncOne(rect14, F) == 0)
						p4 = 0;
				if (MouseFuncOne(rect16, F) == 1 && p4 == 0)
					p5 = 1;
				else
					if (MouseFuncOne(rect16, F) == 0)
						p5 = 0;
			}; break;
			case WM_LBUTTONDOWN:
			{
				if (MouseFuncOne(rect16, F) == 1)//价格排序
				{
					TeamData(str,N,1);
					sprintf(linshi, "总库存量：%d", N);
					NN = linshi;
				}
				if (MouseFuncOne(rect14, F) == 1)//库存排序
				{
					TeamData(str, N, 2);
					sprintf(linshi, "总库存量：%d", N);
					NN = linshi;
				}
				if (MouseFuncOne(rect15, F) == 1)//库存排序
				{
					TeamData(str, N, 3);
					sprintf(linshi, "总库存量：%d", N);
					NN = linshi;
				}
				if (MouseFuncOne(rect1, F) == 1)
				{
					www = 1;
					fill(w, &w[50], 0);
				}
				if (MouseFuncOne(rect2, F) == 1)
				{
					ye = 1;
					
				}
				if (MouseFuncOne(rect3, F) == 1)
					ye = 2;
				if (MouseFuncOne(rect4,F) == 1)
					WindowTwo();

			}; break;

			}
		}
		
		FlushBatchDraw();
	}
}
int UserCreat(const char *x,const char *xx,const char *xxx)//
{//x为用户名，xx为用户的密码，xxx为确认的密码
	struct User* p=users,*pre=NULL;
	while (p)
	{
		if (!strcmp(p->name, x))
			return 0;//返回0则应该提示用户已经存在
		pre = p;
		p = p->next;
	}
	if (strcmp(xx, xxx))
		return 2;//密码与确认密码不符
	pre->next = new struct User;
	sprintf(pre->next->name, "%s", x);
	sprintf(pre->next->passwd, "%s", xx);
	pre->next->weight = 0;
	return 1;//返回1则表示添加成功
}
int UserChangePasswd(const char* x)
{
	struct User* p = users;
	if (weightNow == 1)
	{
		while (p)
		{
			if (!strcmp(p->name, x))
			{
				sprintf(p->passwd, "%s", "123456");
				return 1;//重置成功
			}
			p = p->next;
		}
	}
	else
	{
		while (p)
		{
			if (!strcmp(p->name, NAME.c_str()))
			{
				sprintf(p->passwd, "%s", x);
				return 1;//重置成功
			}
			p = p->next;
		}
	}

	return 0;
}
int GoodsSell(const char*x, const char*xx, const char*xxx,int &kucun,float &danjia,int pan)
{
	int a ,b;
	for (a = 0; a < 4; a++)
	{
		if (!strcmp(x, (kind[a].classname).c_str()))
			break;
		if (a == 3)
			return 0;//类名错误，返回
	}
	struct GoodsMessage* p = kind[a].next;
	while (p)
	{
		if (!strcmp(xx, (p->name).c_str()))
		break;
		p = p->next;
	}
	if (p == NULL)
		return 1;//商品名称错误，返回
	sscanf(xxx, "%d", &b);
	if (p->sum == 0)
	{
		kucun = 0;
		danjia = p->price;
		return 2;//库存为0，返回
	}
	if (b <= 0)
		return -1;
	if (b > p->sum)
	{
		kucun = p->sum;
		danjia = p->price;
		return 3;//所需数量大于库存数量，返回
	}
	kucun = p->sum;
	danjia = p->price;
	if (pan == 1)
		p->sum -= b;//减去购买的
	return 4;//检测完毕，可以出售，返回
	
}

int FillData(string*str)//先修改，在修改的过程中添加，最后实施删除
{
	struct GoodsMessage* p=NULL, * pre=NULL, * pp=NULL, * x=NULL;//下面使用时，p为当前结点，pre为p的上一结点，pp为p的下一个结点,x为工具指针
	char x1, x2, x3=0, x4,x5;
	char w[10] = "\0",pan=0;
	for (x2 = 5; x2<=47; x2 += 7)
	{
		if (str[x2] != "")
		{
			for (x5 = 0; x5 < 4; x5++)
			{
				if (!strcmp(str[x2].c_str(), kind[x5].classname.c_str()))
					break;
				if (x5==3)
					return 0;//表的类名栏受到非法修改，返回
			}

		}
	}
	for (x1 = 0, x2 = 5, x3 = 0; x2 <= 47; x2 += 7,x1+=7)//该部分实现了修改，添加，删除商品功能
	{
		if (str[x2] != "")
		{
			for (x4 = 0,pan=0; x4 < 4; x4++)
			{
				if (!strcmp(str[x2].c_str(), kind[x4].classname.c_str()))//类名是否相同
				{
					p = kind[x4].next;
					while (p)
					{
						if (!strcmp(str[x1].c_str(), p->name.c_str()))//商品名与现有商品名相同,以下完成了修改
						{
							sscanf(str[x1 + 1].c_str(), "%d",&p->sum);
							p->brind = str[x1 + 2];
							sscanf(str[x1 + 3].c_str(), "%f", &p->price);
							p->bornland = str[x1 + 4];
							p->borndate = str[x1 + 6];

							pan = 1;//为下文铺垫
						}
						pre = p;
						p = p->next;
					}
					if (pan == 0)//类名相同，而商品名与现有商品名均不同，属于新添加的商品;下面实现了同类名新商品的添加
					{//能进如这块说明，p一定为NULL，pre指向p的前一个不为空的结点
						p = new struct GoodsMessage;
						p->name = str[x1];
						sscanf(str[x1 + 1].c_str(), "%d", &p->sum);
						p->brind = str[x1 + 2];
						sscanf(str[x1 + 3].c_str(), "%f", &p->price);
						p->bornland = str[x1 + 4];
						p->borndate = str[x1 + 6];
						p->next = NULL;
						pre->next = p;
					}
				}
				
			}
		}
		else
			if (str[x1] != "")//类名为空，且商品名不为空，触发删除条件
			{
				for (x4 = 0; x4 < 4; x4++)
				{
					p = kind[x4].next;
					pre = p;
					pp = p->next;
					while (p)
					{
						if (!strcmp(str[x1].c_str(), p->name.c_str()))//找到商品名相同的，即要删除的商品
						{
							
							if (p == pre)//针对第一个结点就是要删除的结点
							{
								kind[x4].next = pp;
								x4 = 5;//干涉外循环条件
								break;
							}
							else
							{
								pre->next = pp;
								x4 = 5;//干涉外循环条件
								break;
							}
						}
						pre = p;
						p = p->next;
						pp = p->next;
					}
				}
			}
	}
	

	
}
int FindData(string*str, string&import)
{
	char pan = '\0',x1,x2,x3,x4;
	for ( x1 = 0; x1 < 4; x1++)
	{
		if (!strcmp(kind[x1].classname.c_str(), import.c_str()))
		{
			pan = 1;//查找的是类名
			break;
		}
	}
	struct GoodsMessage* p =NULL;
	for (x2 = 0; x2 < 4; x2++)
	{
		p = kind[x2].next;
		while (p)
		{
			if (!strcmp((p->name).c_str(), import.c_str()))
			{
				pan = 2;//查找的是商品名
				x2 = 5;///干涉循环变量以跳出循环
				break;
			}
			else
				if (!strcmp((p->bornland).c_str(), import.c_str()))
				{
					pan = 3;//查找的是产地
					x2 = 5;///干涉循环变量以跳出循环
					break;
				}
			p = p->next;
		}
	}
	char i = 0,w[20];//工具变量
	if (pan== 0)
		return 0;//import输入有误
	if (pan == 1)
	{
		p = kind[x1].next;
		while (p)
		{
			str[i++] = p->name;
			sprintf(w, "%d",p->sum);
			str[i++] = w;
			str[i++] = p->brind;
			sprintf(w, "%.2f", p->price);
			str[i++] = w;
			str[i++] = p->bornland;
			str[i++] = kind[x1].classname;
			str[i++] = p->borndate;
			
			p = p->next;
		}
		return 1;//类名查询并赋值完毕
	}
	if (pan == 2)
	{
		for (x3 = 0; x3< 4; x3++)
		{
			p = kind[x3].next;
			while (p)
			{
				if (!strcmp((p->name).c_str(), import.c_str()))
				{
					str[i++] = p->name;
					sprintf(w, "%d", p->sum);
					str[i++] = w;
					str[i++] = p->brind;
					sprintf(w, "%.2f", p->price);
					str[i++] = w;
					str[i++] = p->bornland;
					str[i++] = kind[x3].classname;
					str[i++] = p->borndate;
					
				}
				p = p->next;
			}
			if (x3 == 3)
				return 2;//商品名称查询并复制完毕
		}
	}
	if (pan == 3)
	{
		for (x4 = 0; x4 < 4; x4++)
		{
			p = kind[x4].next;
			while (p)
			{
				if (!strcmp((p->bornland).c_str(), import.c_str()))
				{
					str[i++] = p->name;
					sprintf(w, "%d", p->sum);
					str[i++] = w;
					str[i++] = p->brind;
					sprintf(w, "%.2f", p->price);
					str[i++] = w;
					str[i++] = p->bornland;
					str[i++] = kind[x4].classname;
					str[i++] = p->borndate;
				
				}
				p = p->next;
			}
			if (x4 == 3)
				return 3;//商品名称查询并复制完毕
		}
	}
	return 1;
}
int TeamData(string*x,int& N,int n)
{
	struct GoodsMessage* p;
	char z='\0',x1, x2, x3, x4;
	int w = 0, w1 = 0, w2 = 0,w3;
	
	float ww=0;//工具变量
	char linshi[15]="\0";
	for (z = 0; z < 4; z++)
	{
		p = kind[z].next;
		while (p)
		{
			w+=p->sum;//计算出商品的总数
			p = p->next;
		}
	}
	N = w;
	if (n == 1)//按价格排序
	{
		float money[30] = { 0 };
		for (x2 = 0; x2 < 4; x2++)
		{
			p = kind[x2].next;
			while (p)
			{
				money[w1++] = p->price;
				p = p->next;
			}
		}
		sort(money, money + w1,_SORT);
		for (x2 = 0; x2 < w1; x2++)
		{
			for (x3 = 0; x3 < 4; x3++)
			{
				p = kind[x3].next;
				while (p)
				{
					if (p->price == money[x2])
					{
						x[w2++] = p->name;
						sprintf(linshi, "%d", p->sum);
						x[w2++] = linshi;
						x[w2++] = p->brind;
						sprintf(linshi, "%.2f", p->price);
						x[w2++] = linshi;
						x[w2++] = p->bornland;
						x[w2++] = kind[x3].classname;
						x[w2++] = p->borndate;
						x3 = 5;//干涉循环条件进行循环控制
						break;
					}
					p = p->next;
				}
			}
		}
	}
	if (n == 2)//按库存排序
	{
		int money[30] = { 0 };
		for (x2 = 0; x2 < 4; x2++)
		{
			p = kind[x2].next;
			while (p)
			{
				money[w1++] = p->sum;
				p = p->next;
			}
		}
		sort(money, money + w1, _SORT);
		for (x2 = 0; x2 < w1; x2++)
		{
			for (x3 = 0; x3 < 4; x3++)
			{
				p = kind[x3].next;
				while (p)
				{
					if (p->sum == money[x2])
					{
						x[w2++] = p->name;
						sprintf(linshi, "%d", p->sum);
						x[w2++] = linshi;
						x[w2++] = p->brind;
						sprintf(linshi, "%.2f", p->price);
						x[w2++] = linshi;
						x[w2++] = p->bornland;
						x[w2++] = kind[x3].classname;
						x[w2++] = p->borndate;
						x3 = 5;//干涉循环条件进行循环控制
						break;
					}
					p = p->next;
				}
			}
		}
	}
	if (n == 3)//按产地排序
	{
		struct chandi
		{
			int num;
			char chandi[3];
		}money[4] = { {0,"AA"},{0,"BB"},{0,"CC"},{0,"DD"} };
		for (x2 = 0; x2 < 4; x2++)
		{
			p = kind[x2].next;
			while (p)
			{
				for (x1 = 0; x1 < 4; x1++)
				{
					if (!strcmp(p->bornland.c_str(), money[x1].chandi))
					money[x1].num++;
				}
				p = p->next;
			}
		}
		for (x1 = 0; x1 < 3; x1++)
		{
			for (x2 = x1 + 1; x2 < 4; x2++)
			{
				if (money[x1].num < money[x2].num)
					swap(money[x1], money[x2]);
			}
		}
		for (x2 = 0; x2 < 4; x2++)
		{
			for (x3 = 0; x3 < 4; x3++)
			{
				p = kind[x3].next;
				while (p)
				{
					if (!strcmp(p->bornland.c_str(), money[x2].chandi))
					{
						x[w2++] = p->name;
						sprintf(linshi, "%d", p->sum);
						x[w2++] = linshi;
						x[w2++] = p->brind;
						sprintf(linshi, "%.2f", p->price);
						x[w2++] = linshi;
						x[w2++] = p->bornland;
						x[w2++] = kind[x3].classname;
						x[w2++] = p->borndate;
					}
					p = p->next;
				}
			}
		}
		
	}
	return 0;
	
}
int StoreMessage()//保存至本地
{
	FILE* F;
	struct GoodsMessage * p;
	F = fopen("商品保存柜.dat", "wb");
	if (F == NULL)
		return 0;
	char x1;
	for (x1 = 0; x1 < 4; x1++)
	{
		p = kind[x1].next;
		while (p)
		{
			fputs(kind[x1].classname.c_str(), F);
			fwrite(p, sizeof(struct GoodsMessage), 1, F);
			p = p->next;
		}
	}
	fclose(F);
	return 1;
	
}
int ReadStore()//本地导入
{
	FILE* F;
	struct GoodsMessage* p=NULL,*pp=NULL,*pre=NULL;
	F = fopen("商品保存柜.dat", "rb");
	if (F == NULL)
		return 0;
	char x1,x2,linshi[30]="\0";
	string x;
	for (x1 = 0; x1 < 4; x1++)
	{
		p = kind[x1].next;
		while(1)
		{
			fgets(linshi,kind[x1].classname.length()+1, F);
			if (!strcmp(linshi, kind[x1].classname.c_str()))
			{
				if (p == NULL)
				{
					p = new struct GoodsMessage;
					p->next = NULL;
					pre->next = p;
				}
				fread(p, sizeof(struct GoodsMessage), 1, F);
				pre = p;
				p = p->next;
			}
			else
			break;
		}
	}
	fclose(F);
	return 1;
}

int _SORT(int a, int b)
{
	return a > b;
}
//      2021/6/13/12点33分