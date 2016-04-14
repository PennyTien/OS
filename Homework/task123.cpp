/*
source: https://www.ibm.com/developerworks/aix/library/au-spunix_sharedmemory/
compile: gcc -o shm shm.c -lrt
exec: ./shm
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
using namespace std;

void *ptr;

void taskOne();
void taskTwo();

void error_and_die(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	int r;

	const char *memname = "sample";
	const size_t region_size = sysconf(_SC_PAGE_SIZE);

	int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd == -1)
		error_and_die("shm_open");

	r = ftruncate(fd, region_size);
	if (r != 0)
		error_and_die("ftruncate");

	ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED)
		error_and_die("mmap");
	close(fd);
	
	int task =0;
	cout<<"\nTask :";
	cin>>task;
	if (task == 1)
		taskOne();
	else if  (task ==2)
		taskTwo();
	else
		cout<<"Error！！！"<<endl;
	
	r = munmap(ptr, region_size);
	if (r != 0)
		error_and_die("munmap");

	r = shm_unlink(memname);
	if (r != 0)
		error_and_die("shm_unlink");

	return 0;

}

void taskOne()
{
	//begin task1
	int digit = 0;
	int A = 0;
	int B = 0;
	do
	{
		cout << "\nDigit number (less than 11):";
		cin >> digit;
		if (digit > 11 || digit < 1)
			cout << "Error!! \nPlease try again.\n";
	} while (digit > 11 || digit < 1);

	int *answer = new int[digit];

											//create the answer
	for (size_t i = 0; i < digit; i++)
	{
		answer[i] = rand() % 10;
		for (size_t k = 0; k < i; k++)
			if (answer[k] == answer[i])
			{
				i--;
				break;
			}	
	}

	//for (size_t i = 0; i < digit; i++)
	//	cout << answer[i];
	//cout << endl;

	//transform to string
	char *a = new char[digit];
	for (size_t i = 0; i < digit; i++)
	{
		a[i] = static_cast<char>(answer[i] + 48);
		//cout << a[i] << "  " << answer[i] << endl;
	}
	pid_t pid1 = fork();
	pid_t pid2 = fork();

	cin.ignore();
	do
	{
		if (pid1 == 0|| pid2 == 0)
		 {
			//sleep(2);

			//if (*((int*)ptr) == 1)
			//{
			int * s = (int*)ptr;
			int *ID =  (int*)ptr+15;
			//cout<<"Step2"<<endl;
			while ( 0  == s[0])
				;
			//cout<<"Step2"<<endl;
			if (*((int*)ptr) == -1)
				exit(0);
			else if ( ID[0] == getpid() )
			{
				//cout<<"Step3"<<endl;
				for (size_t i = 0; i < digit; i++)			//countA
					if (static_cast<int>(answer[i]) == s[i + 4])
						s[1]++;
				cout<<"["<<getpid()<<" Anser "<<s[3]<<"]: "<<s[1]<<"A"<<endl;
				s[0] = 2;
				while(s[0]==2)
					;
			}
			else if (ID[1] == getpid())
			{		
				while(*((int*)ptr) != 2)
					;	
				//cout<<"Step4"<<endl;					//count B
				for (size_t i = 0; i < digit; i++)			
					for (size_t j = 0; j < digit; j++)
						if (static_cast<int>(answer[i]) == s[j + 4] && i != j)
							s[2]++;
				cout<<"["<<getpid()<<" Anser "<<s[3]<<"]: "<<s[2]<<"B"<<endl;
				*((int*)ptr) =0;
			}
			else
				exit(0);;
			//cout<<"Step5"<<endl;
			if (s[1] == digit && s[2] == 0)
				exit(0);
		}
		else {
			int *ID = (int *) ptr+15;
			ID[0] = pid1;
			ID[1] = pid2;
			char *user = new char[100];
			cout << "\n\n[" << getpid() << " User] :";

			cin.getline(user, 100);
			//cout<<user<<endl;
			if (static_cast<int>(user[0]) == 0)
			{
				cout<<"The answer: ";
				for (size_t i = 0; i < digit; i++)
              				  cout << answer[i];
      				cout << endl;
				*((int *)ptr) = -1;
				return;
			}
			else
			{
				int * temp = (int *)ptr;
				temp[1] = temp[2] = 0;
				temp[3]++;
				for (int i = 0; i < digit; i++)
					temp[i + 4] = user[i] - 48;
				*((int *)ptr) = 1;
			}

			while (*(int *)ptr != 0)
				;
			int * s = (int*)ptr;

			cout << "[" << getpid() << " Main " << s[3] << " ] : " << s[1] << "A" << s[2] << "B";

			if (s[1] == digit && s[2] == 0)
			{
				cout << " Done." << endl;
				break;
			}
		}

	} while (true);
	//cout << "><" << endl;
}

void taskTwo()
{
        int digit = 0;
        int A = 0;
        int B = 0;
        do
        {
                cout << "\nDigit number (less than 11):";
                cin >> digit;
                if (digit > 11 || digit < 1)
                        cout << "Error!! \nPlease try again.\n";
        } while (digit > 11 || digit < 1);

        int *answer = new int[digit];
        // srand(time(0));                                                                                //create the answer
        for (size_t i = 0; i < digit; i++)
                answer[i] = rand() % 10;
      

             // for (size_t i = 0; i < digit; i++)
              //	 cout << answer[i];
     	//cout << endl;
                                                                                                        //transform to string
        char *a = new char[digit];
        for (size_t i = 0; i < digit; i++)
        {
                a[i] = static_cast<char>(answer[i] + 48);
                //cout << a[i] << "  " << answer[i] << endl;
        }
	
        pid_t pid1 = fork();
        pid_t pid2 = fork();

        cin.ignore();

        do
        {
                if (pid1 == 0 || pid2 == 0) 
                {								//child
                        	int * s = (int*)ptr;
		int *ID =  (int*)ptr+15;

		while ( 0  == s[0])
			;
		//cout<<"Step 2:"<<endl;
                           int *temp = new int[digit];
                           //for (int i = 0; i < digit; ++i)
                           //             temp[i] = answer[i];
                           if (*((int*)ptr) == -1)
			exit(0);
		else if ( ID[0] == getpid())
                           {
                           	for (size_t i = 0; i < digit; i++)                        //countA
                                       	if (static_cast<int>(temp[i]) == s[i + 4])
                                               	 {
                                                     	   s[1]++;
                                                     	   temp[i] = -1;
                                            	 }
                                 	cout<<"["<<getpid()<<" Anser "<<s[3]<<"]: "<<s[1]<<"A"<<endl;
			s[0] = 2;
			while(s[0]==2)
				;
                           }
                           else if ( ID[1] == getpid())     
                          {					//count B
                          	while(*((int*)ptr) != 2)
					;
			for (size_t i = 0; i < digit; i++)              
                                       	if (temp[i] == s[i + 4])
                                                     {
                                                     	cout<<"A : "<<i<<": "<<temp[i]<<endl;
                                                     	temp[i] = -1;
                                                     	s[i+4] = -2;
                                                     }
                                            	 
                          	for (size_t i = 0; i < digit; i++)                        
                                       	for (size_t j = 0; j < digit; j++)
                                              		  if (static_cast<int>(temp[i]) == s[j + 4] &&  s[j + 4]>0 && static_cast<int>(temp[i]) >0 )
                                              		  {        
                                                   			s[2]++;
                                                   			cout<<"temp"<<i<<":"<<temp[i]<<"=="<<"!"<<s[j+4]<<endl;;
                                                   			temp[i] = -1;
                                                   			break;
                                             		   }
                                    	cout<<"["<<getpid()<<" Anser "<<s[3]<<"]: "<<s[2]<<"B"<<endl;
			*((int*)ptr) =0;
                          }		
                           else
			exit(0);;
		

		if (s[1] == digit && s[2] == 0)
		{
			exit(0);
                       	 }
                 
                }										//parents
                else {

                      	int *ID = (int *) ptr+15;
		ID[0] = pid1;
		ID[1] = pid2;
		char *user = new char[100];
                        do
                        {
                                        cout << "\n[" << getpid() << " User] : ";


                                        cin.getline(user, 100);
			if  (static_cast<int>(user[0]) == 0)
			{
				cout<<"\nThe answer: ";
				for (size_t i = 0; i < digit; i++)
              				  cout << answer[i];
      				cout << endl<<endl;
				*((int *)ptr) = -1;
				return;
			}
                                        else if (strlen(user)<digit)
                                                cout<<"The number you insert is too short, please try again."<<endl;
                                        else if (strlen(user)>digit)
                                                cout<<"The number you insert is too long, please try again."<<endl;
			
                        	}while(strlen(user)!=digit);

                       
                      	  int * temp = (int *)ptr;
                      	  temp[1] = temp[2] = 0;
                      	  temp[3]++;
                      	  for (int i = 0; i < digit; i++)
            	           		temp[i + 4] = user[i] - 48;

                       	 *((int *)ptr) = 1;

                        	while (*(int *)ptr != 0)
                          	;

                   	int * s = (int*)ptr;
                      	cout << "[" << getpid() << " Main " << s[3] << " ] : " << s[1] << "A" << s[2] << "B";

                      	  if (s[1] == digit && s[2] == 0)
                      	  {
                            	cout << " Done." << endl;
                      	         	return;
                      	  }
			cout<<endl;
                }
        } while (true);
}
