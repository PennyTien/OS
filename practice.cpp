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
#include <iostream>
using namespace std;

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

  void *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED)
    error_and_die("mmap");
  close(fd);

  pid_t pid1 = fork();
  pid_t pid2 = fork();


  if (pid1 == 0||pid2==0) 
{
	sleep(2);
	int *ID = (int *) ptr;
	if (ID[0] == getpid())
	{
		cout<<"\nI'm first child: "<<getpid()<<endl;
		cout<<pid1<<endl;
		cout<<pid2<<endl;
	}
	else if (ID[1]==getpid())
	{
		cout<<"\nI'm second child: "<<getpid()<<endl;
			cout<<pid1<<endl;
			cout<<pid2<<endl;
	}
	exit(0);
}
  else {
    int *ID = (int *) ptr;
    ID[0]=pid1;
    ID[1]=pid2;
    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    cout<<"Love you"<<endl;
  }

  r = munmap(ptr, region_size);
  if (r != 0)
    error_and_die("munmap");

  r = shm_unlink(memname);
  if (r != 0)
    error_and_die("shm_unlink");

  return 0;
}
