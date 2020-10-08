//cursor optimized(√); + score export & import; 倒计时已写好未加入 
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#define f(i, a, b) for(register int i = a; i <= b; i++) 
#define Chenxy 30
#define CntDn 30	//count down time

#define BLANK '+'	//cell unfilled
#define CUR '\v'	//cursor
#define OO 'O'	//player 1
#define W 'w'
#define A 'a'
#define S 's'
#define D 'd' 
#define Q ' '
#define XX 'X'	//player 2
#define I 0x48
#define K 0x50
#define J 0x4b
#define L 0x4d
#define P '\r'

#define SCTA SetConsoleTextAttribute
#define hOUT GetStdHandle(STD_OUTPUT_HANDLE)
#define cWHITEh SCTA(hOUT, FOREGROUND_INTENSITY)
#define cDEFAULT SCTA(hOUT, 15)
#define pSTR cDEFAULT, gotoxy(0, n * 3 + 7)

#define COVER pSTR, puts("DON'T cover the filled cell                ")
#define OVERFLOW pSTR, puts("don't run out of the checkerboard                ")
#define INVALID1 pSTR, printf("input 1 charater which can be %c/%c/%c/%c only        \n", W, A, S, D)
#define INVALID2 pSTR, printf("input 1 charater which can be ↑↓←→ only        \n")
#define RESULT "player %c : player %c = %d : %d\t\t\t\t\n", OO, XX, win1, win2

#define IMPORT fopen("history score.cxy", "r")
#define EXPORT fopen("history score.txt", "a")
#define EXPORT2 freopen("history score.cxy", "w", stdout)
#define CHENXY cWHITEh, puts("\nGobang made by Chenxy@FJNU, 19-11-14\n"), cDEFAULT

int cO, cX, cNUM = 15, cCELL = 15;	//color
int n, xx, yy, step, win1, win2;
char a[Chenxy][Chenxy];
bool fill[Chenxy][Chenxy], result;	//filled? have result?

void def();
void rule();
void import();

void init(int);
void hide();
void gotoxy(int, int);
void print();
void printP(int, int);
void start();
void move(char, char, char, char, char);
void play();
void check();

void cntDn();	//count down qwq
void timestamp();

FILE *fp;
void import() {
	fp = IMPORT;
	fscanf(fp, "%d %d", &win1, &win2);
	fp = EXPORT;
	fprintf(fp, "\tlast result:\t"RESULT);
	fclose(fp);
	
	printf("\tlast  result:\t"RESULT), puts("\n\n");
	getch();
}

void rule() {
	system("cls");
	CHENXY;
	
	getch();
	puts("there are 2 players, player O and player X.\n");
	puts("player O FIRST, and then 2 players take turns placing pieces on the board.\n");
	puts("the WINNER is who FIRST get an unbroken row of 5 pieces horizontally / vertically / diagonally.\n");
	getch();
	puts("player O: 【w/a/s/d】to move the cursor and【space】to place the piece\n");
	puts("player X: 【↑/↓/←/→】and【enter】\n");
	puts("Got it?\n");
	getch(); 
}

void color(int &c) {c = c > 15 ? (c - 15) * 17 : c;}
void def() {
	//board size:
	puts("Now you can define the size of the board.\n");
	do{
		puts("input the size(interger n, 5 <= n <= 15) of the board(n * n):");
		scanf("%d", &n);
	} while(n < 5 || n > 15);
	if(n >= 9) puts("\ntips: please MAXIMIZE the window to full screen for the BETTER experience."), getch(); 
	init(n);
	SCTA(hOUT, 15);
	
	//board color:
	puts("Now you can define the color of axises, corsor and blank cells.\n");
	f(i, 1, 15) SCTA(hOUT, i), printf("%3d   ", i); puts("\n");
	do{
		puts("input 2 intergers(1~15) to choose 2 colors for [axises, corsor] and [blank cells]:");
		scanf("%d%d", &cNUM, &cCELL);
	} while(cNUM < 1 || cNUM > 15 || cCELL < 1 || cCELL > 15);
	f(i, 1, 15) SCTA(hOUT, i), printf("%3d   ", i); puts("\n");
	init(n);
	SCTA(hOUT, 15);
	
	//pieces color:
	puts("Now you can define the color of \"O\" pieces and \"X\" pieces.\n");
	f(i, 1, 15) SCTA(hOUT, i), printf(" %3d  ", i); puts("");
	f(i, 1, 15) SCTA(hOUT, i*16), printf(" %3d  ", i+15); SCTA(hOUT, 15), puts("\n");
	do{
		puts("input 2 intergers(1~30) to choose 2 colors for \"O\" and \"X\":");
		puts("(or input interger 0 to hide one of players' pieces or both)");
		scanf("%d%d", &cO, &cX);
	}while(cO < 0 || cO > 30 || cX < 0 || cX > 30);
	if(!cO || !cX) puts("\ntips: the pieces now is invisible!\n"), getch();
	color(cO), color(cX);
	
	puts("\n\n\t\t\t\tWell. Then just have fun~ ^ ^\t\t\t\t");
}

//print the piece & the whole board:
void printP(int x, int y) {
	if(a[x][y] == BLANK) SCTA(hOUT, cCELL), printf("%c", a[x][y]);
	else if(a[x][y] == OO) SCTA(hOUT, cO), printf("%c", a[x][y]);
	else if(a[x][y] == XX) SCTA(hOUT, cX), printf("%c", a[x][y]);
}
void print() {
	system("cls");
	CHENXY;
	SCTA(hOUT, cNUM);
	printf("\n  x\t");
	f(i, 1, n)
		printf("%d\t", i);		//num_x
	puts("\ny\n");
	f(i, 1, n) {
	 	printf("%d\t", i);		//num_y
		f(j, 1, n) {
			printP(i, j);
			SCTA(hOUT, cNUM);
			if(i == xx && j == yy) printf("%-2c", CUR);			//cur_right
			if(i == xx && j + 1 == yy) printf("     %c", CUR);	//cur_left
			printf("\t");
		}
	 	puts("\n\n");
	 }
}

//initial: set zero; locate the coordinate to the center; print the board;
void init(int n) { 
	step = 0;
	result = 0;
	xx = (n + 1) / 2;
	yy = (n + 1) / 2;
	f(i, 1, n)
	f(j, 1, n)
		a[i][j] = BLANK;
	memset(fill, 0, sizeof(fill));
	print();
}

//hide the cursor: 
void hide() {
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOUT, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOUT, &cci);
}

//go to point(x, y):
void gotoxy(int x, int y) {
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = {x, y};
	SetConsoleCursorPosition(hout, pos);
}

//start: hit keyboard to move(), report errors if to type illegally;
int ch;
bool invalid;
void start() {
	while(1)
	if(kbhit()) {
		ch = getch();
		if(!(step & 1)) {
			pSTR, puts("                                                ");
			move(W, S, A, D, Q);
			if(result) break;
			if(invalid) INVALID1; 
		}
		else {
			pSTR, puts("                                                ");
			move(I, K, J, L, P);
			if(result) break;
			if(invalid) INVALID2; 
		}
	}
}

//move & print the cursor: report errors if cursor out of bounds; place pieces to play()
//printCur: gotoxy(x,y) to erase the cursor, then write in gotoxy(_x,_y) 
void printCur(int x, int y, int _x, int _y) {
	hide(); 
	gotoxy(x * 8 - 2, y * 3 + 4), printf("  "); 
	gotoxy(x * 8 + 1, y * 3 + 4), printf("  ");
	SCTA(hOUT, cNUM);
	gotoxy(_x * 8 - 2, _y * 3 + 4), printf("%c ", CUR);
	gotoxy(_x * 8 + 1, _y * 3 + 4), printf("%-2c", CUR);
	cDEFAULT;
}
void move(char w, char s, char a, char d, char q) {
	invalid = 0;
	if(ch == q) play();
	if(ch == 0xe0) ch = getch();
	if(ch == w) if(xx - 1 < 1) OVERFLOW; else printCur(yy, xx, yy, xx-1), xx--;
	else if(ch == s) if(xx + 1 > n) OVERFLOW; else printCur(yy, xx, yy, xx+1), xx++;
	else if(ch == a) if(yy - 1 < 1) OVERFLOW; else printCur(yy, xx, yy-1, xx), yy--;
	else if(ch == d) if(yy + 1 > n) OVERFLOW; else printCur(yy, xx, yy+1, xx), yy++;
	else if(ch != q) invalid = 1; 
}

/*
//count down time: (written but haven't added to main() yet
void cntDn() {
	f(i, 1, CntDn) 
	f(j, 1, 100) {
		if(!kbhit()){
			Sleep(10);
			gotoxy(30, 2);
			printf("%d", CntDn - i);
			if(i == CntDn) result = 1, step & 1 ? win2++ : win1++;
		}
		else start();
	}
}
*/

//play: if place in the cell filled then report errors , or, print the new piece
void play() {
	if(fill[xx][yy]) COVER;
	else a[xx][yy] = ++step & 1 ? OO : XX, 
		fill[xx][yy] = 1, 
		gotoxy(yy * 8, xx * 3 + 4), printP(xx, yy),
//		print(), 	//print the whole board
		check();
}

//check whether have a result: 5 in a row? or ended in a draw? 
int dx[5] = {0, 0, 1, 1, 1};	//r,d,lr,ll
int dy[5] = {0, 1, 0, 1, -1};
bool overflow(int x, int y) {return x < 1 || x > n || y < 1 || y > n ? 1 : 0;}
void check() {
	char c = step & 1 ? OO : XX;
	SCTA(hOUT, 15);
	if(step == n * n) {puts("hehe"), result = 1; return;}
	f(i, 1, 4){
		int c1 = 0, c2 = 0;
		for(int X = xx + dx[i], Y = yy + dy[i]; a[X][Y] == c && !overflow(X, Y); X += dx[i], Y += dy[i]) c1++; 
		for(int X = xx - dx[i], Y = yy - dy[i]; a[X][Y] == c && !overflow(X, Y); X -= dx[i], Y -= dy[i]) c2++; 
		if(c1 + c2 + 1 >= 5) {
			c == OO ? win1++ : win2++;
			pSTR, printf("player %c is the winner!                        \n", c);
			result = 1;
			break;
		}
	}
}

void timestamp() {
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	fprintf(fp, "%2d-%02d-%02d %02d:%02d:%02d\t",
	 sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
}


int main() {
	CHENXY;
	
	puts("\n\n\t\t\trestart or continue?");
	puts("\n\n\t\t\tpress key 1 to continue or any other key to start a newgame.\n\n\n");
	if(getch() == '1') import();
	else rule();
	
	def();
	getch();
	hide(); 
	init(n);

	while(1) {
		start();
		if(result) {
			cDEFAULT;
			printf(RESULT);
			fp = EXPORT;
			timestamp(), fprintf(fp, RESULT);
			fclose(fp);
//			fclose(stdout); 
//			freopen("CON", "w", stdout);
//			freopen + printf, 其他方法输出会出事情 qwq
			
			printf("\nplay again?\npress key 1 to EXIT or any other key to PLAY AGAIN.\n");
			char ch = getch();
			if(ch == '1') {
				fp = EXPORT;
//				fprintf(fp, "\n"), timestamp(), fprintf(fp, RESULT);	//repeat the final score
				fprintf(fp, "\n————————————————————————\n\n");
				fclose(fp);
				
				EXPORT2;
				printf("%d %d", win1, win2);	//save the score for the next playing 
				return 0;
			}
			else {init(n); continue;}
		}
	}

	system("pause");
	return 0;
}
