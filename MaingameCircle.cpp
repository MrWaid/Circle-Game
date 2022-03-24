#include "TXLib.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

#define M 800
#define N 600

void drawField(int field[M / 50][N / 50])
{
	int i, j;
	for (i = 0; i < 16; i ++)
	{
		txSetColor(RGB(0, 0, 0));
		txLine(i * 50, 0 * 50, i * 50, N);
	}
	for (i = 0; i < 12; i ++)
	{
		txSetColor(RGB(0, 0, 0));
		txLine(0, i * 50, M, i * 50);
	}
	for (i = 0; i < 16; i ++)
	{
		for (j = 0; j < 12; j ++)
		{
			if (field[i][j] == 2) txSetFillColor(RGB(255,   0,   0)); //начало
			if (field[i][j] == 3) txSetFillColor(RGB(  0,   0, 255)); //конец
			if (field[i][j] == 0) txSetFillColor(RGB(255, 255, 255)); //поле
			if (field[i][j] == 1) txSetFillColor(RGB(  0,   0,   0)); //стена
			txSetColor(RGB(0, 0, 0));
			txRectangle(i * 50, j * 50, (i + 1) * 50, (j + 1) * 50);
		}
	}
}

int main()
{
	srand (time(0));

	int a[M / 50][N / 50];
	char str[10];
	int i, j, is_game_on = 0, m2 = 0, q, w;
	double x_circle, y_circle, speed_x = 0, speed_y = 0, t = 0;

	time_t start_time, current_time;

	for (i=0; i<16; i++)
	{
		for (j=0; j<12; j++)
		{
			a[i][j]=0;//rand()%2;
		}
	}

	txCreateWindow(M, N);

	while(!GetAsyncKeyState(VK_ESCAPE))
	{
		if (!is_game_on)
		{
			int kol_vo_red = 0, kol_vo_blue = 0;
			int mx = txMouseX();
			int my = txMouseY();
			for (i = 0; i < 16; i ++)
			{
				for (j = 0; j < 12; j ++)
				{
					if (a[i][j] == 2)
					{
						kol_vo_red += 1;
					}
					if (a[i][j] == 3)
					{
						kol_vo_blue += 1;
					}
				}
			}

			i = (mx - mx % 50) / 50;
			j = (my - my % 50) / 50;

			if (txMouseButtons() & 1)
			{
				if (GetAsyncKeyState(VK_CONTROL))
				{
					if (!kol_vo_red) a[i][j] = 2;
				}
				else a[i][j] = 1;
			}

			if (txMouseButtons() & 2)
			{
				if (GetAsyncKeyState(VK_CONTROL))
				{
					if (!kol_vo_blue) a[i][j] = 3;
				}
				else a[i][j] = 0;
			}


			txBegin();

			txSetFillColor(TX_WHITE);
			txClear();
			drawField(a);

			txEnd();

			if (GetAsyncKeyState('L'))
			{
				std::fstream fs;
				fs.open ("input.txt", std::fstream::out);
				for (i = 0; i < 16; i ++)
				{
					for (j = 0; j < 12; j ++)
					{
					fs << a[i][j] << " ";
					}
					fs << "\n";
				}
				fs.close();
			}
			if (GetAsyncKeyState('S'))
			{
				std::fstream fs;
				fs.open ("input1.txt", std::fstream::in);
				for (i = 0; i < 16; i ++)
				{
					for (j = 0; j <12; j ++)
					{
						fs >> a[i][j];
					}
				}
				fs.close();
			}

			if (GetAsyncKeyState(VK_RETURN))
			{
				start_time = time(0);
				is_game_on = 1;
			}

		}

		else
		{
			int mas_i = 0, mas_j = 0;

			txBegin();

			txSetFillColor(RGB(255, 255, 255));
			txClear();
			drawField(a);
			txSetFillColor(RGB(0, 255, 0));
			for (i = 0; i < 16; i ++)
			{
				for (j = 0; j < 12; j ++)
				{
					if (a[i][j] == 2)
					{
						q = i;
						w = j;
						if (m2 == 0)
						{
							x_circle = i * 50 + 25;
							y_circle = j * 50 + 25;
							m2 = 1;
						}
					}
				}
			}
			txCircle(x_circle, y_circle, 5);
			speed_x *= 0.98;
			speed_y *= 0.98;
			x_circle += speed_x;
			y_circle += speed_y;

			mas_i = (x_circle) / 50;
			mas_j = (y_circle) / 50;

			double acc_x = 0.0, acc_y = 0.0;
			double acc = 0.1;

			if (GetAsyncKeyState(VK_UP)) acc_y -= acc;
			if (GetAsyncKeyState(VK_DOWN)) acc_y += acc;
			if (GetAsyncKeyState(VK_LEFT)) acc_x -= acc;
			if (GetAsyncKeyState(VK_RIGHT)) acc_x += acc;

			speed_x += acc_x;
			speed_y += acc_y;
			txSetColor(RGB(0, 255, 0));
			current_time = time(0);
			sprintf(str, "%d", current_time - start_time);
			txTextOut (10, 10, str);

			txEnd();
			if (a[mas_i][mas_j] == 1)
			{
				x_circle = q * 50 + 25;
				y_circle = w *50 + 25;
				speed_x = 0;
				speed_y = 0;
				while (!GetAsyncKeyState(VK_RETURN))
				{
					txSetColor(RGB(0, 255, 0));
					txTextOut(M / 2 - 30, N / 2, "Бум!");
					if (GetAsyncKeyState(VK_ESCAPE)) return 0;
					start_time = time(0);
				}
			}
			if (a[mas_i][mas_j] == 3)
			{
				while (!GetAsyncKeyState(VK_RETURN))
				{
					x_circle = q * 50 + 25;
					y_circle = w *50 + 25;
					speed_x = 0;
					speed_y = 0;
					txSetColor(RGB(0, 255, 0));
					txTextOut(M / 2 - 100, N / 2, "Вы победили!!!!!!!!!!!!!!!!!!!!!");
					if (GetAsyncKeyState(VK_ESCAPE)) return 0;
					start_time = time(0);
				}
			}
			if (x_circle > M || x_circle < 0)
			{
				x_circle = q * 50 + 25;
				y_circle = w *50 + 25;
				speed_x = 0;
				speed_y = 0;
				while (!GetAsyncKeyState(VK_RETURN))
				{
					txSetColor(RGB(0, 255, 0));
					txTextOut(M / 2 - 30, N / 2, "Бум!");
					if (GetAsyncKeyState(VK_ESCAPE)) return 0;
					start_time = time(0);
				}
			}
			if (y_circle < 0 || y_circle > N)
			{
				x_circle = q * 50 + 25;
				y_circle = w *50 + 25;
				speed_x = 0;
				speed_y = 0;
				while (!GetAsyncKeyState(VK_RETURN))
				{
					txSetColor(RGB(0, 255, 0));
					txTextOut(M / 2 - 30, N / 2, "Бум!");
					if (GetAsyncKeyState(VK_ESCAPE)) return 0;
					start_time = time(0);
				}
			}

		}


	}

    return 0;
}
