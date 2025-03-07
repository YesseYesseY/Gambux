#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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

int random_num(int max)
{
	return rand() % (max + 1);
}

int main(int argc, char** argv)
{
	if (argc != 2 || !isnum(argv[1]))
	{
		printf("Expected usage: %s [Starting Money]\n", argv[0]);
		return 1;
	}
	
	starting_money = atoi(argv[1]);
	
	printf("starting_money = %d\n", starting_money);
	
	printf("Welcome to Gambux, where you can gamble fake money!\n"
		   "Please input how much you want to gamble\n"
		   "You can add %% at the end to bet a percentage of your money or bet negative numbers to bet on yourself losing\n");
	
	char input[32];
	int money_to_gamble = 0;
	int current_money = starting_money;
	time_t start_time = time(NULL);
	srand(start_time);
	while (true)
	{
		printf("Input: ");
		scanf("%31s", input);
		
		int input_len = strlen(input);
		
		if (input[input_len-1] == '%')
		{
			input_len -= 1;
			input[input_len] = 0;
			
			if (!isnum(input))
			{
				printf("Invalid percentage\n");
				continue;
			}
			
			money_to_gamble = atoi(&input[input[0] == '-']);
			
			if (money_to_gamble <= 0 || money_to_gamble > 100)
			{
				printf("Invalid percentage\n");
				continue;
			}
			
			money_to_gamble = current_money * ((float)money_to_gamble / 100) * (input[0] == '-' ? -1 : 1);
		}
		else if (!isnum(input))
		{
			printf("Input is not a number!\n");
			continue;
		}
		else
		{
			money_to_gamble = atoi(input);
		}
		
		// Idk kinda intresting to bet if you're gonna lose lol
		if (money_to_gamble > current_money)
		{
			printf("You can't bet more money than you have.\n");
			continue;
		}
		
		if (random_num(1) == 0)
		{
			if (money_to_gamble > 2147483647 - current_money)
			{
				printf("Congratulations you won enough money to go over the integer limit (2147483647), you win gambling!\n");
				break;
			}
			
			current_money += money_to_gamble;
			printf("Congratulations you won %d money! ", money_to_gamble);
		}
		else
		{
			current_money -= money_to_gamble;
			printf("Oopsies you lost %d money! ", money_to_gamble);
		}
		printf("You now have %d money.\n", current_money);
		
		money_to_gamble = 0;
		
		if (current_money <= 0)
		{
			printf("It seems like you lost all your money, it only took you %ld seconds!\n", time(NULL) - start_time);
			break;
		}
	}

	return 0;
}