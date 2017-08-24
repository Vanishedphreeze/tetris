#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<ctime>
#include<string>
#include<conio.h>
#include<windows.h>
#include "textprint.h"
#include "shapes.h"

using namespace std;

#define WIDTH  10
#define HEIGHT 20

class TETRISBOARD {

/*
public functions:
    void CoverFromTopEffect(int SleepTimePerBlock);
    void CoverFromBottomEffect(int SleepTimePerBlock);
    void UncoverFromTopEffect(int SleepTimePerBlock);
    void gamestart();
    void gameinstantstart();
    void blockmove(int movx, int movy);
    void blockquickdown();
    void blockrotate();
    bool isgameover();
    int  getscore();
    void showmessage(LPTSTR _message, WORD _messagecolor); // strlen == 12
    void scorerefresh();
*/

    LPTSTR pattern[HEIGHT+5][WIDTH+5],nextpatternboard[4][4],PLAYERNAME;
    WORD board[HEIGHT+5][WIDTH+5],prevboard[HEIGHT+5][WIDTH+5];//for display
    WORD backup[HEIGHT+5][WIDTH+5];//for backup
    int x,y,blockNo,rot,nextblockNo;
    int XOFFSET,YOFFSET,XNEXTBOARDPOS,YNEXTBOARDPOS;
    WORD NEXTBOARDBGCOLOR;
    int score;
    string scoreshowup;
    bool _gameover;
    bool SCOREMASK=false;

    void scoreupdate() {
        int temp=score;
        for (int i=8;i>=1;i--) {
            scoreshowup[i]=(temp % 10) + '0';
            temp /= 10;
        }
    }

    void printnextboard() {
        WORD tempcolor[10];
        WORD tempblockcolor;
        char* temp2;

        temp2=(char*) scoreshowup.data();

        for (int i=0;i<10;i++) tempcolor[i]=NEXTBOARDBGCOLOR;
        textout(HSTDOUT,XNEXTBOARDPOS,YNEXTBOARDPOS    ,tempcolor,5,PLAYERNAME);
        textout(HSTDOUT,XNEXTBOARDPOS,YNEXTBOARDPOS + 1,tempcolor,5," SCORE    ");
        textout(HSTDOUT,XNEXTBOARDPOS,YNEXTBOARDPOS + 2,tempcolor,5,(LPTSTR) temp2);
        textout(HSTDOUT,XNEXTBOARDPOS,YNEXTBOARDPOS + 3,tempcolor,5,"          ");
        textout(HSTDOUT,XNEXTBOARDPOS,YNEXTBOARDPOS + 4,tempcolor,5," NEXT     ");

        tempblockcolor=NEXTBOARDBGCOLOR|block[nextblockNo][0].color;

        for (int i=0;i<4;i++) {
            textout(HSTDOUT,XNEXTBOARDPOS,YNEXTBOARDPOS + i + 5,&tempblockcolor,1," ");
            for (int j=0;j<4;j++) {
                textout(HSTDOUT,XNEXTBOARDPOS + j*2+1,YNEXTBOARDPOS + i + 5,&tempblockcolor,1,nextpatternboard[i][j]);
            }
            textout(HSTDOUT,XNEXTBOARDPOS + 9,YNEXTBOARDPOS + i + 5,&tempblockcolor,1," ");
        }
    }

    void printboard() {
        printnextboard();

        for (int i=0;i<=HEIGHT+1;i++) {
            for (int j=0;j<=WIDTH+1;j++) {
                textout(HSTDOUT,j*2+XOFFSET,i+YOFFSET,&board[i][j],1,pattern[i][j]);
            }
        }
    }

    void refreshnextboard() {
        WORD tempblockcolor;
        tempblockcolor=NEXTBOARDBGCOLOR|block[nextblockNo][0].color;

        for (int i=0;i<4;i++) {
            textout(HSTDOUT,XNEXTBOARDPOS,YNEXTBOARDPOS + i + 5,&tempblockcolor,1," ");
            for (int j=0;j<4;j++) {
                textout(HSTDOUT,XNEXTBOARDPOS + j*2+1,YNEXTBOARDPOS + i + 5,&tempblockcolor,1,nextpatternboard[i][j]);
            }
            textout(HSTDOUT,XNEXTBOARDPOS + 9,YNEXTBOARDPOS + i + 5,&tempblockcolor,1," ");
        }
    }

    void refreshboard() {
        for (int i=1;i<=HEIGHT;i++) {
            for (int j=1;j<=WIDTH;j++) if (prevboard[i][j]!=board[i][j]) {
                textout(HSTDOUT,j*2+XOFFSET,i+YOFFSET,&board[i][j],1,pattern[i][j]);
            }
        }

        for (int i=1;i<=HEIGHT;i++) {
            for (int j=1;j<=WIDTH;j++) prevboard[i][j]=board[i][j];
        }

        char* temp2;
        temp2=(char*) scoreshowup.data();
        WORD tempcolor[10];
        for (int i=0;i<10;i++) tempcolor[i]=NEXTBOARDBGCOLOR;
        textout(HSTDOUT,XNEXTBOARDPOS,YNEXTBOARDPOS + 2,tempcolor,5,(LPTSTR) temp2);
    }

    void getnextblock() {
        blockNo=nextblockNo;
        rot=0;
        x=0;
        y=4;
        nextblockNo=rand() % 7;
        for (int i=0;i<4;i++) {
            for (int j=0;j<4;j++) if (block[blockNo][rot].shape[i][j]) {
                if (backup[i+x][j+y]) _gameover=true;
                board[i+x][j+y]=block[blockNo][rot].color;
            }
        }

        for (int i=0;i<4;i++) {
            for (int j=0;j<4;j++) nextpatternboard[i][j]="  ";
        }

        for (int i=1;i<=2;i++) {
            for (int j=0;j<4;j++) if (block[nextblockNo][0].shape[i][j]==1) nextpatternboard[i][j]="¡ö";
        }
    }

    void remotelines() {
        int lines=0;
        for (int i=1;i<=HEIGHT;i++) {
            bool flag2=true;
            for (int j=1;j<=WIDTH;j++) if(backup[i][j]==0) flag2=false;
            if (flag2) {
                lines++;
                for (int k=i;k>1;k--) {
                    for (int j=1;j<=WIDTH;j++) backup[k][j]=backup[k-1][j];
                }
            }
        }
        if (lines==1) score+=100;
        else if (lines==2) score+=300;
        else if (lines==3) score+=600;
        else if (lines==4) score+=1000;

        for (int i=1;i<=HEIGHT;i++) {
            for (int j=1;j<=WIDTH;j++) board[i][j]=backup[i][j];
        }
    }

public:
    TETRISBOARD(int _xoffset=5, int _yoffset=3, int _xnextpos=50, int _ynextpos=10, LPTSTR _playername="PLAYER 1", bool _scoremask=false) {
        XOFFSET=_xoffset;
        YOFFSET=_yoffset;
        XNEXTBOARDPOS=_xnextpos;
        YNEXTBOARDPOS=_ynextpos;
        PLAYERNAME=_playername;
        NEXTBOARDBGCOLOR=BACKGROUND_INTENSITY;
        nextblockNo=rand() % 7;
        x=0;
        y=4;
        blockNo=0;
        rot=0;
        score=0;
        SCOREMASK=_scoremask;
        scoreshowup= SCOREMASK ? " ???????? ": " 00000000 ";
        _gameover=false;

        for (int i=0;i<=HEIGHT+4;i++) {
            for (int j=0;j<=WIDTH+4;j++) {
                backup[i][j]=prevboard[i][j]=board[i][j]=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
                pattern[i][j]="  ";
            }
        }

        for (int i=1;i<=HEIGHT;i++) {
            for (int j=1;j<=WIDTH;j++) {
                backup[i][j]=prevboard[i][j]=board[i][j]=0;
                pattern[i][j]="¡ö";
            }
        }

        for (int i=0;i<4;i++) {
            for (int j=0;j<4;j++) nextpatternboard[i][j]="  ";
        }

        for (int i=1;i<=2;i++) {
            for (int j=0;j<4;j++) if (block[nextblockNo][0].shape[i][j]==1) nextpatternboard[i][j]="¡ö";
        }
    }

    void blockmove(int movx, int movy)  {
        bool flag=true;
        for (int i=0;i<4;i++) {
            for (int j=0;j<4;j++) if (block[blockNo][rot].shape[i][j]) {
                if (backup[i+x+movx][j+y+movy]) flag=false;
            }
        }
        if (flag) {
            for (int i=1;i<=HEIGHT;i++) {
                for (int j=1;j<=WIDTH;j++) board[i][j]=backup[i][j];
            }
            x+=movx;
            y+=movy;
            for (int i=0;i<4;i++) {
                for (int j=0;j<4;j++) if (block[blockNo][rot].shape[i][j]) {
                    board[i+x][j+y]=block[blockNo][rot].color;
                }
            }
            refreshboard();
        }
        else if (movx==1) {
            for (int i=1;i<=HEIGHT;i++) {
                for (int j=1;j<=WIDTH;j++) backup[i][j]=board[i][j];
            }

            remotelines();
            if (!SCOREMASK) scoreupdate();
            getnextblock();
            refreshboard();
            refreshnextboard();
        }
    }

    void blockquickdown()  {
        for (int posx=1;posx<=HEIGHT;posx++) {
            bool flag=true;
            for (int i=0;i<4;i++) {
                for (int j=0;j<4;j++) if (block[blockNo][rot].shape[i][j]) {
                    if (backup[i+posx][j+y]) flag=false;
                }
            }

            if (!flag) {
                x=posx-1;
                for (int i=0;i<4;i++) {
                    for (int j=0;j<4;j++) if (block[blockNo][rot].shape[i][j]) {
                        backup[i+x][j+y]=block[blockNo][rot].color;
                    }
                }

                remotelines();
                if (!SCOREMASK) scoreupdate();
                getnextblock();
                refreshboard();
                refreshnextboard();
                return;
            }
        }
    }

    void blockrotate()  {
        bool flag=true;
        for (int i=0;i<4;i++) {
            for (int j=0;j<4;j++) if (block[blockNo][(rot+1) % 4].shape[i][j]) {
                if (backup[i+x][j+y]) flag=false;
            }
        }
        if (flag) {
            for (int i=1;i<=HEIGHT;i++) {
                for (int j=1;j<=WIDTH;j++) board[i][j]=backup[i][j];
            }
            rot=(rot+1) % 4;
            for (int i=0;i<4;i++) {
                for (int j=0;j<4;j++) if (block[blockNo][rot].shape[i][j]) {
                    board[i+x][j+y]=block[blockNo][rot].color;
                }
            }
            refreshboard();
        }
    }

    void CoverFromTopEffect(int SleepTimePerBlock) {
        WORD temp=7|FOREGROUND_INTENSITY;
        for (int i=1;i<=HEIGHT;i++) {
            for (int j=1;j<=WIDTH;j++) {
                textout(HSTDOUT,j*2+XOFFSET,i+YOFFSET,&temp,1,pattern[i][j]);
                Sleep(SleepTimePerBlock);
            }
        }
    }

    void CoverFromBottomEffect(int SleepTimePerBlock) {
        WORD temp=7|FOREGROUND_INTENSITY;
        for (int i=HEIGHT;i>=1;i--) {
            for (int j=WIDTH;j>=1;j--) {
                textout(HSTDOUT,j*2+XOFFSET,i+YOFFSET,&temp,1,pattern[i][j]);
                Sleep(SleepTimePerBlock);
            }
        }
    }

    void UncoverFromTopEffect(int SleepTimePerBlock) {
        for (int i=1;i<=HEIGHT;i++) {
            for (int j=1;j<=WIDTH;j++) {
                textout(HSTDOUT,j*2+XOFFSET,i+YOFFSET,&board[i][j],1,pattern[i][j]);
                Sleep(SleepTimePerBlock);
            }
        }
    }

    void gamestart() {
        printboard();
        CoverFromTopEffect(15);
        Sleep(500);
        UncoverFromTopEffect(15);
        getnextblock();
        refreshboard();
        refreshnextboard();
    }

    void gameinstantstart() {
        printboard();
        CoverFromTopEffect(0);
        UncoverFromTopEffect(0);
        getnextblock();
        refreshboard();
        refreshnextboard();
    }

    void showmessage(LPTSTR _message, WORD _messagecolor) {
        WORD temp=_messagecolor;
        textout(HSTDOUT,XOFFSET+6,YOFFSET+7,&temp,1,_message);
    }

    void scorerefresh() {
        scoreupdate();
        char* temp2;
        temp2=(char*) scoreshowup.data();
        WORD tempcolor[10];
        for (int i=0;i<10;i++) tempcolor[i]=NEXTBOARDBGCOLOR;
        textout(HSTDOUT,XNEXTBOARDPOS,YNEXTBOARDPOS + 2,tempcolor,5,(LPTSTR) temp2);
    }

    bool isgameover() { return _gameover; }

    int getscore() { return score; }
};
