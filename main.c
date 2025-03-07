#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MODE_DEFAULT         0
#define MODE_SIMPLEBLACKJACK 1

#define RANDOM_CARD random_num_range(1, 13)

int starting_money = 0;

bool isnum(char* str)
{
	int str_len = strlen(str);
	if (str_len <= 0)
		return false;
	
	for (int i = str[0] == '-'; i < str_len; i++)
		if (str[i] < '0' || str[i] > '9')
			return false;
	
	return true;
}

int random_num_range(int min, int max)
{
	return rand() % ((max-min)+1)+min;
}

int random_num(int max)
{
	return rand() % (max + 1);
}

int current_money = 0;

bool get_money_to_gamble(char* input, int* money_to_gamble)
{
	int input_len = strlen(input);
	
	if (input[input_len-1] == '%')
	{
		input_len -= 1;
		input[input_len] = 0;
		
		if (!isnum(input))
		{
			printf("Invalid percentage\n");
			return false;
		}
		
		*money_to_gamble = atoi(&input[input[0] == '-']);
		
		if (*money_to_gamble <= 0 || *money_to_gamble > 100)
		{
			printf("Invalid percentage\n");
			return false;
		}
		
		*money_to_gamble = current_money * ((float)*money_to_gamble / 100) * (input[0] == '-' ? -1 : 1);
	}
	else if (!isnum(input))
	{
		printf("Input is not a number!\n");
		return false;
	}
	else
	{
		*money_to_gamble = atoi(input);
	}
	
	// Idk kinda intresting to bet if you're gonna lose lol
	if (*money_to_gamble > current_money)
	{
		printf("You can't bet more money than you have.\n");
		return false;
	}
	
	return true;
}

char run_default()
{
	return random_num(1) == 0;
}

char run_sbj()
{
	char dealerhand[18];
	char dealerhand_count = 1;
	char playerhand[18];
	char playerhand_count = 2;
	
	dealerhand[0] = RANDOM_CARD;
	
	playerhand[0] = RANDOM_CARD;
	int playertotal = 0;
	do 
	{
		playerhand[1] = RANDOM_CARD;
		playertotal = playerhand[0] + playerhand[1];
	} while (playertotal > 21);
	
	
	char sbj_input[32];
	
	while (true)
	{
		playertotal = 0;
		int dealertotal = 0;
		if (dealerhand_count == 1)
		{
			printf("Dealer: %d - ? (%d)\n", dealerhand[0], dealerhand[0]);
		}
		else
		{
			printf("Dealer: ");
			for (int i = 0; i < dealerhand_count; i++)
			{
				dealertotal += dealerhand[i];
				
				printf("%d", dealerhand[i]);
				if (i != dealerhand_count - 1)
				{
					printf(" - ");
				}
			}
			printf(" (%d)\n", dealertotal);
		}
		
		printf("Player: ");
		for (int i = 0; i < playerhand_count; i++)
		{
			playertotal += playerhand[i];
			
			printf("%d", playerhand[i]);
			if (i != playerhand_count - 1)
			{
				printf(" - ");
			}
		}
		printf(" (%d)\n", playertotal);
		
		printf("(h)it or (s)tand? ");
		scanf("%31s", sbj_input);
		
		if (sbj_input[0] == 'h')
		{
			playerhand[playerhand_count] = RANDOM_CARD;
			playertotal += playerhand[playerhand_count++];
		}
		if (sbj_input[0] == 's' || playertotal > 21)
		{
			while (dealertotal <= 16)
			{
				dealerhand[dealerhand_count++] = RANDOM_CARD;
				dealertotal = 0;
				
				for (int i = 0; i < dealerhand_count; i++)
				{
					dealertotal += dealerhand[i];
				}
			}
			printf("Final scores:\n");
			printf("Dealer: ");
			for (int i = 0; i < dealerhand_count; i++)
			{
				printf("%d", dealerhand[i]);
				if (i != dealerhand_count - 1)
				{
					printf(" - ");
				}
			}
			printf(" (%d)\n", dealertotal);
			printf("Player: ");
			for (int i = 0; i < playerhand_count; i++)
			{
				printf("%d", playerhand[i]);
				if (i != playerhand_count - 1)
				{
					printf(" - ");
				}
			}
			printf(" (%d)\n", playertotal);
			
			if ((playertotal > 21 && dealertotal > 21) || playertotal == dealertotal)
			{
				return 2; // tie
			}
			else if (playertotal > 21 || (dealertotal <= 21 && dealertotal > playertotal))
			{
				return 0;
			}
			else
			{
				return 1;
			}	
		}
	}
}

int main(int argc, char** argv)
{
	if (argc < 2 || !isnum(argv[argc-1]))
	{
		printf("Expected usage: %s (flags) [Starting Money]\n", argv[0]);
		printf("Flags:\n    -sbj = Simple Blackjack (No card deck logic)\n");
		return 1;
	}

	printf("Welcome to Gambux, where you can gamble fake money!\n");
	printf("You can bet a percentage of your money by adding %% to your input\nYou can also bet negative numbers/percentages to bet on yourself losing\n\n");
	printf("When you want to stop type \"stop\" into the output\n");
	
	int current_mode = MODE_DEFAULT;
	
	for (int i = 1; i < argc-1; i++)
	{
		if (strcmp(argv[i], "-sbj") == 0)
		{
			current_mode = MODE_SIMPLEBLACKJACK;
		}
	}
	
	char(*run_game)();
	switch(current_mode)
	{
		case MODE_DEFAULT:
			printf("This is the default mode in gambux, basically every time you bet you have a 50%% chance of winning.\n");
			run_game = run_default;
			break;
		case MODE_SIMPLEBLACKJACK:
			printf("This is a simple implementation of blackjack, simple meaning there is no card deck logic it's just random numbers.\n");
			run_game = run_sbj;
			break;
		default:
			printf("Mode %d is not implemented!\n", current_mode);
			return 1;
	}
	
	starting_money = atoi(argv[argc-1]);
	
	char input[32];
	int money_to_gamble = 0;
	current_money = starting_money;
	time_t start_time = time(NULL);
	srand(start_time);
	while (true)
	{
		printf("Input: ");
		scanf("%31s", input);
		
		if (strcmp(input, "stop") == 0)
		{
			printf("Stopping the games, you started with %d money and ended with %d after %ld seconds!\n", starting_money, current_money, time(NULL) - start_time);
			break;
		}
		
		if (!get_money_to_gamble(input, &money_to_gamble))
			continue;
		
		char game_result = run_game();
		if (game_result == 0)
		{
			current_money -= money_to_gamble;
			printf("Oopsies you lost %d money! ", money_to_gamble);
		}
		else if (game_result == 1)
		{
			if (money_to_gamble > 2147483647 - current_money)
			{
				printf("Congratulations you won enough money to go over the integer limit (2147483647), you win gambling!\n");
				break;
			}
			
			current_money += money_to_gamble;
			printf("Congratulations you won %d money! ", money_to_gamble);
		}
		else // TIE
		{
			printf("No money was lost or won!\n");
		}
		printf("You now have %d money.\n\n", current_money);
		
		money_to_gamble = 0;
		
		if (current_money <= 0)
		{
			printf("It seems like you lost all your money, it only took you %ld seconds!\n", time(NULL) - start_time);
			break;
		}
	}

	return 0;
}