#include<windows.h>
#include<conio.h>

char title[10][80]={
"##############  ########## #############  #########     ######     ####### ",
"##############  ########## #############  ###########   ######   ##########",
"##############  ####       #############  ####   #####  ######  #####  ####",
"     ####       ####           ####       ####    ####   ####   ####       ",
"     ####       #########      ####       ####   #####   ####   ########   ",
"     ####       #########      ####       ##########     ####    ######### ",
"     ####       ####           ####       ##########     ####        ######",
"     ####       ####           ####       ####   ####    ####   ##     ####",
"     ####       ##########     ####       ####    ####  ######  ###########",
"     ####       ##########     ####       ####     #### ######   ######### "
};

int maintitle() {
    WORD TITLECOLOR=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY|BACKGROUND_BLUE;
    WORD fcolor1=7|FOREGROUND_INTENSITY|BACKGROUND_INTENSITY;
    WORD fcolor2=7|FOREGROUND_INTENSITY;
    int XTITLEPOS=28;
    int YTITLEPOS=14;
    int XHIGHSCOREPOS=28;
    int YHIGHSCOREPOS=19;
    LPTSTR optiontext[3]={"1 PLAYER MODE","2 PLAYERS VS MODE","EXIT"};
    for (int i=0;i<10;i++) textout(HSTDOUT,0,i,&TITLECOLOR,1,title[i]);
    char opt;
    int highlight=0;

    highscoreupdate();
    char* temp2;
    temp2=(char*) highscoreshowup.data();
    textout(HSTDOUT,XHIGHSCOREPOS,YHIGHSCOREPOS,&fcolor2,1,(LPTSTR) temp2);

    for (int i=0;i<3;i++) {
        if (i==highlight) textout(HSTDOUT,XTITLEPOS,YTITLEPOS + i,&fcolor1,1,optiontext[i]);
        else textout(HSTDOUT,XTITLEPOS,YTITLEPOS + i,&fcolor2,1,optiontext[i]);
    }


    while (true) {
        if (kbhit()) {
            opt=getch();
            if (opt==UP || opt=='w') {
                highlight=(highlight+2) % 3;
            }
            else if (opt==DOWN || opt=='s') {
                highlight=(highlight+1) % 3;
            }
            else if (opt=='\r') return highlight;

            for (int i=0;i<3;i++) {
                if (i==highlight) textout(HSTDOUT,XTITLEPOS,YTITLEPOS + i,&fcolor1,1,optiontext[i]);
                else textout(HSTDOUT,XTITLEPOS,YTITLEPOS + i,&fcolor2,1,optiontext[i]);
            }
        }
    }
}
