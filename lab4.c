#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

double factorial(int n)
{
    int result = 1;
    for(int i=1;i<=n;i++)
    {
        result *= i;
    }
    return result;
}

double pif1()
{
  double pif1=0;
  int t=1;
  for (int i=0; i<31; i++)
  {
    pif1 = pif1 + t/(pow(3,i)*(2*i+1));
    t=t*(-1);
  }
  pif1 =  pif1 * 2 * sqrt(3);
  return pif1;
}

double expf2(double x)
{
  double expf2 = 0;
  for (int i=1; i<31; i++)
  {
    expf2 = expf2 + (pow(x,i) / factorial(i));
  }
  expf2 =  1 + expf2;
  return expf2;
}

double lnf3(double x)
{
  double lnf3 = 0;
  for (int i=1; i<31; i++)
  {
    lnf3 = lnf3 + pow((x-1),i) / (i*pow(x,i));
  }
  return lnf3;  
}

int main(int argc , char *argv[] )
{
  double pif;
  int status;
  pid_t pid;
  if (argc != 2) 
    { 
      printf("введите значение x\n"); 
      exit(0); 
    } 
  
  // atoi converts string to integer 
  if (atoi( argv[1] ) <0 ) 
    { 
      printf("negative number entered %d", atoi(argv[1])); 
      exit(0); 
    } 
  pid = fork(); 
  switch(pid) 
  {
  case -1:
          perror("fork"); /* произошла ошибка */
          return -1; /*выход из родительского процесса*/
  case 0:
  {
    // atoi converts string to integer 
    int x = atoi(argv[1]);
    
    printf("CHILD: Это процесс-потомок!\n");
    printf("CHILD: Мой PID -- %d\n", getpid());
    printf("CHILD: PID моего родителя -- %d\n", getppid());
    pif = pif1();
    printf("CHILD: Значение pi:%f\n", pif);
    pid = fork();
    if (pid ==0)
    {
      printf("CHILD: Это процесс-потомок!\n");
      printf("CHILD: Мой PID -- %d\n", getpid());
      printf("CHILD: PID моего родителя -- %d\n", getppid());
      double lnf = lnf3(x);
      printf("CHILD: значение ln(x):%f\n", lnf);
      pid = fork();
      if (pid ==0)
      {
        printf("CHILD: Это процесс-потомок!\n");
        printf("CHILD: Мой PID -- %d\n", getpid());
        printf("CHILD: PID моего родителя -- %d\n", getppid());
        double expf = expf2(-((lnf * lnf) / 2));
        printf("CHILD: значение exp(x):%f\n", expf);
        pid = fork();
        if (pid == 0)
        {
          printf("CHILD: Это процесс-потомок!\n");
          printf("CHILD: Мой PID -- %d\n", getpid());
          printf("CHILD: PID моего родителя -- %d\n", getppid());
          double fx = expf / (2*(x*sqrt(2*pif)));
          printf("Значение функции при x>0  = %f\n",fx);
          exit(0);
        }
        exit(0);
      }
      exit(0);
    }
    exit(0);
  }
          
  default:
  {
      printf("PARENT: Это процесс-родитель!\n");
      printf("PARENT: Мой PID -- %d\n", getpid());
      printf("PARENT: PID моего потомка %d\n",pid);
      printf("PARENT: Я жду, пока потомок не вызовет exit()...\n");
      wait(&status);
      printf("PARENT: Выход!\n");
      exit(0);
  }
  
  }
  return 0;
}
  