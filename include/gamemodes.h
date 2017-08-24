#include<ctime>
#include<iostream>
#include<windows.h>
#include<conio.h>
#include "tetrisboard.h"

#define UP      72
#define LEFT    75
#define RIGHT   77
#define DOWN    80


int highscore=10000;
string highscoreshowup="HISCORE 00000000";

void highscoreupdate() {
    int temp=highscore;
    for (int i=15;i>=8;i--) {
        highscoreshowup[i]=(temp % 10) + '0';
        temp /= 10;
    }
}

void singlemode () {
    int XHIGHSCOREPOS=29;
    int YHIGHSCOREPOS=1;
    WORD HIGHSCORECOLOR=7;
    DWORD stamp,stamppause,DELAY,LEVEL;
    char opt;

    system("cls");
    TETRISBOARD player1(5,3,32,4,"PLAYER1");
    DELAY=1000;
    highscoreupdate();
    char* temp;
    temp=(char*) highscoreshowup.data();
    textout(HSTDOUT,XHIGHSCOREPOS,YHIGHSCOREPOS,&HIGHSCORECOLOR,1,(LPTSTR) temp);
    player1.gamestart();

    stamp=GetTickCount();
    while (true) {
        if (kbhit()) {
            opt=getch();
            if (opt=='j') player1.blockrotate();
            else if (opt=='w') {
                player1.blockquickdown();
                stamp=GetTickCount();
            }
            else if (opt=='a') player1.blockmove(0,-1);
            else if (opt=='d') player1.blockmove(0,1);
            else if (opt=='s') {
                player1.blockmove(1,0);
                stamp=GetTickCount();
            }
            else if (opt=='p') {
                stamppause=GetTickCount();
                player1.CoverFromTopEffect(0);
                player1.showmessage("    PAUSE   ",7);
                getch();
                player1.UncoverFromTopEffect(0);
                stamp+=GetTickCount()-stamppause;
            }
        }

        if (player1.isgameover()) {
            player1.CoverFromBottomEffect(5);
            player1.showmessage(" GAME  OVER ",7);
            while (opt!='\r') opt=getch();
            break;
        }

        if (GetTickCount()-stamp>=DELAY) {
            player1.blockmove(1,0);
            stamp=GetTickCount();
        }

        if (player1.getscore() / 1000 > LEVEL) {
            LEVEL=player1.getscore() / 1000;
            DELAY=DELAY * 2 / 3;
            if (DELAY<=15) DELAY=15;
        }

        if (player1.getscore() > highscore) {
            highscore=player1.getscore();
            highscoreupdate();

            char* temp2;
            temp2=(char*) highscoreshowup.data();
            textout(HSTDOUT,XHIGHSCOREPOS,YHIGHSCOREPOS,&HIGHSCORECOLOR,1,(LPTSTR) temp2);
        }
    }
}

void vsmode () {
    int XHIGHSCOREPOS=29;
    int YHIGHSCOREPOS=1;
    bool p1hangup=false,p2hangup=false;
    WORD HIGHSCORECOLOR=7;
    DWORD p1stamp,p2stamp,DELAY,LEVEL;
    char opt;

    system("cls");
    TETRISBOARD player1(5,3,32,4,"PLAYER 1",1),player2(45,3,32,14,"PLAYER 2",1);
    DELAY=100;
    highscoreupdate();
    char* temp;
    temp=(char*) highscoreshowup.data();
    textout(HSTDOUT,XHIGHSCOREPOS,YHIGHSCOREPOS,&HIGHSCORECOLOR,1,(LPTSTR) temp);
    player1.gameinstantstart();
    player2.gameinstantstart();
    player1.showmessage("   READY    ",7);
    player2.showmessage("    READY   ",7);
    Sleep(3000);
    player1.showmessage("            ",7);
    player2.showmessage("            ",7);

    p2stamp=p1stamp=GetTickCount();
    while (true) {
        if (kbhit()) {
            opt=getch();
            //player 1 operation
            if (!p1hangup) {
                if (opt=='j') player1.blockrotate();
                else if (opt=='w') {
                    player1.blockquickdown();
                    p1stamp=GetTickCount();
                }
                else if (opt=='a') player1.blockmove(0,-1);
                else if (opt=='d') player1.blockmove(0,1);
                else if (opt=='s') {
                    player1.blockmove(1,0);
                    p1stamp=GetTickCount();
                }
            }

            // player 2 operation
            if (!p2hangup) {
                if (opt=='0') player2.blockrotate();
                else if (opt==UP) {
                    player2.blockquickdown();
                    p2stamp=GetTickCount();
                }
                else if (opt==LEFT) player2.blockmove(0,-1);
                else if (opt==RIGHT) player2.blockmove(0,1);
                else if (opt==DOWN) {
                    player2.blockmove(1,0);
                    p2stamp=GetTickCount();
                }
            }
        }

        if (!p1hangup) {
            if (player1.isgameover()) {
                p1hangup=true;
                player1.CoverFromBottomEffect(0);
                player1.showmessage("PLEASE  WAIT",7);
            }

            if (GetTickCount()-p1stamp>=DELAY) {
                player1.blockmove(1,0);
                p1stamp=GetTickCount();
            }

            if (player1.getscore() > highscore) {
                highscore=player1.getscore();
                highscoreupdate();
            }
        }

        if (!p2hangup) {
            if (player2.isgameover()) {
                p2hangup=true;
                player2.CoverFromBottomEffect(0);
                player2.showmessage("PLEASE  WAIT",7);
            }

            if (GetTickCount()-p2stamp>=DELAY) {
                player2.blockmove(1,0);
                p2stamp=GetTickCount();
            }

            if (player2.getscore() > highscore) {
                highscore=player2.getscore();
                highscoreupdate();
            }
        }

        if (p1hangup && p2hangup) {
            Sleep(2000);
            player1.scorerefresh();
            player2.scorerefresh();
            char* temp2;
            temp2=(char*) highscoreshowup.data();
            textout(HSTDOUT,XHIGHSCOREPOS,YHIGHSCOREPOS,&HIGHSCORECOLOR,1,(LPTSTR) temp2);

            if (player1.getscore()>player2.getscore()) {
                player1.showmessage("  YOU  WIN  ",7);
                player2.showmessage("  YOU  LOSE ",7);
            }
            else if (player1.getscore()<player2.getscore()) {
                player1.showmessage("  YOU  LOSE ",7);
                player2.showmessage("  YOU  WIN  ",7);
            }
            else if (player1.getscore()==player2.getscore()) {
                player1.showmessage("    DRAW    ",7);
                player2.showmessage("    DRAW    ",7);
            }
            while (opt!='\r') opt=getch();
            break;
        }
    }
}
