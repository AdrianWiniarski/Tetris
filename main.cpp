// BARTOSZ BICZ
// ADRIAN WINIARSKI
// EKRAN PODZIELONY NA 4 CZESCI 
// PIERWSZA CZESC - SPADAJACE KLOCKI JAK TETRIS
// RESZTA CWIARTEK PRZECHWYTUJE I WYPISUJE FIGURY W SWOICH CWIARTKACH

#include "stdafx.h"
#include "include/curses.h"
#include <vector>
#include <time.h>
#include <thread>
#include <windows.h>
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;
int max_ogon = 5;
mutex mx;
condition_variable cv;
class Klocek
{
public:
	int x, y;
};
class Figura
{
	public:
		Klocek klooce[4];
};
vector<Figura> figury;
int x;
class Waz
{
public: bool gameOver = false;
		Figura figura;
		Klocek klocek1[4], klocek2[4], klocek3[4], klocek4[4], klocek5[4];
		int minW, maxW, minS, maxS; //min i max wysokosc/szerokosc, narozniki pola ruchu weza
		
		Figura tablicaFigur[6];
		
		
		Waz::Waz() {}
		
		Waz(int a, int b, int c, int d)
		{
			klocek1[0].x = 4;
			klocek1[0].x = 5;
			klocek1[0].x = 6;
			klocek1[0].x = 7;
			klocek1[0].y = 0;
			klocek1[1].y = 0;
			klocek1[2].y = 0;
			klocek1[3].y = 0;//Tworzenie figur do losowania

			//tworzenie drugiej figury do pozniejszego losowania

			minW = a;
			maxW = b;
			minS = c;
			maxS = d;
			figura.klooce[0].x = c + 19;
			figura.klooce[0].y = a + 1;
			figura.klooce[1].x = figura.klooce[0].x;
			figura.klooce[1].y = figura.klooce[0].y - 1;
			figura.klooce[2].x = figura.klooce[1].x;
			figura.klooce[2].y = figura.klooce[1].y-1;
			figura.klooce[3].x = figura.klooce[0].x+1;
			figura.klooce[3].y = figura.klooce[2].y;
		}
		void nowyKlocek()
		{
			srand(time(NULL));
			int los = rand();
			int przesuniecie = los % 35;
			if (los % 6 == 0)
			{
				figura.klooce[0].x = 0+przesuniecie;
				figura.klooce[0].y = -2;
				figura.klooce[1].x = figura.klooce[0].x + 1;
				figura.klooce[1].y = figura.klooce[0].y;
				figura.klooce[2].x = figura.klooce[1].x + 1;//figura1
				figura.klooce[2].y = figura.klooce[1].y;
				figura.klooce[3].x = figura.klooce[2].x + 1;
				figura.klooce[3].y = figura.klooce[2].y;
			}
			else if(los%6==1)
			{
				figura.klooce[0].x = 0+przesuniecie;
				figura.klooce[0].y = 0;
				figura.klooce[1].x = figura.klooce[0].x;
				figura.klooce[1].y = figura.klooce[0].y+1;
				figura.klooce[2].x = figura.klooce[1].x;
				figura.klooce[2].y = figura.klooce[1].y+1;//figura2
				figura.klooce[3].x = figura.klooce[2].x;
				figura.klooce[3].y = figura.klooce[2].y+1;
			}
			else if (los % 6 == 2)
			{
				figura.klooce[0].x = 0 + przesuniecie;
				figura.klooce[0].y = 0;
				figura.klooce[1].x = figura.klooce[0].x+1;
				figura.klooce[1].y = figura.klooce[0].y;
				figura.klooce[2].x = figura.klooce[1].x;
				figura.klooce[2].y = figura.klooce[1].y + 1;//figura3
				figura.klooce[3].x = figura.klooce[2].x;
				figura.klooce[3].y = figura.klooce[2].y + 1;
			}
			else if (los % 6 == 3)
			{
				figura.klooce[0].x = 0 + przesuniecie;
				figura.klooce[0].y = 0;
				figura.klooce[1].x = figura.klooce[0].x + 1;
				figura.klooce[1].y = figura.klooce[0].y;
				figura.klooce[2].x = figura.klooce[1].x;
				figura.klooce[2].y = figura.klooce[1].y + 1;//figura4
				figura.klooce[3].x = figura.klooce[2].x+1;
				figura.klooce[3].y = figura.klooce[2].y;
			}
			else if (los % 6 == 4)
			{
				figura.klooce[0].x = 0 + przesuniecie;
				figura.klooce[0].y = 0;
				figura.klooce[1].x = figura.klooce[0].x+1;
				figura.klooce[1].y = figura.klooce[0].y;
				figura.klooce[2].x = figura.klooce[1].x;//figura5
				figura.klooce[2].y = figura.klooce[1].y+1;
				figura.klooce[3].x = figura.klooce[2].x-1;
				figura.klooce[3].y = figura.klooce[2].y;
			}
			else if (los % 6 == 5)
			{
				figura.klooce[0].x = 0 + przesuniecie;
				figura.klooce[0].y = 0;
				figura.klooce[1].x = figura.klooce[0].x;
				figura.klooce[1].y = figura.klooce[0].y+1;
				figura.klooce[2].x = figura.klooce[1].x;//figura6
				figura.klooce[2].y = figura.klooce[1].y + 1;
				figura.klooce[3].x = figura.klooce[2].x+1;
				figura.klooce[3].y = figura.klooce[2].y;
			}
			
			
		}
		void snake()
		{

	
			for (int i = minW; i < maxW; i++)
			{
				for (int j = minS; j < maxS; j++)
				{

					if ((i == figura.klooce[0].y && j == figura.klooce[0].x)||(i == figura.klooce[1].y && j == figura.klooce[1].x) || (i == figura.klooce[2].y && j == figura.klooce[2].x) || (i == figura.klooce[3].y && j == figura.klooce[3].x))
					{
						move(i, j);
						addch('@');
					}
					else
					{
						move(i, j);
						addch(' ');
					}
				}

			}
		}

		void silnik_gry()
		{
			if ((maxW > figura.klooce[0].y+1)&&maxW>figura.klooce[3].y+1)
			{
				x = 0;
				figura.klooce[0].y++;
				figura.klooce[1].y++;
				figura.klooce[2].y++;
				figura.klooce[3].y++;
			}
			else
			{
				figury.push_back(figura);
				x = 1;//CV NOTIFY ALL
				cv.notify_all();
				nowyKlocek(); 
			}
		}
		void pobierzFigure()
		{
			if (figury.size() > 1)
			{
				figura.klooce[0].x = figury.front().klooce[0].x;
				figura.klooce[1].x = figury.front().klooce[0].x;
				figura.klooce[2].x = figury.front().klooce[0].x;
				figura.klooce[3].x = figury.front().klooce[0].x;
				figura.klooce[0].y = figury.front().klooce[0].y;
				figura.klooce[1].y = figury.front().klooce[1].y;
				figura.klooce[2].y = figury.front().klooce[2].y;
				figura.klooce[3].y = figury.front().klooce[3].y;
			}
		}
		void pop_front()
		{
			if (figury.size() > 1)
			{
				Figura fig;
				fig = figury.back();
				figury.back() = figury.front();
				figury.front() = fig;
				figury.pop_back();
				for (int i = 0; i < figury.size() - 1; i++)
			{
				fig = figury[i];
				figury[i] = figury[i + 1];
				figury[i + 1] = fig;
			}
		}
		}
		void wyswietl()
		{
			if (figury.size() > 1)
			{
				srand(time(NULL));
				int losowanie = rand() % 8;
				int size = figury.size() - 1;
				for (int i = minW; i < maxW; i++)
				{
					for (int j = minS; j < maxS; j++)
					{
						if ((i == figury.front().klooce[0].y + minW && (j == figury.front().klooce[0].x + minS)) || (i == figury.front().klooce[1].y + minW && (j == figury.front().klooce[1].x + minS)))
						{
							move((i - losowanie), j);
							addch('X');
						}
						else if ((i == figury.front().klooce[2].y + minW && (j == figury.front().klooce[2].x + minS)) || (i == figury.front().klooce[3].y + minW && (j == figury.front().klooce[3].x + minS)))
						{
							move(i - losowanie, j);
							addch('X');
						}
					}
				}
				for (int i = 0; i < 4; i++)
				{
					figura.klooce[i].x = 999;
					figura.klooce[i].y = 998;
				}
			}
		}
		void czekajNaFigure()
		{
			for (;;)
			{
					
						unique_lock<mutex>  lock(mx);
						cv.wait(lock);
						pobierzFigure();
						wyswietl();
						pop_front();
					
			
			}
		}
		int graj()
		{
			for (;;)
			{
				snake();
				wrefresh(stdscr);
				silnik_gry();
				Sleep(100);
			}
			return 0;
		}

		
};

int main(void)
{
	initscr();
	int szer;
	int wys;
	curs_set(0);
	srand(time(NULL));
	getmaxyx(stdscr, wys, szer);


	Waz waz1(0, wys / 2, 0, szer / 2);
	Waz waz2(0, wys / 2, szer / 2 + 1, szer - 1);
	Waz waz3(wys / 2 + 1, wys, 0, szer / 2);
	Waz waz4(wys / 2 + 1, wys, szer / 2 + 1, szer);

	for (int i = 0; i<szer / 3 - 1; i++) {
		move(i, szer / 2);
		addch('#');
	}

	for (int i = 0; i<szer - 1; i++) {
		move(wys / 2, i);
		addch('#');
	}
	
		//waz1.graj();
		//refresh();
			thread watek1(&Waz::graj, &waz1);
			thread watek2(&Waz::czekajNaFigure, &waz2);
			thread watek3(&Waz::czekajNaFigure, &waz3);
			thread watek4(&Waz::czekajNaFigure, &waz4);
			watek1.join();
			watek2.join();
			watek3.join();
			watek4.join();
			
		


	int c;

	cin >> c;


	return 0;
}



