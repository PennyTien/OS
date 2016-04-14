#include <iostream>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
using namespace std;

void parent(char *, int);
void child(char *, int, char*, int*, int &);

int main()
{
	int digit = 0;
	
	do
	{
		cout << "Digit number (less than 10):";
		cin >> digit;
		if (digit > 10 || digit < 1)
			cout << "Error!! \nPlease try again.\n";
	} while (digit > 10 || digit < 1);

	int *answer = new int[digit];

	answer[0] = rand() % 9 + 1;						//create the answer
	for (size_t i = 1; i < digit; i++)
	{
		answer[i] = rand() % 10;
		for (size_t k = 0; k < i; k++)
			if (answer[k] == answer[i])
			{
				i--;
				break;
			}
	}
	for (size_t i = 0; i < digit; i++)
	{
		cout << answer[i];
	}
	cout << endl;
	//transform to string
	char *a = new char[digit];
	for (size_t i = 0; i < digit; i++)
	{
		a[i] = static_cast<char>(answer[i]+48);
		//cout << a[i] << "  " << answer[i] << endl;
	}

	parent(a, digit);
	system("pause");
}

void parent(char *anser, int digit)
{
	int counter = 0;
	int ab[2] = { 0 };
	char *user = new char[1000];
	cin.ignore();
	do
	{
		do
		{
			cout << "\nUser:";
			
			cin >> user;
			
			if (static_cast<int>(user[0]) == 0)
				return;
			else if (strlen(user) != digit)
				cout << "Error!!\nPlease try again.\n";

		} while (strlen(user) != digit);

		child(anser, digit, user, ab, counter);
		cout<<counter << "  Main:" << ab[0] << "A" << ab[1] << "B";
	} while (!(ab[0] == digit && ab[1] == 0));
	cout << " Done.";

	
}

void child(char *anser, int digit, char *user, int *ab, int &counter)
{
	ab[0] = ab[1] = 0;
	for (size_t i = 0; i < digit; i++)			//count B
		for (size_t j = 0; j < digit; j++)
			if (anser[i] == user[j] && i!=j)
				ab[1]++;

	for (size_t i = 0; i < digit; i++)			//countA
		if (anser[i] == user[i])
			ab[0]++;
		
		counter++;
		cout<<counter<<"  Anser:" << ab[0] << "A" << ab[1] << "B" << endl;
}
