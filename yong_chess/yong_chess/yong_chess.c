/*
	chess �ȿ� chessboard, players�� �־ ü���ǰ� �÷��̾���� ������ �������� �������.
	chessboard�� players�� ���ε��� �̸�, chessboard�� ������ ������ players�� ����ȭ �����ִ� ���.
	0~15 ���� ��, 20~23 ���θ������ ���λ��ܳ� ���� ��.
	100~114 ���� ��, 120~123 ���θ������ ���λ��ܳ� ���� ��.
	players �ȿ� pieces�� rank(��), file(��), life ������ �������.
	life�� 0(�׾��� ���), 1(����� ���), 11(���Ļ� ���ɼ� �ִ� ��, �ѹ��̶� ������ ��,ŷ) ���� ����.
	���� ���� ������ �����ϼ� �ִ°��� 1000�� �����־ ǥ�ø� ����.
	swap - chessboard�� ������ �ٲ���. ��ĭ���� ���°��� ����� ���� �Դ� ��찡 ����.
	m_thset - 1000�� ������ ��� 1000�� �ٽ� ����.
	sync - chessboard�� players ���� ����ȭ ������.
	deadshow - life�� 0�� ���� ������.
	initialize - chessboard�� ������ ���� �־��ְ�, players�� rank, file, life ���� �־���.
	show - chessboard�� �� ���� �°� ���ĺ��� �����, ������ ������ �ٸ���, ���õȰ�, �����ϼ� �ִ� �� ������ �ٸ���
		   ������. üũ, üũ����Ʈ, �����ϸ���Ʈ�� ���⼭ Ȯ����.
	again - ������ �������� �ٽ�����, ������ Ȯ����.
	rook ~ pawn - �����ϼ� �ִ� ���� 1000�� ������.
	promotion - ���� chessboard�� ���� �Դ��� Ȯ���ؼ� ���θ�� ����.
	enpassant - ���Ļ� ���ɼ� �ִ� ��(life==11) �� �� ���� ��� ���� ������ ���Ļ� �� ����.
	castling - ŷ, ���� �ѹ��� �ȿ�������, �߰��� ����� ���� ����, �� �����̸� ĳ���� �� ����.
	checkattackk - � ���� � ������ �����Ҽ� �ִ��� Ȯ������.
	check - üũ�������� Ȯ������
	checkmate -
	checkmate_check - �츮�� ���� 1�� �����̴� ��� ����� ���� ����ؼ� üũ�� �ƴ� ��찡 �ִ��� Ȯ����.
*/
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

//rank(��), file(��)
typedef struct
{
	int rank, file;
	int life;
} piece;

//player���� ��⸻�� ����� ����� �� - 0:l ~ 15:p �� - 100:l ~ 115:p ��� - 16
typedef struct
{
	piece pieces[16];
} player;

//ü����, players[0]=��, [1]=��
typedef struct
{
	int chessboard[8][8];
	player players[2];
} chess;

void swap(chess*, int*, int*, int*, int*); //���� ���� �̵� �Լ�
void m_thset(chess*); //1000���������� �ٽ� ���ִ� �Լ�
void sync(chess*); //chessboard�� players�� rank,file�� ����ȭ �����ִ� �Լ�
void deadshow(chess*); //���� ���� �����ִ� �Լ�

void initialize(chess*); //�ʱ� ���� �Լ�
int show(chess*, int*, int*, int); //��� �Լ�
int key(chess*, int*, int*, int*, int*, int); //Ű �Է� �Լ�
int again(); //������ �������� �ٽ� ���� Ȯ��

void rook(chess*, int*, int*); //�� �̵��Լ���
void night(chess*, int*, int*);
void bishop(chess*, int*, int*);
void king(chess*, int*, int*, int);
void queen(chess*, int*, int*);
void pawn(chess*, int*, int*, int);

void promotion(chess*); //���θ��
void enpassant(chess*, int*, int*); //���Ļ�
void castling(chess*, int*, int*); //ĳ����
int checkattack(chess, int, int, int); //���� ������ �ֳ� Ȯ�� �Լ�
int check(chess, int); //üũ Ȯ��
int checkmate(chess, int); //üũ����Ʈ
int checkmate_check(chess, int);

int main()
{
regame:
	{
		int k = 7, l = 7; //���缱�õ� �ε���[k][l]
		int sk, sl; //�÷��̾ ������ ���� �ε���[sk][sl]
		int flag = 0; //��, �� �����ư��鼭 �ϴ��� Ȯ���ϴ� �÷���
		chess game;
		initialize(&game);
		while (1)
		{
			if (show(&game, &k, &l, flag) != 0)
			{
				printf("\n������ �������ϴ�!\n");
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
		enpassant(_game, _k, _l); //���� ���ΰ� �ƴϸ�, ���Ļ� Ȯ��
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
			printf("���� ���� �� : ");
		}
		else
		{
			printf("\033[36m");
			printf("���� ���� �� : ");
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
	//Ŀ�� ����
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { .dwSize = 1,.bVisible = 0 };
	SetConsoleCursorInfo(consoleHandle, &cursor);
	//�ܼ� ũ�� ����, ���� ����
	system("mode con cols=50 lines=25");
	system("title yont_chess");
	//���Ǹ��� �ʱ���ġ ���� �־���
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
	//ü���ǿ� ���ǵ��� ��ġ�� �־���
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
	//ü������ ����� 16�� ä��
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
	//ü����, ������ ������
	int i, j, temp, flag = 0, confirm;
	for (i = 0; i < 8; i++)
	{
		printf("-----------------\n");
		printf("|");
		for (j = 0; j < 8; j++)
		{
			temp = (_game->chessboard)[i][j];
			if (temp >= 100) //�渻�� ���
			{
				printf("\033[36m");
			}
			if (temp >= 1000) //�����ϼ� �ִ� ������ ���
			{
				printf("\033[41m");
			}
			temp = temp % 100;
			if (temp >= 20 && temp <= 24) //���θ�ǵ� ���
			{
				temp -= 20;
			}
			if (i == *_k && j == *_l) //���õ� ���� ���
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
		//���� �����
		if (i == 0)
		{
			printf("	---�� ü��---");
		}
		if (i == 1)
		{
			printf("	�̵� : 'Ŀ��'");
		}
		if (i == 2)
		{
			printf("	���� : 'enter'");
		}
		if (i == 4)
		{
			if (_flag == 0)
			{
				printf("	�� �����Դϴ�!");
			}
			else
			{
				printf("\033[36m");
				printf("	�� �����Դϴ�!");
				printf("\033[0m");
			}
		}
		if (i == 5)
		{
			confirm = check(*_game, 0); //��, �� �����ϱ� ���� ����.
			if (confirm != 0)
			{
				if (confirm < 10)
				{
					printf("	�� ");
				}
				else
				{
					printf("	�� ");
				}
			}
			switch (confirm%10)
			{
			case 1:printf("üũ�Դϴ�!"); break; //üũ
			case 2:printf("üũ����Ʈ�Դϴ�!"); flag = 1; break; //üũ����Ʈ (���ӳ�)
			default:
				if (checkmate(*_game, 4) == 1 || checkmate(*_game, 104) == 1)
			{
				printf("�����ϸ���Ʈ�Դϴ�!"); flag = 2; break; //�����ϸ���Ʈ (���ӳ�)
			}
			}
		}
		printf("\n");
	}
	printf("-----------------\n");
	//���� �� �����
	printf("\n");
	deadshow(_game);
	for (i = 0; i < 2; i++)
	{
		if ((_game->players)[i].pieces[4].life == 0) //ŷ�� �׾����� (���ӳ�)
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
	if (temp == 224) //����Ű�϶�
	{
		temp = getch();
		switch (temp)
		{
		case 72: if (*_k > 0) { (*_k)--; } break; //�Ʒ�
		case 80: if (*_k < 7) { (*_k)++; } break; //��
		case 75: if (*_l > 0) { (*_l)--; } break; //��
		case 77: if (*_l < 7) { (*_l)++; } break; //����
		default: printf("error\n"); return -1;
		}
		return 0;
	}
	else if (temp == 13) //�����϶�
	{
		if ((_game->chessboard)[*_k][*_l] >= 1000)
		{
			swap(_game, &(_game->chessboard)[*_sk][*_sl], &(_game->chessboard)[*_k][*_l], _k, _l);
			m_thset(_game);
			sync(_game);
			int encheck = (_game->chessboard)[*_k][*_l] % 100; //���Ļ� Ȯ�� ���� ����
			if (*_sk == 6 && *_k == 4 && (_game->chessboard)[*_k][*_l] >= 8 && (_game->chessboard)[*_k][*_l] <= 15) //���Ļ� ���ɼ� �ִ� ���� ǥ�� (life�� 10 �÷���)
			{
				(_game->players)[0].pieces[encheck].life += 10;
			}
			if ((*_sk == 1 && *_k == 3 && (_game->chessboard)[*_k][*_l] >= 108 && (_game->chessboard)[*_k][*_l] <= 115))
			{
				(_game->players)[1].pieces[encheck].life += 10;
			}
			promotion(_game); //���θ�� Ȯ��
			if ((_game->chessboard)[*_k][*_l] == 4 && (*_sl - *_l >= 2 || *_l - *_sl >= 2)) //ĳ���� Ȯ�� (ŷ�� 2ĭ �̻� ����������)
			{
				castling(_game, _k, _l);
			}
			if ((_game->chessboard)[*_k][*_l] == 104 && (*_sl - *_l >= 2 || *_l - *_sl >= 2))
			{
				castling(_game, _k, _l);
			}
			return 1;
		}
		else if ((_game->chessboard)[*_k][(*_l)] >= _flag * 100 && (_game->chessboard)[*_k][(*_l)] <= (_flag * 100) + 100) //������ ���鼭 ��⸻�� �����̰� �ϱ����� Ȯ��
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
	printf("�ٽ� �Ͻ÷��� 'y' �����÷��� 'n'�� �Է��� 'enter'");
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
	//���� �����ϼ� �ִ� ���� ���� 1000�� ������
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
	//����Ʈ�� �����ϼ� �ִ°��� 1000�� ������
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
	//����� �����ϼ� �ִ� ���� 1000�� ������
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

void king(chess* _game, int* _k, int* _l, int _onlyattack) //_onlyattack�� üũȮ�� ���� �Լ����� ŷ�� �������� ���� Ȯ���ϱ� ���� ���� (üũȮ�� ���� �Լ������� ĳ����, �ڻ������� �����ʿ� x)
{
	//ŷ�� �����ϼ� �ִ� ���� 1000�� ������
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
	//ĳ����
	if (_onlyattack == 0)
	{
		int i, flag = 0, j;
		if ((_game->chessboard)[*_k][*_l] == 4 && *_k == 7 && *_l == 4 && (_game->players)[0].pieces[4].life < 10) //��
		{
			if ((_game->players)[0].pieces[7].rank == 7 && (_game->players)[0].pieces[7].file == 7 && (_game->players)[0].pieces[7].life < 10) //ŷ���̵�
			{
				for (i = 5; i <= 6; i++)
				{
					if ((_game->chessboard)[7][i] % 100 != 16)
					{
						flag++;
						break;
					}
					if (flag == 0) //ĳ�����ϴ� ���̿� ���� ���� �������� Ȯ��
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
			if ((_game->players)[0].pieces[0].rank == 7 && (_game->players)[0].pieces[0].file == 0 && (_game->players)[0].pieces[0].life < 10) //�����̵�
			{
				for (i = 1; i <= 3; i++)
				{
					if ((_game->chessboard)[7][i] % 100 != 16)
					{
						flag++;
						break;
					}
					if (flag == 0) //ĳ�����ϴ� ���̿� ���� ���� �������� Ȯ��
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
		if ((_game->chessboard)[*_k][*_l] == 104 && *_k == 0 && *_l == 4 && (_game->players)[1].pieces[4].life < 10) //��
		{
			if ((_game->players)[1].pieces[7].rank == 0 && (_game->players)[1].pieces[7].file == 7 && (_game->players)[1].pieces[7].life < 10) //ŷ���̵�
			{
				for (i = 5; i <= 6; i++)
				{
					if ((_game->chessboard)[0][i] % 100 != 16)
					{
						flag++;
					}
					if (flag == 0) //ĳ�����ϴ� ���̿� ���� ���� �������� Ȯ��
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
			if ((_game->players)[1].pieces[0].rank == 0 && (_game->players)[1].pieces[0].file == 0 && (_game->players)[1].pieces[0].life < 10) //�����̵�
			{
				for (i = 1; i <= 3; i++)
				{
					if ((_game->chessboard)[0][i] % 100 != 16)
					{
						flag++;
					}
					if (flag == 0) //ĳ�����ϴ� ���̿� ���� ���� �������� Ȯ��
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
	//���� �� + ����� ������
	rook(_game, _k, _l);
	bishop(_game, _k, _l);
	return;
}

void pawn(chess* _game, int* _k, int* _l, int _onlyattack) //_onlyattack�� üũ���� �Լ����� ���� �������� Ȯ���ϱ� ���� ���� (üũ���� �Լ������� ���� �����ϴ� ����[�밢��]���⸸ �ʿ���
{
	//���� �����ϼ� �ִ°��� 1000�� ������
	int temp;
	if ((_game->chessboard)[*_k][*_l] <= 15) //�� ��
	{
		if (_onlyattack == 0)
		{
			if ((_game->chessboard)[(*_k) - 1][*_l] == 16 && ((*_k) - 1 >= 0 && (*_l) < 8))
			{
				(_game->chessboard)[(*_k) - 1][*_l] += 1000;
				if ((_game->chessboard)[(*_k) - 2][*_l] == 16 && ((*_k) - 2 >= 0 && (*_l) < 8) && *_k == 6) //�ѹ�¦�� �� ����������
				{
					(_game->chessboard)[(*_k) - 2][*_l] += 1000;
				}
			}
			if (*_k == 3) //���Ļ�
			{
				temp = (_game->chessboard)[*_k][(*_l) - 1] % 100;
				if ((_game->chessboard)[*_k][(*_l) - 1] >= 108 && (_game->chessboard)[*_k][(*_l) - 1] <= 115 && (_game->players)[1].pieces[temp].life >= 10 //���Ļ��� ������ ���Ļ� ���ɼ� (���� �������� 2ĭ�� ����������) �̸鼭 �� �� ���� �������
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
	else //�� ��
	{
		if (_onlyattack == 0)
		{
			if ((_game->chessboard)[(*_k) + 1][*_l] == 16 && ((*_k) + 1 < 8 && (*_l) < 8))
			{
				(_game->chessboard)[(*_k) + 1][*_l] += 1000;
				if ((_game->chessboard)[(*_k) + 2][*_l] == 16 && ((*_k) + 2 < 8 && (*_l) < 8) && *_k == 1) //�ѹ�¦�� �� ����������
				{
					(_game->chessboard)[(*_k) + 2][*_l] += 1000;
				}
			}
			if (*_k == 4) //���Ļ�
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

void promotion(chess* _game) //���θ�� �Լ�
{
	int i, j;
	char temp;
	for (i = 0; i < 2; i++)
	{
		for (j = 8; j < 16; j++)
		{
			if (i == 0 && (_game->players)[i].pieces[j].rank == 0)
			{
				printf("\n�������� �±�?\n");
			RE0:
				printf("Ű���忡�� ���ϴ� ���� �Է��� 'enter' ");
				scanf("%c", &temp);
				switch (temp)
				{
				case 'r': temp = 20; break;
				case 'n': temp = 21; break;
				case 'b': temp = 22; break;
				case 'q': temp = 23; break;
				default: goto RE0; break;
				}
				(_game->players)[i].pieces[j].life = 2; //���θ���� ���� life�� 2 (deadshow���� ���� ���� ó������ �ʱ� ����)
				(_game->chessboard)[((_game->players)[i].pieces[j].rank)++][(_game->players)[i].pieces[j].file] = temp;
				return;
			}
			if (i == 1 && (_game->players)[i].pieces[j].rank == 7)
			{
				printf("\n�������� �±��ұ��?\n");
			RE1:
				printf("Ű���忡�� ���ϴ� ���� �Է��� 'enter' ");
				scanf("%c", &temp);
				switch (temp)
				{
				case 'r': temp = 120; break;
				case 'n': temp = 121; break;
				case 'b': temp = 122; break;
				case 'q': temp = 123; break;
				default: goto RE1; break;
				}
				(_game->players)[i].pieces[j].life = 2; //���θ���� ���� life�� 2 (deadshow���� ���� ���� ó������ �ʱ� ����)
				(_game->chessboard)[((_game->players)[i].pieces[j].rank)--][(_game->players)[i].pieces[j].file] = temp;
				return;
			}
		}
	}
	return;
}

void enpassant(chess* _game, int* _k, int* _l) //���Ļ��� ��� Ư���� �������� �ʿ���
{
	int temp;
	if ((_game->chessboard)[*_k][*_l] >= 8 && (_game->chessboard)[*_k][*_l] <= 15 && *_k == 2)
	{
		temp = (_game->chessboard)[(*_k) + 1][*_l] % 100;
		if ((_game->players)[1].pieces[temp].life >= 10)
		{
			(_game->players)[1].pieces[temp].life = 0; //��븻 life ���̰�
			(_game->chessboard)[(*_k) + 1][*_l] = 16; //chessboard�� ������ �����(16)�� �־���
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

void castling(chess* _game, int* _k, int* _l) //ĳ���� �Լ� (ŷ�� ����������� �赵 ��������)
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

int checkattack(chess _game, int _k, int _l, int _attack) //��� ���� k,l�ε����� ���� �������� Ȯ���ϴ� �Լ�
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
				default:return -1; //����
				}
				if ((_game.chessboard)[_k][_l] >= 1000)
				{
					return 1; //���� ���ɼ� ����(����)
				}
				else
				{
					return 0; //���� ���ɼ� ����(����)
				}
			}
		}
	}
	return 0; //���� ���ɼ� ����(����)
}

int check(chess _game, int _onlycheck) //üũ Ȯ�� �Լ�
{
	m_thset(&_game);
	int king, i, j, k;
	for (king = 4; king <= 104; king += 100) //��ŷ >> ��ŷ ������� Ȯ��
	{
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 8; j++)
			{
				if ((_game.chessboard)[i][j] == king) //chessboard���� ŷ�� ã�Ƽ�
				{
					if (king == 4)
					{
						for (k = 100; k <= 115; k++) //����� ������ ���� �������� Ȯ��
						{
							if ((_game.players)[1].pieces[k % 100].life == 0)
							{
								continue;
							}
							if (checkattack(_game, i, j, k) == 1)
							{
								if (_onlycheck == 0) //üũ�� Ȯ���Ҳ��� ���⼭ �׸�
								{
									if (checkmate(_game, king) == 1) //üũ �̸� üũ����Ʈ ������ Ȯ��
									{
										return 2;
									}
								}
								return 1;
							}
						}
						for (k = 120; k <= 123; k++) //���θ������ ��Ƴ� ���鵵 Ȯ������
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

int checkmate(chess _game, int _king) //üũ����Ʈ ���� Ȯ�� (�� ������ 1ĭ�� ��� ������ ������ ��� üũ�̸� üũ����Ʈ)
{
	int i, j, flag;
	if (_king == 4)
	{
		flag = 0; //���϶�
		i = 0;
	}
	else
	{
		flag = 10; //���϶�
		i = 100;
	}
	j = i + 15;
	for (; i <= j; i++)
	{
		if (checkmate_check(_game, i) == 1)
		{
			return 0 + flag; //üũ����Ʈ�� �ƴ�
		}
	}
	for (i = j + 5; i <= j + 8; i++)
	{
		if (checkmate_check(_game, i) == 1)
		{
			return 0 + flag; //üũ����Ʈ�� �ƴ�
		}
	}
	return 1 + flag; //üũ����Ʈ��
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
		return 0; //���� ������ �����ټ� ����
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
	default:return -1; //����
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
					return 1; //üũ����Ʈ�� �ƴ�(�� ���� �����ټ� ����)
				}
			}
		}
	}
	return 0; //üũ����Ʈ��(�� ���� �����ټ� ����)
}