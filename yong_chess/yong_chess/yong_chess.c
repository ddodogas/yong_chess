/*
	chess 안에 chessboard, players가 있어서 체스판과 플레이어들의 말들의 정보들이 들어있음.
	chessboard와 players는 따로따로 이며, chessboard의 값들을 변경후 players와 동기화 시켜주는 방식.
	0~15 백팀 말, 20~23 프로모션으로 새로생겨난 백팀 말.
	100~114 흑팀 말, 120~123 프로모션으로 새로생겨난 흑팀 말.
	players 안에 pieces는 rank(행), file(열), life 정보가 들어있음.
	life는 0(죽었을 경우), 1(살았을 경우), 11(앙파상 가능성 있는 폰, 한번이라도 움직인 룩,킹) 으로 했음.
	말이 선택 됬을때 움직일수 있는곳에 1000을 더해주어서 표시를 해줌.
	swap - chessboard의 값들을 바꿔줌. 빈칸으로 가는경우와 상대편 말을 먹는 경우가 있음.
	m_thset - 1000이 더해진 경우 1000을 다시 빼줌.
	sync - chessboard와 players 정보 동기화 시켜줌.
	deadshow - life가 0인 말들 보여줌.
	initialize - chessboard에 각말들 값들 넣어주고, players에 rank, file, life 정보 넣어줌.
	show - chessboard의 각 값에 맞게 알파벳을 띄워줌, 흑팀은 색깔을 다르게, 선택된곳, 움직일수 있는 곳 색깔을 다르게
		   보여줌. 체크, 체크메이트, 스테일메이트도 여기서 확인함.
	again - 게임이 끝났을때 다시할지, 끝낼지 확인함.
	rook ~ pawn - 움직일수 있는 곳에 1000을 더해줌.
	promotion - 폰이 chessboard의 끝에 왔는지 확인해서 프로모션 해줌.
	enpassant - 앙파상 가능성 있는 말(life==11) 인 폰 옆에 상대 폰이 있으면 앙파상 룰 적용.
	castling - 킹, 룩이 한번도 안움직였고, 중간에 상대편 말의 공격, 빈 공간이면 캐슬링 룰 적용.
	checkattackk - 어떤 말이 어떤 공간을 공격할수 있는지 확인해줌.
	check - 체크상태인지 확인해줌
	checkmate -
	checkmate_check - 우리편 말을 1개 움직이는 모든 경우의 수를 계산해서 체크가 아닌 경우가 있는지 확인함.
*/
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

//rank(행), file(열)
typedef struct
{
	int rank, file;
	int life;
} piece;

//player마다 장기말들 장기판 모양대로 백 - 0:l ~ 15:p 흑 - 100:l ~ 115:p 빈곳 - 16
typedef struct
{
	piece pieces[16];
} player;

//체스판, players[0]=백, [1]=흑
typedef struct
{
	int chessboard[8][8];
	player players[2];
} chess;

void swap(chess*, int*, int*, int*, int*); //말의 실제 이동 함수
void m_thset(chess*); //1000더해진곳을 다시 빼주는 함수
void sync(chess*); //chessboard와 players의 rank,file을 동기화 시켜주는 함수
void deadshow(chess*); //죽은 말을 보여주는 함수

void initialize(chess*); //초기 세팅 함수
int show(chess*, int*, int*, int); //출력 함수
int key(chess*, int*, int*, int*, int*, int); //키 입력 함수
int again(); //게임이 끝났을때 다시 할지 확인

void rook(chess*, int*, int*); //말 이동함수들
void night(chess*, int*, int*);
void bishop(chess*, int*, int*);
void king(chess*, int*, int*, int);
void queen(chess*, int*, int*);
void pawn(chess*, int*, int*, int);

void promotion(chess*); //프로모션
void enpassant(chess*, int*, int*); //앙파상
void castling(chess*, int*, int*); //캐슬링
int checkattack(chess, int, int, int); //공격 범위에 있나 확인 함수
int check(chess, int); //체크 확인
int checkmate(chess, int); //체크메이트
int checkmate_check(chess, int);

int main()
{
regame:
	{
		int k = 7, l = 7; //현재선택된 인덱스[k][l]
		int sk, sl; //플레이어가 선택한 말의 인덱스[sk][sl]
		int flag = 0; //백, 흑 번갈아가면서 하는지 확인하는 플래그
		chess game;
		initialize(&game);
		while (1)
		{
			if (show(&game, &k, &l, flag) != 0)
			{
				printf("\n게임이 끝났습니다!\n");
				if (again() == 1)
				{
					goto regame;
				}
				else
				{
					return 0;
				}
			}
			if (key(&game, &k, &l, &sk, &sl, flag) == 1)
			{
				flag = flag ? 0 : 1;
			}
			system("cls");
		}
	}
}

void swap(chess* _game, int* _attack, int* _defense, int* _k, int* _l)
{
	int confirm = (*_defense) % 100;
	if (confirm == 16)
	{
		int temp = *_attack;
		*_attack = *_defense;
		*_defense = temp;
		enpassant(_game, _k, _l); //말을 죽인게 아니면, 앙파상 확인
	}
	else
	{
		*_defense = *_attack;
		*_attack = 16;
	}
	return;
}

void m_thset(chess* _game)
{
	int i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (_game->chessboard[i][j] >= 1000)
			{
				_game->chessboard[i][j] -= 1000;
			}
		}
	}
	return;
}

void sync(chess* _game)
{
	int i, j, k, l, temp, flag = 0;
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 16; j++)
		{
			temp = j;
			if (i == 1)
			{
				temp += 100;
			}
			if ((_game->chessboard)[(_game->players)[i].pieces[j].rank][(_game->players)[i].pieces[j].file] == temp)
			{
				continue;
			}
			else
			{
				for (k = 0; k < 8; k++)
				{
					for (l = 0; l < 8; l++)
					{
						if ((_game->chessboard)[k][l] == temp)
						{
							(_game->players[i]).pieces[j].rank = k;
							(_game->players[i]).pieces[j].file = l;
							if (j == 0 || j == 4 || j == 7)
							{
								(_game->players)[i].pieces[j].life += 10;
							}
							flag++;
						}
					}
				}
				if (flag == 0)
				{
					(_game->players[i]).pieces[j].life = 0;
				}
				flag = 0;
			}
		}
	}
}

void deadshow(chess* _game)
{
	int i, j;
	for (i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			printf("백팀 죽은 말 : ");
		}
		else
		{
			printf("\033[36m");
			printf("흑팀 죽은 말 : ");
		}
		for (j = 0; j < 16; j++)
		{
			if ((_game->players)[i].pieces[j].life == 0)
			{
				switch (j)
				{
				case 0:
				case 7:printf("R "); break;
				case 1:
				case 6:printf("N "); break;
				case 2:
				case 5:printf("B "); break;
				case 3:printf("Q "); break;
				case 4:printf("K "); break;
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:printf("P "); break;
				default:printf("? "); break;
				}
			}
		}
		printf("\n");
	}
	printf("\033[0m");
	return;
}


void initialize(chess* _game)
{
	//커서 숨김
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { .dwSize = 1,.bVisible = 0 };
	SetConsoleCursorInfo(consoleHandle, &cursor);
	//콘솔 크기 조절, 제목 변경
	system("mode con cols=50 lines=25");
	system("title yont_chess");
	//말판마다 초기위치 정보 넣어줌
	int i, j, k = 7;
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 16; j++)
		{
			if (i == 0)
			{
				if (j == 8)
				{
					k--;
				}
				(_game->players[i]).pieces[j].rank = k;
				(_game->players[i]).pieces[j].file = j % 8;
			}
			else
			{
				if (j == 8)
				{
					k++;
				}
				(_game->players[i]).pieces[j].rank = k;
				(_game->players[i]).pieces[j].file = j % 8;
			}
			(_game->players[i]).pieces[j].life = 1;
		}
		k = 0;
	}
	//체스판에 말판들의 위치를 넣어줌
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 16; j++)
		{
			if (i == 0)
			{
				(_game->chessboard)[_game->players[i].pieces[j].rank][_game->players[i].pieces[j].file] = j;
			}
			else
			{
				(_game->chessboard)[_game->players[i].pieces[j].rank][_game->players[i].pieces[j].file] = j + 100;
			}

		}
	}
	//체스판의 빈곳을 16로 채움
	for (i = 2; i <= 5; i++)
	{
		for (j = 0; j < 8; j++)
		{
			(_game->chessboard)[i][j] = 16;
		}
	}
	return;
}

int show(chess* _game, int* _k, int* _l, int _flag)
{
	//체스판, 말판을 보여줌
	int i, j, temp, flag = 0, confirm;
	for (i = 0; i < 8; i++)
	{
		printf("-----------------\n");
		printf("|");
		for (j = 0; j < 8; j++)
		{
			temp = (_game->chessboard)[i][j];
			if (temp >= 100) //흑말인 경우
			{
				printf("\033[36m");
			}
			if (temp >= 1000) //움직일수 있는 범위인 경우
			{
				printf("\033[41m");
			}
			temp = temp % 100;
			if (temp >= 20 && temp <= 24) //프로모션된 경우
			{
				temp -= 20;
			}
			if (i == *_k && j == *_l) //선택된 말의 경우
			{
				printf("\033[45m");
			}
			switch (temp)
			{
			case 0:
			case 7:printf("R"); break;
			case 1:
			case 6:printf("N"); break;
			case 2:
			case 5:printf("B"); break;
			case 3:printf("Q"); break;
			case 4:printf("K"); break;
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:printf("P"); break;
			case 16:printf(" "); break;
			default:printf("?"); break;
			}
			printf("\033[0m");
			printf("|");
		}
		//설명 출력함
		if (i == 0)
		{
			printf("	---용 체스---");
		}
		if (i == 1)
		{
			printf("	이동 : '커서'");
		}
		if (i == 2)
		{
			printf("	선택 : 'enter'");
		}
		if (i == 4)
		{
			if (_flag == 0)
			{
				printf("	백 순서입니다!");
			}
			else
			{
				printf("\033[36m");
				printf("	흑 순서입니다!");
				printf("\033[0m");
			}
		}
		if (i == 5)
		{
			confirm = check(*_game, 0); //백, 흑 구분하기 위한 변수.
			if (confirm != 0)
			{
				if (confirm < 10)
				{
					printf("	백 ");
				}
				else
				{
					printf("	흑 ");
				}
			}
			switch (confirm%10)
			{
			case 1:printf("체크입니다!"); break; //체크
			case 2:printf("체크메이트입니다!"); flag = 1; break; //체크메이트 (게임끝)
			default:
				if (checkmate(*_game, 4) == 1 || checkmate(*_game, 104) == 1)
			{
				printf("스테일메이트입니다!"); flag = 2; break; //스테일메이트 (게임끝)
			}
			}
		}
		printf("\n");
	}
	printf("-----------------\n");
	//죽은 말 출력함
	printf("\n");
	deadshow(_game);
	for (i = 0; i < 2; i++)
	{
		if ((_game->players)[i].pieces[4].life == 0) //킹이 죽었을때 (게임끝)
		{
			flag = 3;
		}
	}
	return flag;
}

int key(chess* _game, int* _k, int* _l, int* _sk, int* _sl, int _flag)
{
	kbhit();
	int temp = getch();
	if (temp == 224) //방향키일때
	{
		temp = getch();
		switch (temp)
		{
		case 72: if (*_k > 0) { (*_k)--; } break; //아래
		case 80: if (*_k < 7) { (*_k)++; } break; //위
		case 75: if (*_l > 0) { (*_l)--; } break; //왼
		case 77: if (*_l < 7) { (*_l)++; } break; //오른
		default: printf("error\n"); return -1;
		}
		return 0;
	}
	else if (temp == 13) //엔터일때
	{
		if ((_game->chessboard)[*_k][*_l] >= 1000)
		{
			swap(_game, &(_game->chessboard)[*_sk][*_sl], &(_game->chessboard)[*_k][*_l], _k, _l);
			m_thset(_game);
			sync(_game);
			int encheck = (_game->chessboard)[*_k][*_l] % 100; //앙파상 확인 예비 변수
			if (*_sk == 6 && *_k == 4 && (_game->chessboard)[*_k][*_l] >= 8 && (_game->chessboard)[*_k][*_l] <= 15) //앙파상 가능성 있는 말에 표시 (life를 10 올려줌)
			{
				(_game->players)[0].pieces[encheck].life += 10;
			}
			if ((*_sk == 1 && *_k == 3 && (_game->chessboard)[*_k][*_l] >= 108 && (_game->chessboard)[*_k][*_l] <= 115))
			{
				(_game->players)[1].pieces[encheck].life += 10;
			}
			promotion(_game); //프로모션 확인
			if ((_game->chessboard)[*_k][*_l] == 4 && (*_sl - *_l >= 2 || *_l - *_sl >= 2)) //캐슬링 확인 (킹이 2칸 이상 움직였을때)
			{
				castling(_game, _k, _l);
			}
			if ((_game->chessboard)[*_k][*_l] == 104 && (*_sl - *_l >= 2 || *_l - *_sl >= 2))
			{
				castling(_game, _k, _l);
			}
			return 1;
		}
		else if ((_game->chessboard)[*_k][(*_l)] >= _flag * 100 && (_game->chessboard)[*_k][(*_l)] <= (_flag * 100) + 100) //번갈아 가면서 장기말을 움직이게 하기위한 확인
		{
			m_thset(_game);
			*_sk = *_k;
			*_sl = *_l;
			temp = (_game->chessboard)[*_k][*_l];
			if ((temp >= 20 && temp <= 24) || (temp >= 120 && temp <= 124))
			{
				temp -= 20;
			}
			switch (temp % 100)
			{
			case 0:
			case 7:rook(_game, _k, _l); break;
			case 1:
			case 6:night(_game, _k, _l); break;
			case 2:
			case 5:bishop(_game, _k, _l); break;
			case 3:queen(_game, _k, _l); break;
			case 4:king(_game, _k, _l, 0); break;
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:pawn(_game, _k, _l, 0); break;
			case 16:break;
			default:printf("error\n"); return -1; break;
			}
			return 0;
		}
	}
	return 0;
}

int again()
{
	char select;
	printf("다시 하시려면 'y' 끝내시려면 'n'을 입력후 'enter'");
	scanf("%c", &select);
	if (select == 'y')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void rook(chess* _game, int* _k, int* _l)
{
	//룩이 움직일수 있는 공간 값에 1000을 더해줌
	int i;
	for (i = 1; (*_k + i) < 8; i++)
	{
		if ((_game->chessboard)[*_k + i][*_l] == 16)
		{
			(_game->chessboard)[*_k + i][*_l] += 1000;
		}
		else if (((_game->chessboard)[*_k + i][*_l] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k + i][*_l] >= 76))
		{
			(_game->chessboard)[*_k + i][*_l] += 1000;
			break;
		}
		else
		{
			break;
		}
	}
	for (i = 1; (*_k - i) >= 0; i++)
	{
		if ((_game->chessboard)[*_k - i][*_l] == 16)
		{
			(_game->chessboard)[*_k - i][*_l] += 1000;
		}
		else if (((_game->chessboard)[*_k - i][*_l] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k - i][*_l] >= 76))
		{
			(_game->chessboard)[*_k - i][*_l] += 1000;
			break;
		}
		else
		{
			break;
		}
	}
	for (i = 1; (*_l + i) < 8; i++)
	{
		if ((_game->chessboard)[*_k][*_l + i] == 16)
		{
			(_game->chessboard)[*_k][*_l + i] += 1000;
		}
		else if (((_game->chessboard)[*_k][*_l + i] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k][*_l + i] >= 76))
		{
			(_game->chessboard)[*_k][*_l + i] += 1000;
			break;
		}
		else
		{
			break;
		}
	}
	for (i = 1; (*_l - i) >= 0; i++)
	{
		if ((_game->chessboard)[*_k][*_l - i] == 16)
		{
			(_game->chessboard)[*_k][*_l - i] += 1000;
		}
		else if (((_game->chessboard)[*_k][*_l - i] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k][*_l - i] >= 76))
		{
			(_game->chessboard)[*_k][*_l - i] += 1000;
			break;
		}
		else
		{
			break;
		}
	}
	return;
}

void night(chess* _game, int* _k, int* _l)
{
	//나이트가 움직일수 있는곳에 1000을 더해줌
	if (((_game->chessboard)[*_k + 2][*_l + 1] == 16
		|| ((_game->chessboard)[*_k + 2][*_l + 1] - (_game->chessboard)[*_k][*_l] >= 76 || (_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k + 2][*_l + 1] >= 76)
		) && (*_k + 2 < 8 && *_l + 1 < 8))
	{
		(_game->chessboard)[*_k + 2][*_l + 1] += 1000;
	}
	if (((_game->chessboard)[*_k - 2][*_l + 1] == 16
		|| ((_game->chessboard)[*_k - 2][*_l + 1] - (_game->chessboard)[*_k][*_l] >= 76 || (_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k - 2][*_l + 1] >= 76))
		&& (*_k - 2 >= 0 && *_l + 1 < 8))
	{
		(_game->chessboard)[*_k - 2][*_l + 1] += 1000;
	}
	if (((_game->chessboard)[*_k + 2][*_l - 1] == 16
		|| ((_game->chessboard)[*_k + 2][*_l - 1] - (_game->chessboard)[*_k][*_l] >= 76 || (_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k + 2][*_l - 1] >= 76))
		&& (*_k + 2 < 8 && *_l - 1 >= 0))
	{
		(_game->chessboard)[*_k + 2][*_l - 1] += 1000;
	}
	if (((_game->chessboard)[*_k - 2][*_l - 1] == 16
		|| ((_game->chessboard)[*_k - 2][*_l - 1] - (_game->chessboard)[*_k][*_l] >= 76 || (_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k - 2][*_l - 1] >= 76))
		&& (*_k - 2 >= 0 && *_l - 1 >= 0))
	{
		(_game->chessboard)[*_k - 2][*_l - 1] += 1000;
	}
	if (((_game->chessboard)[*_k + 1][*_l + 2] == 16
		|| ((_game->chessboard)[*_k + 1][*_l + 2] - (_game->chessboard)[*_k][*_l] >= 76 || (_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k + 1][*_l + 2] >= 76))
		&& (*_k + 1 < 8 && *_l + 2 < 8))
	{
		(_game->chessboard)[*_k + 1][*_l + 2] += 1000;
	}
	if (((_game->chessboard)[*_k - 1][*_l + 2] == 16
		|| ((_game->chessboard)[*_k - 1][*_l + 2] - (_game->chessboard)[*_k][*_l] >= 76 || (_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k - 1][*_l + 2] >= 76))
		&& (*_k - 1 >= 0 && *_l + 2 < 8))
	{
		(_game->chessboard)[*_k - 1][*_l + 2] += 1000;
	}
	if (((_game->chessboard)[*_k + 1][*_l - 2] == 16
		|| ((_game->chessboard)[*_k + 1][*_l - 2] - (_game->chessboard)[*_k][*_l] >= 76 || (_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k + 1][*_l - 2] >= 76))
		&& (*_k + 1 < 8 && *_l - 2 >= 0))
	{
		(_game->chessboard)[*_k + 1][*_l - 2] += 1000;
	}
	if (((_game->chessboard)[*_k - 1][*_l - 2] == 16
		|| ((_game->chessboard)[*_k - 1][*_l - 2] - (_game->chessboard)[*_k][*_l] >= 76 || (_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k - 1][*_l - 2] >= 76))
		&& (*_k - 1 >= 0 && *_l - 2 >= 0))
	{
		(_game->chessboard)[*_k - 1][*_l - 2] += 1000;
	}
	return;
}

void bishop(chess* _game, int* _k, int* _l)
{
	//비숍이 움직일수 있는 곳에 1000을 더해줌
	int i;
	for (i = 1; (*_k + i) < 8 && (*_l + i) < 8; i++)
	{
		if ((_game->chessboard)[*_k + i][*_l + i] == 16)
		{
			(_game->chessboard)[*_k + i][*_l + i] += 1000;
		}
		else if (((_game->chessboard)[*_k + i][*_l + i] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k + i][*_l + i] >= 76))
		{
			(_game->chessboard)[*_k + i][*_l + i] += 1000;
			break;
		}
		else
		{
			break;
		}
	}
	for (i = 1; (*_k - i) >= 0 && (*_l - i) >= 0; i++)
	{
		if ((_game->chessboard)[*_k - i][*_l - i] == 16)
		{
			(_game->chessboard)[*_k - i][*_l - i] += 1000;
		}
		else if (((_game->chessboard)[*_k - i][*_l - i] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k - i][*_l - i] >= 76))
		{
			(_game->chessboard)[*_k - i][*_l - i] += 1000;
			break;
		}
		else
		{
			break;
		}
	}
	for (i = 1; (*_k - i) >= 0 && (*_l + i) < 8; i++)
	{
		if ((_game->chessboard)[*_k - i][*_l + i] == 16)
		{
			(_game->chessboard)[*_k - i][*_l + i] += 1000;
		}
		else if (((_game->chessboard)[*_k - i][*_l + i] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k - i][*_l + i] >= 76))
		{
			(_game->chessboard)[*_k - i][*_l + i] += 1000;
			break;
		}
		else
		{
			break;
		}
	}
	for (i = 1; (*_k + i) < 8 && (*_l - i) >= 0; i++)
	{
		if ((_game->chessboard)[*_k + i][*_l - i] == 16)
		{
			(_game->chessboard)[*_k + i][*_l - i] += 1000;
		}
		else if (((_game->chessboard)[*_k + i][*_l - i] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k + i][*_l - i] >= 76))
		{
			(_game->chessboard)[*_k + i][*_l - i] += 1000;
			break;
		}
		else
		{
			break;
		}
	}
	return;
}

void king(chess* _game, int* _k, int* _l, int _onlyattack) //_onlyattack은 체크확인 관련 함수에서 킹의 움직임을 볼때 확인하기 위한 인자 (체크확인 관련 함수에서는 캐슬링, 자살유무를 따질필요 x)
{
	//킹이 움직일수 있는 곳에 1000을 더해줌
	chess temp;
	if (((_game->chessboard)[*_k + 1][*_l] == 16
		|| ((_game->chessboard)[*_k + 1][*_l] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k + 1][*_l] >= 76))
		&& (*_k) + 1 < 8)
	{
		if (_onlyattack == 0)
		{
			temp = *_game;
			swap(&temp, &(temp.chessboard)[*_k][*_l], &(temp.chessboard)[(*_k) + 1][(*_l)], _k, _l);
			if (check(temp, 1) == 0)
			{
				(_game->chessboard)[*_k + 1][*_l] += 1000;
			}
		}
		else
		{
			(_game->chessboard)[*_k + 1][*_l] += 1000;
		}
	}
	if (((_game->chessboard)[*_k + 1][*_l + 1] == 16
		|| ((_game->chessboard)[*_k + 1][*_l + 1] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k + 1][*_l + 1] >= 76))
		&& (*_k) + 1 < 8 && (*_l) + 1 < 8)
	{
		if (_onlyattack == 0)
		{
			temp = *_game;
			swap(&temp, &(temp.chessboard)[*_k][*_l], &(temp.chessboard)[(*_k) + 1][(*_l) + 1], _k, _l);
			if (check(temp, 1) == 0)
			{
				(_game->chessboard)[*_k + 1][*_l + 1] += 1000;
			}
		}
		else
		{
			(_game->chessboard)[*_k + 1][*_l + 1] += 1000;
		}
	}
	if (((_game->chessboard)[*_k][*_l + 1] == 16
		|| ((_game->chessboard)[*_k][*_l + 1] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k][*_l + 1] >= 76))
		&& (*_l) + 1 < 8)
	{
		if (_onlyattack == 0)
		{
			temp = *_game;
			swap(&temp, &(temp.chessboard)[*_k][*_l], &(temp.chessboard)[(*_k)][(*_l) + 1], _k, _l);
			if (check(temp, 1) == 0)
			{
				(_game->chessboard)[*_k][*_l + 1] += 1000;
			}
		}
		else
		{
			(_game->chessboard)[*_k][*_l + 1] += 1000;
		}
	}
	if (((_game->chessboard)[*_k - 1][*_l] == 16
		|| ((_game->chessboard)[*_k - 1][*_l] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k - 1][*_l] >= 76))
		&& (*_k) - 1 >= 0)
	{
		if (_onlyattack == 0)
		{
			temp = *_game;
			swap(&temp, &(temp.chessboard)[*_k][*_l], &(temp.chessboard)[(*_k) - 1][(*_l)], _k, _l);
			if (check(temp, 1) == 0)
			{
				(_game->chessboard)[*_k - 1][*_l] += 1000;
			}
		}
		else
		{
			(_game->chessboard)[*_k - 1][*_l] += 1000;
		}
	}
	if (((_game->chessboard)[*_k - 1][*_l - 1] == 16
		|| ((_game->chessboard)[*_k - 1][*_l - 1] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k - 1][*_l - 1] >= 76))
		&& (*_k) - 1 >= 0 && (*_l) - 1 >= 0)
	{
		if (_onlyattack == 0)
		{
			temp = *_game;
			swap(&temp, &(temp.chessboard)[*_k][*_l], &(temp.chessboard)[(*_k) - 1][(*_l) - 1], _k, _l);
			if (check(temp, 1) == 0)
			{
				(_game->chessboard)[*_k - 1][*_l - 1] += 1000;
			}
		}
		else
		{
			(_game->chessboard)[*_k - 1][*_l - 1] += 1000;
		}
	}
	if (((_game->chessboard)[*_k][*_l - 1] == 16
		|| ((_game->chessboard)[*_k][*_l - 1] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k][*_l - 1] >= 76))
		&& (*_l) - 1 >= 0)
	{
		if (_onlyattack == 0)
		{
			temp = *_game;
			swap(&temp, &(temp.chessboard)[*_k][*_l], &(temp.chessboard)[(*_k)][(*_l) - 1], _k, _l);
			if (check(temp, 1) == 0)
			{
				(_game->chessboard)[*_k][*_l - 1] += 1000;
			}
		}
		else
		{
			(_game->chessboard)[*_k][*_l - 1] += 1000;
		}
	}
	if (((_game->chessboard)[*_k + 1][*_l - 1] == 16
		|| ((_game->chessboard)[*_k + 1][*_l - 1] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k + 1][*_l - 1] >= 76))
		&& (*_k) + 1 < 8 && (*_l) - 1 >= 0)
	{
		if (_onlyattack == 0)
		{
			temp = *_game;
			swap(&temp, &(temp.chessboard)[*_k][*_l], &(temp.chessboard)[(*_k) + 1][(*_l) - 1], _k, _l);
			if (check(temp, 1) == 0)
			{
				(_game->chessboard)[*_k + 1][*_l - 1] += 1000;
			}
		}
		else
		{
			(_game->chessboard)[*_k + 1][*_l - 1] += 1000;
		}
	}
	if (((_game->chessboard)[*_k - 1][*_l + 1] == 16
		|| ((_game->chessboard)[*_k - 1][*_l + 1] - (_game->chessboard)[*_k][*_l] >= 76) || ((_game->chessboard)[*_k][*_l] - (_game->chessboard)[*_k - 1][*_l + 1] >= 76))
		&& (*_k) - 1 >= 0 && (*_l) + 1 < 8)
	{
		if (_onlyattack == 0)
		{
			temp = *_game;
			swap(&temp, &(temp.chessboard)[*_k][*_l], &(temp.chessboard)[(*_k) - 1][(*_l) + 1], _k, _l);
			if (check(temp, 1) == 0)
			{
				(_game->chessboard)[*_k - 1][*_l + 1] += 1000;
			}
		}
		else
		{
			(_game->chessboard)[*_k - 1][*_l + 1] += 1000;
		}
	}
	//캐슬링
	if (_onlyattack == 0)
	{
		int i, flag = 0, j;
		if ((_game->chessboard)[*_k][*_l] == 4 && *_k == 7 && *_l == 4 && (_game->players)[0].pieces[4].life < 10) //백
		{
			if ((_game->players)[0].pieces[7].rank == 7 && (_game->players)[0].pieces[7].file == 7 && (_game->players)[0].pieces[7].life < 10) //킹사이드
			{
				for (i = 5; i <= 6; i++)
				{
					if ((_game->chessboard)[7][i] % 100 != 16)
					{
						flag++;
						break;
					}
					if (flag == 0) //캐슬링하는 사이에 적이 공격 가능한지 확인
					{
						for (j = 100; j <= 115; j++)
						{
							if (checkattack(*_game, 7, i, j) == 1)
							{
								flag++;
								break;
							}
						}
					}
				}
				if (flag == 0)
				{
					(_game->chessboard)[7][6] += 1000;
				}
			}
			flag = 0;
			if ((_game->players)[0].pieces[0].rank == 7 && (_game->players)[0].pieces[0].file == 0 && (_game->players)[0].pieces[0].life < 10) //퀸사이드
			{
				for (i = 1; i <= 3; i++)
				{
					if ((_game->chessboard)[7][i] % 100 != 16)
					{
						flag++;
						break;
					}
					if (flag == 0) //캐슬링하는 사이에 적이 공격 가능한지 확인
					{
						for (j = 100; j <= 115; j++)
						{
							if (checkattack(*_game, 7, i, j) == 1)
							{
								flag++;
								break;
							}
						}
					}
				}
				if (flag == 0)
				{
					(_game->chessboard)[7][2] += 1000;
				}
			}
		}
		if ((_game->chessboard)[*_k][*_l] == 104 && *_k == 0 && *_l == 4 && (_game->players)[1].pieces[4].life < 10) //흑
		{
			if ((_game->players)[1].pieces[7].rank == 0 && (_game->players)[1].pieces[7].file == 7 && (_game->players)[1].pieces[7].life < 10) //킹사이드
			{
				for (i = 5; i <= 6; i++)
				{
					if ((_game->chessboard)[0][i] % 100 != 16)
					{
						flag++;
					}
					if (flag == 0) //캐슬링하는 사이에 적이 공격 가능한지 확인
					{
						for (j = 0; j <= 15; j++)
						{
							if (checkattack(*_game, 0, i, j) == 1)
							{
								flag++;
								break;
							}
						}
					}
				}
				if (flag == 0)
				{
					(_game->chessboard)[0][6] += 1000;
				}
			}
			flag = 0;
			if ((_game->players)[1].pieces[0].rank == 0 && (_game->players)[1].pieces[0].file == 0 && (_game->players)[1].pieces[0].life < 10) //퀸사이드
			{
				for (i = 1; i <= 3; i++)
				{
					if ((_game->chessboard)[0][i] % 100 != 16)
					{
						flag++;
					}
					if (flag == 0) //캐슬링하는 사이에 적이 공격 가능한지 확인
					{
						for (j = 0; j <= 15; j++)
						{
							if (checkattack(*_game, 0, i, j) == 1)
							{
								flag++;
								break;
							}
						}
					}
				}
				if (flag == 0)
				{
					(_game->chessboard)[0][2] += 1000;
				}
			}
		}
	}
	return;
}

void queen(chess* _game, int* _k, int* _l)
{
	//퀸은 룩 + 비숍의 움직임
	rook(_game, _k, _l);
	bishop(_game, _k, _l);
	return;
}

void pawn(chess* _game, int* _k, int* _l, int _onlyattack) //_onlyattack은 체크관련 함수에서 폰의 움직임을 확인하기 위한 인자 (체크관련 함수에서는 폰이 공격하는 방향[대각선]방향만 필요함
{
	//폰이 움직일수 있는곳에 1000을 더해줌
	int temp;
	if ((_game->chessboard)[*_k][*_l] <= 15) //백 폰
	{
		if (_onlyattack == 0)
		{
			if ((_game->chessboard)[(*_k) - 1][*_l] == 16 && ((*_k) - 1 >= 0 && (*_l) < 8))
			{
				(_game->chessboard)[(*_k) - 1][*_l] += 1000;
				if ((_game->chessboard)[(*_k) - 2][*_l] == 16 && ((*_k) - 2 >= 0 && (*_l) < 8) && *_k == 6) //한발짝도 안 움직였을때
				{
					(_game->chessboard)[(*_k) - 2][*_l] += 1000;
				}
			}
			if (*_k == 3) //앙파상
			{
				temp = (_game->chessboard)[*_k][(*_l) - 1] % 100;
				if ((_game->chessboard)[*_k][(*_l) - 1] >= 108 && (_game->chessboard)[*_k][(*_l) - 1] <= 115 && (_game->players)[1].pieces[temp].life >= 10 //앙파상은 내옆에 앙파상 가능성 (직전 움직임이 2칸을 움직였었음) 이면서 내 말 옆에 있을경우
					&& ((*_k) - 1 >= 0 && (*_l) - 1 >= 0) && (_game->chessboard)[(*_k) - 1][(*_l) - 1] == 16)
				{
					(_game->chessboard)[(*_k) - 1][(*_l) - 1] += 1000;
				}
				temp = (_game->chessboard)[*_k][(*_l) + 1] % 100;
				if ((_game->chessboard)[*_k][(*_l) + 1] >= 108 && (_game->chessboard)[*_k][(*_l) + 1] <= 115 && (_game->players)[1].pieces[temp].life >= 10
					&& ((*_k) - 1 >= 0 && (*_l) + 1 <= 7) && (_game->chessboard)[(*_k) - 1][(*_l) + 1] == 16)
				{
					(_game->chessboard)[(*_k) - 1][(*_l) + 1] += 1000;
				}
			}
		}
		if ((_game->chessboard)[(*_k) - 1][(*_l) - 1] >= 100 && (_game->chessboard)[(*_k) - 1][(*_l) - 1] != 16 && ((*_k) - 1 >= 0 && (*_l) - 1 >= 0))
		{
			(_game->chessboard)[(*_k) - 1][(*_l) - 1] += 1000;
		}
		if ((_game->chessboard)[(*_k) - 1][(*_l) + 1] >= 100 && (_game->chessboard)[(*_k) - 1][(*_l) + 1] != 16 && ((*_k) - 1 >= 0 && (*_l) + 1 < 8))
		{
			(_game->chessboard)[(*_k) - 1][(*_l) + 1] += 1000;
		}
	}
	else //흑 폰
	{
		if (_onlyattack == 0)
		{
			if ((_game->chessboard)[(*_k) + 1][*_l] == 16 && ((*_k) + 1 < 8 && (*_l) < 8))
			{
				(_game->chessboard)[(*_k) + 1][*_l] += 1000;
				if ((_game->chessboard)[(*_k) + 2][*_l] == 16 && ((*_k) + 2 < 8 && (*_l) < 8) && *_k == 1) //한발짝도 안 움직였을때
				{
					(_game->chessboard)[(*_k) + 2][*_l] += 1000;
				}
			}
			if (*_k == 4) //앙파상
			{
				temp = (_game->chessboard)[*_k][(*_l) - 1] % 100;
				if ((_game->chessboard)[*_k][(*_l) - 1] >= 8 && (_game->chessboard)[*_k][(*_l) - 1] <= 15 && (_game->players)[0].pieces[temp].life >= 10
					&& ((*_k) + 1 <= 7 && (*_l) - 1 >= 0) && (_game->chessboard)[(*_k) + 1][(*_l) - 1] == 16)
				{
					(_game->chessboard)[(*_k) + 1][(*_l) - 1] += 1000;
				}
				temp = (_game->chessboard)[*_k][(*_l) + 1] % 100;
				if ((_game->chessboard)[*_k][(*_l) + 1] >= 8 && (_game->chessboard)[*_k][(*_l) + 1] <= 15 && (_game->players)[0].pieces[temp].life >= 10
					&& ((*_k) + 1 <= 7 && (*_l) + 1 <= 7) && (_game->chessboard)[(*_k) + 1][(*_l) + 1] == 16)
				{
					(_game->chessboard)[(*_k) + 1][(*_l) + 1] += 1000;
				}
			}
		}
		if ((_game->chessboard)[(*_k) + 1][(*_l) - 1] <= 100 && (_game->chessboard)[(*_k) + 1][(*_l) - 1] != 16 && ((*_k) + 1 < 8 && (*_l) - 1 >= 0))
		{
			(_game->chessboard)[(*_k) + 1][(*_l) - 1] += 1000;
		}
		if ((_game->chessboard)[(*_k) + 1][(*_l) + 1] <= 100 && (_game->chessboard)[(*_k) + 1][(*_l) + 1] != 16 && ((*_k) + 1 < 8 && (*_l) + 1 < 8))
		{
			(_game->chessboard)[(*_k) + 1][(*_l) + 1] += 1000;
		}
	}
	return;
}

void promotion(chess* _game) //프로모션 함수
{
	int i, j;
	char temp;
	for (i = 0; i < 2; i++)
	{
		for (j = 8; j < 16; j++)
		{
			if (i == 0 && (_game->players)[i].pieces[j].rank == 0)
			{
				printf("\n무엇으로 승급?\n");
			RE0:
				printf("키보드에서 원하는 말을 입력후 'enter' ");
				scanf("%c", &temp);
				switch (temp)
				{
				case 'r': temp = 20; break;
				case 'n': temp = 21; break;
				case 'b': temp = 22; break;
				case 'q': temp = 23; break;
				default: goto RE0; break;
				}
				(_game->players)[i].pieces[j].life = 2; //프로모션한 폰은 life가 2 (deadshow에서 죽은 말로 처리되지 않기 위함)
				(_game->chessboard)[((_game->players)[i].pieces[j].rank)++][(_game->players)[i].pieces[j].file] = temp;
				return;
			}
			if (i == 1 && (_game->players)[i].pieces[j].rank == 7)
			{
				printf("\n무엇으로 승급할까요?\n");
			RE1:
				printf("키보드에서 원하는 말을 입력후 'enter' ");
				scanf("%c", &temp);
				switch (temp)
				{
				case 'r': temp = 120; break;
				case 'n': temp = 121; break;
				case 'b': temp = 122; break;
				case 'q': temp = 123; break;
				default: goto RE1; break;
				}
				(_game->players)[i].pieces[j].life = 2; //프로모션한 폰은 life가 2 (deadshow에서 죽은 말로 처리되지 않기 위함)
				(_game->chessboard)[((_game->players)[i].pieces[j].rank)--][(_game->players)[i].pieces[j].file] = temp;
				return;
			}
		}
	}
	return;
}

void enpassant(chess* _game, int* _k, int* _l) //앙파상일 경우 특수한 움직임이 필요함
{
	int temp;
	if ((_game->chessboard)[*_k][*_l] >= 8 && (_game->chessboard)[*_k][*_l] <= 15 && *_k == 2)
	{
		temp = (_game->chessboard)[(*_k) + 1][*_l] % 100;
		if ((_game->players)[1].pieces[temp].life >= 10)
		{
			(_game->players)[1].pieces[temp].life = 0; //상대말 life 죽이고
			(_game->chessboard)[(*_k) + 1][*_l] = 16; //chessboard의 공간에 빈공간(16)을 넣어줌
		}
	}
	if ((_game->chessboard)[*_k][*_l] >= 108 && (_game->chessboard)[*_k][*_l] <= 115 && *_k == 5)
	{
		temp = (_game->chessboard)[(*_k) - 1][*_l] % 100;
		if ((_game->players)[0].pieces[temp].life >= 10)
		{
			(_game->players)[0].pieces[temp].life = 0;
			(_game->chessboard)[(*_k) - 1][*_l] = 16;
		}
	}
	return;
}

void castling(chess* _game, int* _k, int* _l) //캐슬링 함수 (킹이 움직였을경우 룩도 움직여줌)
{
	if ((_game->chessboard)[0][2] == 104)
	{
		swap(_game, &(_game->chessboard)[0][0], &(_game->chessboard)[0][3], _k, _l);
		(_game->players)[1].pieces[4].life += 10;
		(_game->players)[1].pieces[0].life += 10;
	}
	else if ((_game->chessboard)[0][6] == 104)
	{
		swap(_game, &(_game->chessboard)[0][7], &(_game->chessboard)[0][5], _k, _l);
		(_game->players)[1].pieces[4].life += 10;
		(_game->players)[1].pieces[7].life += 10;
	}
	else if ((_game->chessboard)[7][2] == 4)
	{
		swap(_game, &(_game->chessboard)[7][0], &(_game->chessboard)[7][3], _k, _l);
		(_game->players)[0].pieces[4].life += 10;
		(_game->players)[0].pieces[0].life += 10;
	}
	else if ((_game->chessboard)[7][6] == 4)
	{
		swap(_game, &(_game->chessboard)[7][7], &(_game->chessboard)[7][5], _k, _l);
		(_game->players)[0].pieces[4].life += 10;
		(_game->players)[0].pieces[7].life += 10;
	}
	sync(_game);
	return;
}

int checkattack(chess _game, int _k, int _l, int _attack) //어떠한 말이 k,l인덱스를 공격 가능한지 확인하는 함수
{
	m_thset(&_game);
	int i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if ((_game.chessboard)[i][j] == _attack)
			{
				_attack = _attack % 100;
				if (_attack >= 20)
				{
					_attack -= 20;
				}
				switch (_attack)
				{
				case 0:
				case 7:rook(&_game, &i, &j); break;
				case 1:
				case 6:night(&_game, &i, &j); break;
				case 2:
				case 5:bishop(&_game, &i, &j); break;
				case 3:queen(&_game, &i, &j); break;
				case 4:king(&_game, &i, &j, 1); break;
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:pawn(&_game, &i, &j, 1); break;
				default:return -1; //오류
				}
				if ((_game.chessboard)[_k][_l] >= 1000)
				{
					return 1; //공격 가능성 있음(위험)
				}
				else
				{
					return 0; //공격 가능성 없음(안전)
				}
			}
		}
	}
	return 0; //공격 가능성 없음(안전)
}

int check(chess _game, int _onlycheck) //체크 확인 함수
{
	m_thset(&_game);
	int king, i, j, k;
	for (king = 4; king <= 104; king += 100) //백킹 >> 흑킹 순서대로 확인
	{
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 8; j++)
			{
				if ((_game.chessboard)[i][j] == king) //chessboard에서 킹을 찾아서
				{
					if (king == 4)
					{
						for (k = 100; k <= 115; k++) //상대편 말들이 공격 가능한지 확인
						{
							if ((_game.players)[1].pieces[k % 100].life == 0)
							{
								continue;
							}
							if (checkattack(_game, i, j, k) == 1)
							{
								if (_onlycheck == 0) //체크만 확인할꺼면 여기서 그만
								{
									if (checkmate(_game, king) == 1) //체크 이면 체크메이트 인지도 확인
									{
										return 2;
									}
								}
								return 1;
							}
						}
						for (k = 120; k <= 123; k++) //프로몬션으로 살아난 말들도 확인해줌
						{
							if (checkattack(_game, i, j, k) == 1)
							{
								if (_onlycheck == 0)
								{
									if (checkmate(_game, king) == 1)
									{
										return 2;
									}
								}
								return 1;
							}
						}
					}
					else
					{
						for (k = 0; k <= 15; k++)
						{
							if ((_game.players)[0].pieces[k].life == 0)
							{
								continue;
							}
							if (checkattack(_game, i, j, k) == 1)
							{
								if (_onlycheck == 0)
								{
									if (checkmate(_game, king) == 1)
									{
										return 12;
									}
								}
								return 11;
							}
						}
						for (k = 20; k <= 23; k++)
						{
							if (checkattack(_game, i, j, k) == 1)
							{
								if (_onlycheck == 0)
								{
									if (checkmate(_game, king) == 1)
									{
										return 12;
									}
								}
								return 11;
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

int checkmate(chess _game, int _king) //체크메이트 인지 확인 (내 말들을 1칸씩 모두 움직여 봤을때 모두 체크이면 체크메이트)
{
	int i, j, flag;
	if (_king == 4)
	{
		flag = 0; //백일때
		i = 0;
	}
	else
	{
		flag = 10; //흑일때
		i = 100;
	}
	j = i + 15;
	for (; i <= j; i++)
	{
		if (checkmate_check(_game, i) == 1)
		{
			return 0 + flag; //체크메이트가 아님
		}
	}
	for (i = j + 5; i <= j + 8; i++)
	{
		if (checkmate_check(_game, i) == 1)
		{
			return 0 + flag; //체크메이트가 아님
		}
	}
	return 1 + flag; //체크메이트임
}

int checkmate_check(chess _game, int _piece)
{
	int i, j, k, l, flag = 0;
	chess game1 = _game;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if ((_game.chessboard)[i][j] == _piece)
			{
				flag++;
				break;
			}
		}
		if (flag > 0)
		{
			break;
		}
	}
	if (flag == 0)
	{
		return 0; //말이 없으니 막아줄수 없음
	}
	_piece = _piece % 100;
	if (_piece >= 20)
	{
		_piece -= 20;
	}
	switch (_piece)
	{
	case 0:
	case 7:rook(&_game, &i, &j); break;
	case 1:
	case 6:night(&_game, &i, &j); break;
	case 2:
	case 5:bishop(&_game, &i, &j); break;
	case 3:queen(&_game, &i, &j); break;
	case 4:king(&_game, &i, &j, 1); break;
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:pawn(&_game, &i, &j, 0); break;
	default:return -1; //오류
	}
	for (k = 0; k < 8; k++)
	{
		for (l = 0; l < 8; l++)
		{
			if ((_game.chessboard)[k][l] >= 1000)
			{
				game1 = _game;
				swap(&game1, &(_game.chessboard)[i][j], &(game1.chessboard)[k][l], &k, &l);
				if (check(game1, 1) == 0)
				{
					return 1; //체크메이트가 아님(이 말이 막아줄수 있음)
				}
			}
		}
	}
	return 0; //체크메이트임(이 말이 막아줄수 없음)
}