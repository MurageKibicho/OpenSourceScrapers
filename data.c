#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count = 0;
int over2 = 0;
int under2 =0;

int homeWins = 0;
int awayWins = 0;
int drawWins = 0;

void PrintScores(char *copy)
{
	int count = 0;
	int j = 0;
	while(count < 2)
	{
		if(copy[j] == ',')
		{
			count++;
		}
		else
		{
			printf("%c \t", copy[j]);
		}
		j++;
	}
}

void ProcessBuffer(char *buffer, int bufferSize,double homeSearch, double drawSearch, double awaySearch,double range)
{
	char *comma = ",";
	char *token;
	char *copy = strdup(buffer);
	int index = 0;
	token = strtok(buffer, comma);
	double homeGoals = 0;
	double awayGoals = 0;
	double homeOdds = 0;
	double drawOdds = 0;
	double awayOdds = 0;
	double over25 = 0;
	double under25 = 0;
	
	while(token != NULL)
	{
		if(index == 0)
		{
			homeGoals = strtod(token, NULL);
		}
		else if(index == 1)
		{
			awayGoals = strtod(token, NULL);
		}
		else if(index == 2)
		{
			homeOdds = strtod(token, NULL);
		}
		else if(index == 3)
		{
			drawOdds = strtod(token, NULL);
		}
		else if(index == 4)
		{
			awayOdds = strtod(token, NULL);
		}
		else if(index == 5)
		{
			over25 = strtod(token, NULL);
		}
		else if(index == 6)
		{
			under25 = strtod(token, NULL);
		}
		token = strtok(NULL,comma);
		index++;
	}
	
	if(homeOdds >= homeSearch-range && homeOdds <= homeSearch+range 
	&& awayOdds >= awaySearch-range && awayOdds <= awaySearch+range)
	//&& over25 >= overSearch-range && over25 <= overSearch+range)
	{
		//Predict Over under
		if((homeGoals + awayGoals)>=2.5)
		{
		over2++;	
		}
		else
		{
		under2++;
		}
		
		if(homeGoals == awayGoals)
		{
			drawWins++;
		}
		else if(homeGoals > awayGoals)
		{
			homeWins++;
		}
		else
		{
			awayWins++;
		}
		
		printf("%.0f %.0f HDA (%.2f : %.2f : %.2f), OU (%.2f : %.2f)\n",homeGoals,awayGoals, homeOdds, drawOdds, awayOdds, over25, under25);
		count++;
	}
		
}

void OpenFile(char *fileName, double homeSearch, double drawSearch, double awaySearch, double range)
{
	FILE *fp = fopen(fileName, "r");
	if(fp == NULL)
	{
		printf("\nFile does not exist\n");
		return;
	}
	int c = 0;
	int commaCount = 0;
	char *buffer = calloc(100,1);
	int index = 0;
	while((c=getc(fp)) != EOF)
	{
		if(c == '\n')
		{
			ProcessBuffer(buffer,strlen(buffer),homeSearch, drawSearch,awaySearch, range);
			commaCount = 0;
			memset(buffer,0,100);
			index = 0;
		}
		if(commaCount > 1)
		{
			buffer[index] = c;
			index++;
		}
		if(c == ',')
		{
			commaCount++;
		}
	}
	free(buffer);
	fclose(fp);
}

int main(int argc, char *argv[])
{
	double homeSearch = strtod(argv[1], NULL);
	double drawSearch = strtod(argv[2], NULL);
	double awaySearch = strtod(argv[3], NULL);
	double error = strtod(argv[4], NULL);
	//printf("\n%f %f %f %f",homeSearch,drawSearch,awaySearch,error);
	char *fileName = "data.csv";
	OpenFile(fileName,homeSearch,drawSearch,awaySearch,error);
	printf("Over: %d Under: %d Count: %d", over2, under2, count);
	printf("\nHome: %d Draw: %d Away: %d", homeWins, drawWins, awayWins);
	return 0;
}
