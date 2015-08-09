#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

#define UP 72
#define DOWN 80
#define ENTER 13
#define ESC 27
#define LEFT 75
#define RIGHT 77
#define LEN sizeof(struct snake)
#pragma comment(lib,"Winmm.lib")
#define N 40
/*定义全局变量*/
int a[N][N] = { 0 };                    /*二维数组是全局变量*/
int node = 3;                            /*蛇身的初始化长度 以蛇的长度来积分*/
int score = 0;                            /*得分初始化为0*/
int speed = 200;                             /*游戏速度可调整*/
int num = 0;                                  /*吃掉食物的个数 到4就置0 即吃满四个就重新计数重新生成地雷和毒草*/
int i = 0;                                   /*用于删除一半蛇身的循环变量*/
int sum;                                      /*吃掉的所有食物个数*/
int level=0;									
int top_score[5];                           /*排行榜*/
int saved_level;
int running=1;
enum {
	/*枚举方向*/
	idle,
	up,
	down,
	left,
	right
}direction;


int main()
{
	int t;
	
	void gotoxy(int x, int y);
	void dwall();
	void dwall1();
	void dwall2();
	void change(int t);
	void add(int x, int y);
	void move();
	void choose_level();
	void check_level();
	void draw_extra_wall(int);
	void load_data();
	void save_data();
	void show_top_scores();
	void dwall3();
	load_data();
	choose_level();
	                                            //dwall() and select speed
	add(2, 2);
	add(2, 3);
	add(2, 4);
	while (running)
	{
		t = _getch();
		change(t);
		while(!kbhit())
		{
			move();
			Sleep(speed);                             /*用来控制蛇移动的速度*/
			check_level();                       /*change to next level if snake is long enough*/
			score = 10 * node - 30;
			gotoxy(7, N + 1);
			printf("Your score is %d", score);
			gotoxy(0, 10);
			printf("your score is %d", score);

		}
	}
	save_data();
	return 0;
}

	struct snake
	{                                   /*定义蛇结构体*/
		int x;
		int y;
		struct snake *pre;
		struct snake *next;
	}*head, *tail;

	struct Food                                       /*定义食物的结构体*/
	{
		int x;
		int y;
		int yes;
	}food;

	struct Poison
	{
		int x;
		int y;
		int yes;
	}poison;

	struct Mine
	{
		int x;
		int y;
	}mine;

	
	/************增加蛇头***************/
	void add(int m,int n)
	{
		struct snake *temp;
		temp = (struct snake *)malloc(LEN);
		temp->x = m;
		temp->y = n;
		temp->pre= NULL;
		  
		if (head == NULL)
		{
			head = temp;
			tail = temp;
			temp->next = NULL;
		}
		else
		{
			head->pre = temp;
			temp->next = head;
			head = head->pre;          /*先连上新的节点 再使新的节点为头*/
		}
		a[m][n] = 1;                        
		gotoxy(m, n);                          /*画出蛇身*/
		printf("%c", 29);
    }


	/************删去蛇尾***************/
	void del()
	{
		struct snake *temp;
		temp = tail;
		tail = tail->pre;
		tail->next = NULL;
		a[temp->x][temp->y] = 0;
		gotoxy(temp->x, temp->y);
		printf(" ");
		free(temp);
	}

	/**********毒草的函数***************/
	void Poison()
	{
			 srand((unsigned)time(NULL));
				do
				{
					poison.x = rand() % 38 + 1;
					poison.y = rand() % 38 + 1;
				} while (a[poison.x][poison.y] != 0);
			    
				a[poison.x][poison.y] = 2;
				gotoxy(poison.x, poison.y);
				printf("%c", 21);
	}

	/************地雷的函数**************/
	void Mine()
	{
		srand((unsigned)time(NULL));
		do
		{
			mine.x = rand() % 38 + 1;
			mine.y = rand() % 38 + 1;
		} while (a[mine.x][mine.y] != 0);
		a[mine.x][mine.y] = 3;
		gotoxy(mine.x, mine.y);
		printf("%c", 15);
	}

	/************移动的函数*************/
	void move()                              /*一次移动一格 通过循环一直移动 无键盘响应的时候一直向前移动 */
	{
		void Poison();
		void add();
		void del();
		void gotoxy(int x, int y);
		int m, n;                                    /*将要移向的蛇头的坐标*/
        m = head->x;
		n = head->y;
		switch (direction)
		{
		case up:m--;
			break;
		case down:m++;
			break;
		case left:n--;
			break;
		case right:n++;
			break;
		}
		if (a[m][n] == 1||node<3)                     /*如果蛇头碰到墙或自己的蛇身或蛇身长度小于三 就清屏结束*/
		{
			system("cls");
			gotoxy(10, 9);
			printf("****************************************************");
			gotoxy(11, 9);
			printf("*                                                  *");
			gotoxy(12, 9);
			printf("*                                                  *");
			gotoxy(13, 9);
			printf("*              GAME OVER!!!!!!                     *");
			gotoxy(14, 9);
			printf("*                                                  *");
			gotoxy(15, 9);
			printf("*                                                  *");
			gotoxy(16, 9);
			printf("****************************************************");
			//exit(0);                        /*结束*/
			running = 0;
			return;
		}
		/*************出现食物**************/        /*food由0到1使屏幕上出现食物*/
		if (food.yes == 0)
		{
			srand((unsigned)time(NULL));
			do                                                     /*使食物被吃就再出现一个 否则不出现*/
			{
				food.x = rand() % 38 + 1;
				food.y = rand() % 38 + 1;
			} while (a[food.x][food.y] != 0);
			food.yes = 1;
		}
		if (food.yes == 1)
		{
			a[food.x][food.y] = 4;
			gotoxy(food.x, food.y);
			printf("%c", 5);
		}
		if (a[m][n] == 4)
		{
		add(m, n);
		food.yes = 0;
		node++;                                  /*若食物被吃则由0到1重新生成 否则一直不生成新的 一直是原来的*/
		num++;                                     /*且蛇头增加一个*/            
		sum++;                                        /*食物总数自增 直到20个时毒草消失*/
		return;
	    }
		if (num == 5)
		{
			Poison();
			Poison();
			Poison();
			Poison();
			Mine();
			Mine();
			num = 0;
			                                         /*num重新初始化为零 使每吃四个食物就出现一次*/
		}
		if (a[m][n] == 2)
		{
			add(m, n);
			node--;
			del();
			del();
			return;
		}
		if (a[m][n] == 3)
		{
			add(m, n);
			del();
			if (node % 2 == 0)
			{
				node = node / 2;
				for (i = 0; i < node; i++)
				{
					del();
				}

			}
			else
			{
				node = node / 2;
				for (i = 0; i <= node; i++)
				{
					del();
				}
				return;
			}
		}

		if (sum == 10)                                                /*毒草消失*/
        {
			int i = 0, j = 0;
			for (i = 0; i < N;i++)
			{
				for (j = 0; j < N;j++)
				{
					if (a[i][j] == 2)
					{
						a[i][j] = 0;
						gotoxy(i, j);
						printf(" ");
					}
			    };
			};
		}
			
			
			
	   add(m, n);                                   /*m,n是变化的？？？*/
		del();
   }




	/*应用goto函数输出墙 输出墙的函数 */
		void dwall()
	{
		int m;
		for (m = 0; m <= 39; m++)                          /* 墙的大小可以自己定义*/
		{
			gotoxy(m, 0);
			a[m][0] = 1;
			printf("%c", 42);
			gotoxy(39, m);
			a[39][m] = 1;
			printf("%c", 42);
			gotoxy(m, 39);
			a[m][39] = 1;
			printf("%c", 42);
			gotoxy(0, m);
			a[0][m] = 1;
			printf("%c", 42);
		}
		
		gotoxy(1, N + 1);
		printf("%c--food add a section",5);
		gotoxy(3, N + 1);
		printf("%c--poison reduce a section",21);
		gotoxy(5, N + 1);
		printf("%c--mine be half",15);
	}
		
		
		void dwall1()
		{
			for (i = 20; i <= 28; i++)
			{
				gotoxy(i, 10);
				a[i][10] = 1;
				a[i][11] = 1;
				printf("%c%c", 42, 42);
			};

		}
		
		
		
		void dwall2()
		{
			for (i = 6; i < 9; i++)
			{
				gotoxy(i, 30);
				a[i][30] = 1;
				a[i][31] = 1;
				a[i][32] = 1;
		        printf("%c%c%c", 42, 42, 42);
			};
		}
	
		void dwall3()
		{
			for (i = 30; i < 33; i++)
			{
				gotoxy(i, 30);
				a[i][30] = 1;
				a[i][31] = 1;
				printf("%c%c", 42, 42);
			};
		}

void gotoxy(int x, int y)           /*定义goto函数*/
{
		int xx = 0x0b;
		HANDLE hOutput;
		COORD loc;
		loc.X = 2 * y;
		loc.Y = x;
		hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hOutput, loc);
		CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
		return;
}

/*键盘响应 通过按键改变蛇的方向*/
void change(int t)
{
	switch (t)
	{
	case 72:
		if (direction != down)                             /*保证向右的时候不能向左转等*/
		direction = up;
		break;
	case 80:
		if (direction!=up)
			direction = down;
		break;
	case 75:
		if (direction!=right)
		direction = left;
		break;
	case 77:
		if (direction!=left)
		direction = right;
		break;
	case ' ':_getch();                        /*暂停功能 按空格建 getch()能使画面停顿直到再按一个键*/
	}
}

void choose_level()
{
	char choice;
	int start_game = 0;
	while (!start_game) {
		gotoxy(10, 10);
		printf("Welcome to Snake Game\n");
		gotoxy(11, 10);
		printf("0. continue from last time\n");
		gotoxy(12, 10);
		printf("1. choose level 1\n");
		gotoxy(13, 10);
		printf("2. choose level 2\n");
		gotoxy(14, 10);
		printf("3. choose level 3\n");
		gotoxy(15, 10);
		printf("4. Show top scores\n");
		gotoxy(16, 10);
		printf("Please choose 0-4:");

		choice = _getch();
		level = choice - 48;
		if (level< 0 || level > 4) {
			gotoxy(7, 10);
			printf("Wrong choice, Try again");
			
		}
		else if (level == 4) {
			show_top_scores();
		}
		else {
			start_game = 1;
		}
	}
	if (level == 0) {
		level = saved_level;
	}
	//else {
		//level = choice - '0';
	//}
	system("cls");
	switch (level) {
	case 1: 
		dwall();
		dwall1();
		speed = 200;
		break;
	case 2: 
		dwall();
		dwall1();
		dwall2();
		speed = 150;
		break;
	case 3:
		dwall();
		dwall1();
		dwall2();
		dwall3();
		speed = 100;
		break;
	}
		return;
}
void check_level()
{
	switch (level) {
	case 1: 
		if (node >= 7) {
			level++;
			speed = 150;
			draw_extra_wall(level);
		}
	case 2: 
		if (node >= 12) {
			level++;
			draw_extra_wall(level);
			speed = 100;
		}
	case 3:if (node >18)
	{
		system("cls");
		gotoxy(10,10);
		printf("********************");
		gotoxy(11, 10);
		printf("*                  *");
		gotoxy(12, 10);
		printf("*congratulations!!!*");
		gotoxy(13, 10);
		printf("*  you are winner  *");
		gotoxy(14, 10);
		printf("*                  *");
		gotoxy(15, 10);
		printf("********************");

	}
	}
}

void draw_extra_wall(int level)
{
	if (level == 2) {
		                                        /*draw extra wall for level 2*/
		dwall2();
	}
	else if (level == 3)
	{
		dwall3();                             /*draw extra wall for level 3*/
	}
}

void load_data()                                           /*文件保存 记录分数关卡*/
{
	FILE *fp;
	fp = fopen("data.txt", "r");
	if (fp) {
		fscanf(fp, "saved_level=%d,top1=%d,top2=%d,top3=%d,top4=%d,top5=%d", &saved_level, &top_score[0], &top_score[1], &top_score[2],
			&top_score[3], &top_score[4], &top_score[5]);
		fclose(fp);
	}
}
void save_data()
{
	FILE *fp;
	int i,j;
	fp = fopen("data.txt", "w");
	/*get top score*/
	for (i = 0; i < 5; i++){
		if (score > top_score[i]) {
			for (j = i; j < 4; j++) {
				top_score[j + 1] = top_score[j];
			}
			top_score[i] = score;
			break;
		}
	}
	fprintf(fp, "saved_level=%d,top1=%d,top2=%d,top3=%d,top4=%d,top5=%d", level, top_score[0], top_score[1], top_score[2],
		top_score[3], top_score[4], top_score[5]);
	fclose(fp);
}

void show_top_scores()
{
	int c;
	system("cls");
	gotoxy(0, 20);
	printf("Number 1. %d", top_score[0]);
	gotoxy(1, 23);
	printf("2. %d", top_score[1]);
	gotoxy(2, 23);
	printf("3. %d", top_score[2]);
	gotoxy(3, 23);
	printf("4. %d", top_score[3]);
	gotoxy(4, 23);
	printf("5. %d", top_score[4]);
	c = _getch();
	system("cls");
	return;
}

	


	
	
	
	
	
	
	
	
	
	
