#include<curses.h>  //stdio.h inculded
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<signal.h>
#include<sys/time.h>
int x=1;
int y=1;
const int u=2;//const for screen position
const int n=10;//so is it
int spinban(void);//prevent spin some block infinitly
void gaming(void);//get control
void next(void);//next block
void shade(int bot);//block's shade
void upblock(void);//cheat action
void createblock(void);//after assign new position use it to creat block
void removeblock(void);//before assign new position use it to remove block
int spin(void);
int setblock(char type);//set new block
int moveblock(int direct);//control block
void reset(int a);//reset game
void change(void);//hold block
void down(int a);//let block down
void gameover(int b);
int stopfalling=0;//control block whether falling
int block[25][11]={0};//game screen
int bx1,bx2,bx3,bx4,by1,by2,by3,by4,dotx,doty;//blocks' position and reference dot for spin
char shape;//block's shape
int shmod;//tell spin(void) how to spin 
int ban=0;//ban spin infinitly
int shadebot=1;//whether pirnt shade
int changenum=0;//we can just hold once for every block
int lines=0;//score
int speed=40;//well ,speed

int main(void){
	struct itimerval tick;
	tick.it_value.tv_sec=1;
	tick.it_value.tv_usec=0;
	tick.it_interval.tv_sec=0;
	tick.it_interval.tv_usec=25000;

	int pass=0;	
	int sh;
	int i=0;
	int j=0;
	for(i=1;i<=10;i++){
		block[23][i]=1;
		block[24][i]=1;
	}
	initscr();
	cbreak();
	nonl();
	noecho();
	intrflush(stdscr,FALSE);
	keypad(stdscr,TRUE);
	reset(0); 	
	move(x,y);
	while(1){                  
		curs_set(0);//here i make some change to hide pointer
		reset(0);	
		mvprintw(12,n-5,"Enter to strat new game");	  
		sh=getch();   //enter to start game
		switch(sh){
			case '\r':	
				reset(0);//remove Enter to strat new game
				mvprintw(8,1,"holding:");						
				mvprintw(10,17+n,"press Z to hold");
				mvprintw(12,17+n,"press R to restart");
				mvprintw(14,17+n,"press S to hide/show shadow");
				mvprintw(16,17+n,"press Enter to PUASE");
				mvprintw(18,17+n,"LEVEL 1");
			 	mvprintw(20,17+n,"remove lines   0",lines);
				next();
				setblock(shape);
				stopfalling=0;
				shade(shadebot);
				setitimer(ITIMER_REAL, &tick, NULL);
				signal(SIGALRM, down);
				gaming();
			break;
			case 27:
				endwin();
				exit(1);
			break;
			}			
	}
}

void reset(int a){
	int i=0;
	int j=0;
	lines=0;
	clear();
	refresh();
	attron(A_REVERSE);//open white background
	mvaddstr(1,3+n,"tetris");//print words
	attroff(A_REVERSE);//close white background
	for(i=3;i<=22;i++){
		mvaddstr(i,0+n,"|");
		mvaddstr(i,11+n,"|");
	}
	for(i=1;i<=10;i++){
		mvaddstr(2,i+n,"-");
		mvaddstr(23,i+n,"-");
	}
	for(i=0;i<=22;i++){
		for(j=0;j<=10;j++){
			block[i][j]=0;
		}
	}
	 ban=0; 
	 if(a==1){
	shape='N';
	change();
	shape='N';
	 }
	changenum=0;
}

int setblock(char type){
	int num;
	if(type=='N'){
		srand(time(NULL));
		srand(rand());
		num=rand()%7;
		switch(num){
		case 0:
			type='O';	
			break;
		case 1:
			type='I';
			break;
		case 2:
			type='S';
			break;
		case 3:
			type='Z';
			break;
		case 4:
			type='L';
			break;
		case 5:
			type='J';
			break;
		case 6:
			type='T';
			break;
		}
	}			
	shmod=1;
	doty=2;
	dotx=4;
	switch(type){
		case 'O'://O
			shape='O';
			if((block[2][5]==1)||(block[2][6]==1)||(block[3][5]==1)||(block[3][6]==1)){
			return 1;
			}
			block[2][5]=1;
			block[2][6]=1;
			block[3][5]=1;
			block[3][6]=1;
			mvaddstr(3,5+n,"@");
			mvaddstr(3,6+n,"@");
			by1=2;
			by2=2;
			by3=3;
			by4=3;
			bx1=5;
			bx2=6;
			bx3=5;
			bx4=6;
			break;
		case 'I'://I	
			shape='I';
			if((block[3][4]==1)||(block[3][5]==1)||(block[3][6]==1)||(block[3][7]==1)){
			return 1;
			}
			block[3][4]=1;
			block[3][5]=1;
			block[3][6]=1;
			block[3][7]=1;
			mvaddstr(3,4+n,"@");
			mvaddstr(3,5+n,"@");
			mvaddstr(3,6+n,"@");
			mvaddstr(3,7+n,"@");
			by1=3;
			by2=3;
			by3=3;
			by4=3;
			bx1=4;
			bx2=5;
			bx3=6;
			bx4=7;
			break;
		case 'Z'://shape z
			shape='Z';
			if((block[2][5]==1)||(block[2][4]==1)||(block[3][5]==1)||(block[3][6]==1)){
				return 1;
			}
			block[2][4]=1;
			block[2][5]=1;
			block[3][5]=1;//
			block[3][6]=1;
			mvaddstr(3,5+n,"@");
			mvaddstr(3,6+n,"@");
			by1=2;
			by2=2;
			by3=3;//
			by4=3;
			bx1=4;
			bx2=5;
			bx3=5;//
			bx4=6;
			break;
		case 'T'://shape T
			shape='T';
			if((block[2][5]==1)||(block[3][4]==1)||(block[3][5]==1)||(block[3][6]==1)){
				return 1;
			}
			block[2][5]=1;
			block[3][4]=1;
			block[3][5]=1;//
			block[3][6]=1;
			mvaddstr(3,4+n,"@");
			mvaddstr(3,5+n,"@");
			mvaddstr(3,6+n,"@");
			by1=2;
			by2=3;
			by3=3;
			by4=3;
			bx1=5;
			bx2=4;
			bx3=5;
			bx4=6;
			break;
		case 'J'://shape j
			shape='J';
			if((block[2][4]==1)||(block[3][4]==1)||(block[3][5]==1)||(block[3][6]==1)){
				return 1;
			}
			block[2][4]=1;
			block[3][4]=1;
			block[3][5]=1;
			block[3][6]=1;
			mvaddstr(3,4+n,"@");
			mvaddstr(3,5+n,"@");
			mvaddstr(3,6+n,"@");
			by1=2;
			by2=3;
			by3=3;
			by4=3;
			bx1=4;
			bx2=4;
			bx3=5;
			bx4=6;
			break;
		case 'S'://shape s
			shape='S';
			if((block[2][5]==1)||(block[2][6]==1)||(block[3][5]==1)||(block[3][4]==1)){
				return 1;
			}
			block[2][6]=1;
			block[2][5]=1;
			block[3][5]=1;
			block[3][4]=1;
			mvaddstr(3,4+n,"@");
			mvaddstr(3,5+n,"@");
			by1=2;
			by2=2;
			by3=3;
			by4=3;
			bx1=6;
			bx2=5;
			bx3=5;
			bx4=4;
			break;
		case 'L'://shape L
			shape='L';
			if((block[3][4]==1)||(block[2][6]==1)||(block[3][5]==1)||(block[3][6]==1)){
				return 1;
			}
			block[2][6]=1;
			block[3][4]=1;
			block[3][5]=1;
			block[3][6]=1;
			mvaddstr(3,4+n,"@");
			mvaddstr(3,5+n,"@");
			mvaddstr(3,6+n,"@");
			by1=2;
			by2=3;
			by3=3;
			by4=3;
			bx1=6;
			bx2=4;
			bx3=5;
			bx4=6;
			break;
	}
	ban=0;
	return 0;
}
		
int moveblock(int direct){
	if(direct==1){//
		if((bx1>1)&&(bx2>1)&&(bx3>1)&&(bx4>1)){
			
			if((!((bx1==bx2+1)&&(by1==by2)))&&(!((bx1==bx3+1)&&(by1==by3)))&&(!((bx1==bx4+1)&&(by1==by4)))&&(block[by1][bx1-1]==1)){
				return 0;
			}
			if((!((bx2==bx1+1)&&(by2==by1)))&&(!((bx2==bx3+1)&&(by2==by3)))&&(!((bx2==bx4+1)&&(by2==by4)))&&(block[by2][bx2-1]==1)){
				return 0;
			}
			if((!((bx3==bx1+1)&&(by3==by1)))&&(!((bx3==bx2+1)&&(by3==by2)))&&(!((bx3==bx4+1)&&(by3==by4)))&&(block[by3][bx3-1]==1)){
				return 0;
			}
			if((!((bx4==bx1+1)&&(by4==by1)))&&(!((bx4==bx2+1)&&(by4==by2)))&&(!((bx4==bx3+1)&&(by4==by3)))&&(block[by4][bx4-1]==1)){
				return 0;
			}
			dotx--;				
			removeblock();
			bx1--;
			bx2--;
			bx3--;
			bx4--;
			createblock();
			return 1;
		}
		return 0;
	}
	if(direct==2){
		if((by1<22)&&(by2<22)&&(by3<22)&&(by4<22)){
			
			if((!((by1==by2-1)&&(bx1==bx2)))&&(!((by1==by3-1)&&(bx1==bx3)))&&(!((by1==by4-1)&&(bx1==bx4)))&&(block[by1+1][bx1]==1)){
				return 0;
			}
			if((!((by2==by1-1)&&(bx2==bx1)))&&(!((by2==by3-1)&&(bx2==bx3)))&&(!((by2==by4-1)&&(bx2==bx4)))&&(block[by2+1][bx2]==1)){
				return 0;
			}
			if((!((by3==by1-1)&&(bx3==bx1)))&&(!((by3==by2-1)&&(bx3==bx2)))&&(!((by3==by4-1)&&(bx3==bx4)))&&(block[by3+1][bx3]==1)){
				return 0;
			}
			if((!((by4==by1-1)&&(bx4==bx1)))&&(!((by4==by2-1)&&(bx4==bx2)))&&(!((by4==by3-1)&&(bx4==bx3)))&&(block[by4+1][bx4]==1)){
				return 0;
			}	
			doty++;				
				
			removeblock();
			by1++;
			by2++;
			by3++;
			by4++;
			createblock();

			return 1;
		}
		return 0;
	}
	if(direct==3){//
		if((bx1<10)&&(bx2<10)&&(bx3<10)&&(bx4<10)){
			
			if((!((bx1==bx2-1)&&(by1==by2)))&&(!((bx1==bx3-1)&&(by1==by3)))&&(!((bx1==bx4-1)&&(by1==by4)))&&(block[by1][bx1+1]==1)){
				return 0;
			}
			if((!((bx2==bx1-1)&&(by2==by1)))&&(!((bx2==bx3-1)&&(by2==by3)))&&(!((bx2==bx4-1)&&(by2==by4)))&&(block[by2][bx2+1]==1)){
				return 0;
			}
			if((!((bx3==bx1-1)&&(by3==by1)))&&(!((bx3==bx2-1)&&(by3==by2)))&&(!((bx3==bx4-1)&&(by3==by4)))&&(block[by3][bx3+1]==1)){
				return 0;
			}
			if((!((bx4==bx1-1)&&(by4==by1)))&&(!((bx4==bx2-1)&&(by4==by2)))&&(!((bx4==bx3-1)&&(by4==by3)))&&(block[by4][bx4+1]==1)){
				return 0;
			}				
			dotx++;
			removeblock();
			bx1++;
			bx2++;
			bx3++;
			bx4++;
			createblock();

			return 1;
		}
		return 0;
	}
}
int spin(void){
	int origindoty;
	int up=0;
	origindoty=doty;
	if(shape=='O'){
		return 0;
	}
	if(dotx<=0){
		dotx=1;
	}
	if(shape=='I'){
		if(dotx>=8){
			dotx=7;
		}
	}
	else{
		if(dotx>=9){
			dotx=8;
		}
	}
		removeblock();	
	if(shape=='I'){
		if(shmod==1){
			if((block[doty+1][dotx]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+1][dotx+2]==0)&&(block[doty+1][dotx+3]==0)){
				by1=doty+1;
				by2=doty+1;
				by3=doty+1;
				by4=doty+1;
				bx1=dotx;
				bx2=dotx+1;
				bx3=dotx+2;
				bx4=dotx+3;
				createblock();
				return 0;
			}
		}
		else if(shmod==2){
			if(block[doty+3][dotx+2]==1){
				doty-=2;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			else if(block[doty+2][dotx+2]==1){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty][dotx+2]==0)&&(block[doty+1][dotx+2]==0)&&(block[doty+2][dotx+2]==0)&&(block[doty+3][dotx+2]==0)){
				by1=doty;
				by2=doty+1;
				by3=doty+2;
				by4=doty+3;
				bx1=dotx+2;
				bx2=dotx+2;
				bx3=dotx+2;
				bx4=dotx+2;
				createblock();
				return 0;
			}
			else{
				doty=origindoty;
				if(up==1){
				ban--;
				}
			}
		}
		else if(shmod==3){
			if((block[doty+2][dotx]==0)&&(block[doty+2][dotx+1]==0)&&(block[doty+2][dotx+2]==0)&&(block[doty+2][dotx+3]==0)){
				by1=doty+2;
				by2=doty+2;
				by3=doty+2;
				by4=doty+2;
				bx1=dotx;
				bx2=dotx+1;
				bx3=dotx+2;
				bx4=dotx+3;
				createblock();
				return 0;
			}
		}
		else if(shmod==4){
			if(block[doty+3][dotx+1]==1){
				doty-=2;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			else if(block[doty+2][dotx+1]==1){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty][dotx+1]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+2][dotx+1]==0)&&(block[doty+3][dotx+1]==0)){
				by1=doty;
				by2=doty+1;
				by3=doty+2;
				by4=doty+3;
				bx1=dotx+1;
				bx2=dotx+1;
				bx3=dotx+1;
				bx4=dotx+1;
				createblock();
				return 0;
			}
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
	}
	if(shape=='Z'){
		if(shmod==1){
			if((block[doty][dotx]==0)&&(block[doty][dotx+1]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+1][dotx+2]==0)){
				by1=doty;
				by2=doty;
				by3=doty+1;
				by4=doty+1;
				bx1=dotx;
				bx2=dotx+1;
				bx3=dotx+1;
				bx4=dotx+2;
				createblock();
				return 0;
			}
		}
		else if(shmod==2){
			if(block[doty+2][dotx+1]==1){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty][dotx+2]==0)&&(block[doty+1][dotx+2]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+2][dotx+1]==0)){
				by1=doty;
				by2=doty+1;
				by3=doty+1;
				by4=doty+2;
				bx1=dotx+2;
				bx2=dotx+2;
				bx3=dotx+1;
				bx4=dotx+1;
				createblock();
				return 0;
			}
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
		else if(shmod==3){
			if((block[doty+1][dotx]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+2][dotx+1]==0)&&(block[doty+2][dotx+2]==0)){
				by1=doty+1;
				by2=doty+1;
				by3=doty+2;
				by4=doty+2;
				bx1=dotx;
				bx2=dotx+1;
				bx3=dotx+1;
				bx4=dotx+2;
				createblock();
				return 0;
			}	
		}
		else if(shmod==4){
			if(block[doty+2][dotx]==1){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty][dotx+1]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+1][dotx+0]==0)&&(block[doty+2][dotx+0]==0)){
				by1=doty;
				by2=doty+1;
				by3=doty+1;
				by4=doty+2;
				bx1=dotx+1;
				bx2=dotx+1;
				bx3=dotx+0;
				bx4=dotx+0;
				createblock();
				return 0;
			}	
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
	}
	if(shape=='T'){
		if(shmod==1){
			if((block[doty][dotx+1]==0)&&(block[doty+1][dotx]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+1][dotx+2]==0)){
				by1=doty;
				by2=doty+1;
				by3=doty+1;
				by4=doty+1;
				bx1=dotx+1;
				bx2=dotx;
				bx3=dotx+1;
				bx4=dotx+2;
				createblock();
				return 0;
			}
		}
		else if(shmod==2){
			if(block[doty+2][dotx+1]==1){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty+1][dotx+2]==0)&&(block[doty][dotx+1]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+2][dotx+1]==0)){
				by1=doty+1;
				by2=doty;
				by3=doty+1;
				by4=doty+2;
				bx1=dotx+2;
				bx2=dotx+1;
				bx3=dotx+1;
				bx4=dotx+1;
				createblock();
				return 0;
			}	
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
		else if(shmod==3){
			if((block[doty+2][dotx+1]==0)&&(block[doty+1][dotx]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+1][dotx+2]==0)){
				by1=doty+2;
				by2=doty+1;
				by3=doty+1;
				by4=doty+1;
				bx1=dotx+1;
				bx2=dotx;
				bx3=dotx+1;
				bx4=dotx+2;
				createblock();
				return 0;
			}	
		}
		else if(shmod==4){
			if(block[doty+2][dotx+1]==1){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty+1][dotx]==0)&&(block[doty][dotx+1]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+2][dotx+1]==0)){
				by1=doty+1;
				by2=doty;
				by3=doty+1;
				by4=doty+2;
				bx1=dotx;
				bx2=dotx+1;
				bx3=dotx+1;
				bx4=dotx+1;
				createblock();
				return 0;
			}	
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
	}
	if(shape=='J'){
		if(shmod==1){
			if((block[doty][dotx]==0)&&(block[doty+1][dotx]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+1][dotx+2]==0)){
				by1=doty;
				by2=doty+1;
				by3=doty+1;
				by4=doty+1;
				bx1=dotx;
				bx2=dotx;
				bx3=dotx+1;
				bx4=dotx+2;
				createblock();
				return 0;
			}	
		}
		else if(shmod==2){
			if(block[doty+2][dotx+1]==1){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty][dotx+2]==0)&&(block[doty][dotx+1]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+2][dotx+1]==0)){
				by1=doty;
				by2=doty;
				by3=doty+1;
				by4=doty+2;
				bx1=dotx+2;
				bx2=dotx+1;
				bx3=dotx+1;
				bx4=dotx+1;
				createblock();
				return 0;
			}	
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
		else if(shmod==3){
			if(block[doty+2][dotx+2]==1){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty+2][dotx+2]==0)&&(block[doty+1][dotx]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+1][dotx+2]==0)){
				by1=doty+2;
				by2=doty+1;
				by3=doty+1;
				by4=doty+1;
				bx1=dotx+2;
				bx2=dotx;
				bx3=dotx+1;
				bx4=dotx+2;
				createblock();
				return 0;
			}	
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
		else if(shmod==4){
			if((block[doty+2][dotx+1]==1)||(block[doty+2][dotx]==1)){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty+2][dotx]==0)&&(block[doty][dotx+1]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+2][dotx+1]==0)){
				by1=doty+2;
				by2=doty;
				by3=doty+1;
				by4=doty+2;
				bx1=dotx;
				bx2=dotx+1;
				bx3=dotx+1;
				bx4=dotx+1;
				createblock();
				return 0;
			}	
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
	}
	if(shape=='S'){
		if(shmod==1){
			if((block[doty][dotx+2]==0)&&(block[doty+1][dotx]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty][dotx+1]==0)){
				by1=doty;
				by2=doty+1;
				by3=doty+1;
				by4=doty;
				bx1=dotx+2;
				bx2=dotx;
				bx3=dotx+1;
				bx4=dotx+1;
				createblock();
				return 0;
			}	
		}
		else if(shmod==2){
			if(block[doty+2][dotx+2]==1){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty][dotx+1]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+1][dotx+2]==0)&&(block[doty+2][dotx+2]==0)){
				by1=doty;
				by2=doty+1;
				by3=doty+1;
				by4=doty+2;
				bx1=dotx+1;
				bx2=dotx+1;
				bx3=dotx+2;
				bx4=dotx+2;
				createblock();
				return 0;
			}	
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
		else if(shmod==3){
			if((block[doty+2][dotx+1]==1)||(block[doty+2][dotx]==1)){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty+1][dotx+2]==0)&&(block[doty+2][dotx]==0)&&(block[doty+2][dotx+1]==0)&&(block[doty+1][dotx+1]==0)){
				by1=doty+1;
				by2=doty+2;
				by3=doty+2;
				by4=doty+1;
				bx1=dotx+2;
				bx2=dotx;
				bx3=dotx+1;
				bx4=dotx+1;
				createblock();
				return 0;
			}	
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
		else if(shmod==4){
			if(block[doty+2][dotx+1]==1){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty][dotx]==0)&&(block[doty+1][dotx]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+2][dotx+1]==0)){
				by1=doty;
				by2=doty+1;
				by3=doty+1;
				by4=doty+2;
				bx1=dotx;
				bx2=dotx;
				bx3=dotx+1;
				bx4=dotx+1;
				createblock();
				return 0;
			}	
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
	}
	if(shape=='L'){
		if(shmod==1){
			if((block[doty][dotx+2]==0)&&(block[doty+1][dotx]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+1][dotx+2]==0)){
				by1=doty;
				by2=doty+1;
				by3=doty+1;
				by4=doty+1;
				bx1=dotx+2;
				bx2=dotx;
				bx3=dotx+1;
				bx4=dotx+2;
				createblock();
				return 0;
			}
		}
		else if(shmod==2){
			if((block[doty+2][dotx+1]==1)||(block[doty+2][dotx+2]==1)){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty][dotx+1]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+2][dotx+1]==0)&&(block[doty+2][dotx+2]==0)){
				by1=doty;
				by2=doty+1;
				by3=doty+2;
				by4=doty+2;
				bx1=dotx+1;
				bx2=dotx+1;
				bx3=dotx+1;
				bx4=dotx+2;
				createblock();
				return 0;
			}
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
		else if(shmod==3){
			if(block[doty+2][dotx]==1){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty+2][dotx]==0)&&(block[doty+1][dotx]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+1][dotx+2]==0)){
				by1=doty+1;
				by2=doty+1;
				by3=doty+1;
				by4=doty+2;
				bx1=dotx;
				bx2=dotx+1;
				bx3=dotx+2;
				bx4=dotx;
				createblock();
				return 0;
			}
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}
		}
		else if(shmod==4){
			if(block[doty+2][dotx+1]==1){
				doty--;
				ban++;
				up=1;
				if(spinban()==1){
					return 1;
				}
			}
			if((block[doty][dotx+1]==0)&&(block[doty+1][dotx+1]==0)&&(block[doty+2][dotx+1]==0)&&(block[doty][dotx]==0)){
				by1=doty;
				by2=doty;
				by3=doty+1;
				by4=doty+2;
				bx1=dotx;
				bx2=dotx+1;
				bx3=dotx+1;
				bx4=dotx+1;
				createblock();
				return 0;
			}
			else{
				doty=origindoty;
				if(up==1){
					ban--;
				}
			}	
		}
	}
	createblock();
	return 1;
}
int delin(void){
	int count=0;
	int lin=0;
	int i;
	int j;
	int k;
	int l;
	for(i=22;i>=1;i--){
		for(j=1;j<=10;j++){
			count=block[i][j]+count;
		}
		if(count==10){
			lines++;
			lin++;
			for(j=i;j>=2;j--){
				for(k=1;k<=10;k++){
					block[j][k]=block[j-1][k];//cp value from forward li
					if(j>2){
						if(block[j-1][k]==1){
							mvaddstr(j,k+n,"@");
						}
						else if(block[j-1][k]==0){
							mvaddstr(j,k+n," ");
						}
					}
				}
				for(k=1;k<=10;k++){
					block[1][k]=0;
				}
			}
			i++;
		}
		count=0;
	}
	mvprintw(20,17+n,"remove lines %3d",lines);
	if(lines<=10){
	speed=40;
	mvprintw(18,17+n,"LEVEL 1");
	}
	else if(lines<=20){
	speed=20;
	mvprintw(18,17+n,"LEVEL 2");
	}
	else if(lines<=30){
	speed=10;
	mvprintw(18,17+n,"LEVEL 3");
	}
	else if(lines<=40){
	speed=6;
	mvprintw(18,17+n,"LEVEL 4");
	}
	else if(lines<=50){
	speed=5;
	mvprintw(18,17+n,"LEVEL 5");
	}
	else if(lines<=60){
	speed=4;
	mvprintw(18,17+n,"LEVEL 6");
	}
	else if(lines<=70){
	speed=3;
	mvprintw(18,17+n,"LEVEL 7");
	}
	else if(lines<=80){
	speed=2;
	mvprintw(18,17+n,"LEVEL 8");
	}
	else if(lines<=90){
	speed=1;
	mvprintw(18,17+n,"LEVEL 9");
	}
	else if(lines<=100){
	speed=0;
	mvprintw(18,17+n,"LEVEL 10");
	}
	else{
	mvprintw(18,17+n,"GOD LEVEL");
	gameover(1);
	return 1;
	}
	return 0;
}
void removeblock(void){
	if(by1>2){
		mvaddstr(by1,bx1+n," ");
	}
	if(by2>2){
		mvaddstr(by2,bx2+n," ");
	}
	if(by3>2){
		mvaddstr(by3,bx3+n," ");
	}
	if(by4>2){
		mvaddstr(by4,bx4+n," ");
	}
	block[by1][bx1]=0;
	block[by2][bx2]=0;
	block[by3][bx3]=0;
	block[by4][bx4]=0;
}
void createblock(void){
	if(by1>2){
		mvaddstr(by1,bx1+n,"@");
	}
	if(by2>2){
		mvaddstr(by2,bx2+n,"@");
	}
	if(by3>2){
		mvaddstr(by3,bx3+n,"@");
	}
	if(by4>2){
		mvaddstr(by4,bx4+n,"@");
	}
	block[by1][bx1]=1;
	block[by2][bx2]=1;
	block[by3][bx3]=1;
	block[by4][bx4]=1;
}
void upblock(void){
	removeblock();
			by1--;
			by2--;
			by3--;
			by4--;
			doty--;
	createblock();
}
void shade(int bot){
	int k;
	int dokx;
	int doky;
	static int sx1,sx2,sx3,sx4,sy1,sy2,sy3,sy4;
	dokx=dotx;
	doky=doty;
	if(bot==3){
		sy1=0;
		sy2=0;
		sy3=0;
		sy4=0;
		return;
	}
	if(sy1>2){
		mvaddstr(sy1,sx1+n," ");
	}
	if(sy2>2){
		mvaddstr(sy2,sx2+n," ");
	}
	if(sy3>2){
		mvaddstr(sy3,sx3+n," ");
	}
	if(sy4>2){
		mvaddstr(sy4,sx4+n," ");
	}
	if(bot==1){
	sx1=bx1;
	sx2=bx2;
	sx3=bx3;
	sx4=bx4;
	sy1=by1;
	sy2=by2;
	sy3=by3;
	sy4=by4;
	while(1){
		if(moveblock(2)==0){
			break;
		}
	}
	if(by1>2){
		mvaddstr(by1,bx1+n,"#");
	}
	if(by2>2){
		mvaddstr(by2,bx2+n,"#");
	}
	if(by3>2){
		mvaddstr(by3,bx3+n,"#");
	}
	if(by4>2){
		mvaddstr(by4,bx4+n,"#");
	}
	block[by1][bx1]=0;
	block[by2][bx2]=0;
	block[by3][bx3]=0;
	block[by4][bx4]=0;
	k=bx1;
	bx1=sx1;
	sx1=k;
	k=bx2;
	bx2=sx2;
	sx2=k;
	k=bx3;
	bx3=sx3;
	sx3=k;
	k=bx4;
	bx4=sx4;
	sx4=k;
	k=by1;
	by1=sy1;
	sy1=k;
	k=by2;
	by2=sy2;
	sy2=k;
	k=by3;
	by3=sy3;
	sy3=k;
	k=by4;
	by4=sy4;
	sy4=k;
	createblock();
	}
	else{
	sy1=0;
	sy2=0;
	sy3=0;
	sy4=0;
	}
	dotx=dokx;
	doty=doky;					
	mvprintw(6,15+n,"s1 %2d %2d s2 %2d %2d s3 %2d %2d s4 %2d %2d dotx=%d doty=%d",sx1,sy1,sx2,sy2,sx3,sy3,sx4,sy4,dotx,doty);
}
void change(void){
	static int py1,py2,py3,py4,px1,px2,px3,px4;
	if(changenum>=1){
		mvprintw(7,4,"X");
		return;
	}
	static char chshape='N';
	char o;	
	px1=bx1;
	py1=by1;
	px2=bx2;
	py2=by2;
	px3=bx3;
	py3=by3;
	px4=bx4;
	py4=by4;
	removeblock();
	o=shape;
	if(chshape=='N'){
		changenum=-1;
		next();
		setblock(shape);
	}
	else{
		if(setblock(chshape)==1){
			gameover(0);
		}
	}
	shade(shadebot);
	chshape=o;
	mvprintw(9,3,"   ");
	mvprintw(10,2,"      ");
	switch(chshape){
		case 'O':
			mvprintw(9,3,"@@ ");    // 12@@
			mvprintw(10,2," @@   ");// 1 @@
			break;
		case 'I':
			mvprintw(10,2," @@@@ ");
			break;
		case 'S':
			mvprintw(9,3," @@");
			mvprintw(10,2," @@   ");
			break;
		case 'Z':
			mvprintw(9,3,"@@ ");   //12@@
			mvprintw(10,2,"  @@  ");//1 @@
			break;
		case 'L':
			mvprintw(9,3,"  @ ");
			mvprintw(10,2," @@@  ");
			break;
		case 'J':
			mvprintw(9,3,"@  ");
			mvprintw(10,2," @@@  ");
			break;
		case 'T':
			mvprintw(9,3," @ ");
			mvprintw(10,2," @@@  ");
			break;
	}			
	changenum++;
	return;
}
void down(int a){
		static int i;
		if(i<speed){
			i++;
			return;
		}
		if(i>=speed){
			i=0;
		}
		if(stopfalling==0){
		if(moveblock(2)==0){
			if(delin()==1){
				return;
			}
			next();
			if(setblock(shape)==1){
				gameover(0);
				return;
			}
			shade(3);
			shade(shadebot);
			changenum=0;
			mvprintw(7,4," ");
			refresh();
			return ;
			}
		shade(shadebot);
		refresh();
		return ;
		}
		return;
}
void next(void){
	static char nextblock='N';
	mvprintw(3,15+n,"                  ");
	mvprintw(4,15+n,"                  ");
	shape=nextblock;
	srand(time(NULL));
	srand(rand());
	int num;
	num=rand()%7;
	switch(num){
	case 0:
		nextblock='O';
		mvprintw(3,15+n,"               @@ ");
		mvprintw(4,15+n,"next block is  @@ ");
		break;
	case 1:
		nextblock='I';
		mvprintw(4,15+n,"next block is @@@@");
		break;
	case 2:
		nextblock='S';
		mvprintw(3,15+n,"               @@ ");
		mvprintw(4,15+n,"next block is @@  ");
		break;
	case 3:
		nextblock='Z';
		mvprintw(3,15+n,"              @@  ");
		mvprintw(4,15+n,"next block is  @@ ");
		break;
	case 4:
		nextblock='L';
		mvprintw(3,15+n,"                @ ");
		mvprintw(4,15+n,"next block is @@@ ");
		break;
	case 5:
		nextblock='J';
		mvprintw(3,15+n,"              @   ");
		mvprintw(4,15+n,"next block is @@@ ");
		break;
	case 6:
		nextblock='T';
		mvprintw(3,15+n,"               @  ");
		mvprintw(4,15+n,"next block is @@@ ");
		break;
	}
}
void gameover(int b){
	if(b==0){
	mvprintw(9,1+n,"----------");
	mvprintw(10,1+n,"Game Over!");
	mvprintw(11,1+n,"----------");
	mvprintw(12,1+n,"Any button");
	mvprintw(13,1+n,"To restart");
	mvprintw(14,1+n,"----------");
	}
	else if(b==1){
	mvprintw(9,1+n,"*--*--*--*");
	mvprintw(10,1+n,"100  lines");
	mvprintw(11,1+n,"Complete!!");
	mvprintw(12,1+n,"*--*--*--*");
	mvprintw(13,1+n,"Any button");
	mvprintw(14,1+n,"To restart");
	mvprintw(15,1+n,"*--*--*--*");
	}
	speed=40;
	stopfalling=1;
	getch();
	getch();	
	stopfalling=0;
	reset(1);
	mvprintw(8,1,"holding:");
	mvprintw(10,17+n,"press Z to hold");
	mvprintw(12,17+n,"press R to restart");
	mvprintw(14,17+n,"press S to hide/show shadow");
	mvprintw(16,17+n,"press Enter to PUASE");
	mvprintw(18,17+n,"LEVEL 1");
	mvprintw(20,17+n,"remove lines   0",lines);
	next();
	setblock(shape);
	shade(shadebot);
	//setitimer(ITIMER_REAL, &tick, NULL);
	//signal(SIGALRM, down);
	return;
}
int spinban(void){
	if(ban==3){///////////
		createblock();
		ban=0;
		next();
		if(setblock(shape)==1){
			gameover(0);
		}
		shade(3);
		shade(shadebot);
		changenum=0;
		mvprintw(7,4," ");	
		return 1;
	}
	return 0;
}

void gaming(void){
WINDOW *win;
win=newwin(5,22,10,n-5);
box(win,'|','-');
mvwaddstr(win,1,8,"PAUSE");
mvwaddstr(win,2,4,"Press enter to");
mvwaddstr(win,3,6,"continue...");
int i;	
int ch;
	while(1){
		mvprintw(7,15+n,"b1 %2d %2d b2 %2d %2d b3 %2d %2d b4 %2d %2d shmod=%d ban=%d",bx1,by1,bx2,by2,bx3,by3,bx4,by4,shmod,ban);
		ch=getch();             /* 等待自鍵盤輸入字元 */
			switch(ch) {           /* 判斷輸入字元為何 */
				case 114://restart
				break;
				case 117:
					if(doty>=3){
						upblock();
					}
				break;
				case 47:
					for(i=0;i<=2;i++){
						shmod--;
						if(shmod==0){
							shmod=4;
						}
					//	mvprintw(0,20,"pa%d",i);
						if(spin()==0){
							shade(shadebot);
						}
					}
				break;
				case KEY_UP:        /* 判斷是否"↑"鍵被按下 */
						//	mvprintw(0,20,"pa");i
					for(i=0;i<=2;i++){
						shmod--;
						if(shmod==0){
							shmod=4;
						}
						//	mvprintw(0,20,"pa%d",i);
						if(spin()==0){
							shade(shadebot);
						}
					}
				break;
				case KEY_DOWN:  /* 判斷是否"↓"鍵被按下 */
		//               mvprintw(0,0,"%d",moveblock(2));
					if(moveblock(2)==0){
						if(delin()==1){
						break;
						}
						next();
						if(setblock(shape)==1){
							gameover(0);
						}
						shade(3);
						shade(shadebot);
						changenum=0;
						mvprintw(7,4," ");
						break;
					}
					shade(shadebot);
				break;
				case KEY_RIGHT:               /* 判斷是否"→"鍵被按下 */
					moveblock(3);
					shade(shadebot);
				break;
				case KEY_LEFT:                /* 判斷是否"←"鍵被按下 */
					moveblock(1);
					shade(shadebot);
				break;
				case '\r':                    /* 判斷是否 ENTER 鍵被按下 */
					stopfalling=1;
						//signal(SIGALRM,SIG_IGN);
						//signal(SIGHUP,down);			
					touchwin(win);
					wrefresh(win);
					//	mvprintw(1,20,"here");
					refresh();
					int gh;
					while(1){		
						gh=getchar();
							if(gh=='\r'){
								touchwin(stdscr);
								stopfalling=0;
								break;
							}
							else if(gh==27){
								endwin();            /* 判斷是否[ESC]鍵被按下 */
								exit(1);
							}		
						}
				break;
				case 104:             
					stopfalling=abs(stopfalling-1);
				break;
				case 263:                     /* 判斷是否 BACKSPACE 鍵被按下 */
					mvaddch(y,--x,' ');/* delete 一個字元 */
				break;
				case 115:
					shadebot=abs(shadebot-1);
					shade(shadebot);
				break;
				case 32:
					while(1){
						if(moveblock(2)==0){
							if(delin()==1){
								break;
							}
							next();
							if(setblock(shape)==1){
								gameover(0);
							}
							shade(3);
							shade(shadebot);
							changenum=0;
							mvprintw(7,4," ");
							break;	
						}
					}
				break;
				case 27: //ESC
					endwin();
					exit(1);
				case 122:
					change();
				break;
				default:
					mvprintw(0,0,"%d",ch);
				break;
			}
		if(ch==114){
			reset(1);
			stopfalling=1;
			return;
		}			
	}
return;
}
