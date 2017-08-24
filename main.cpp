#include<ctime>
#include<iostream>
#include<windows.h>
#include<conio.h>
#include "include/gamemodes.h"
#include "include/title.h"

#define UP      72
#define LEFT    75
#define RIGHT   77
#define DOWN    80

int main () {
    srand((unsigned) time(0));
    while (true) {
        system("cls");
        int temp=maintitle();
        if (temp==0) singlemode();
        else if (temp==1) vsmode();
        else if (temp==2) break;
    }

    return 0;
}
