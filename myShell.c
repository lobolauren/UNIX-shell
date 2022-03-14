#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <sys/types.h>   /* Primitive System Data Types */
#include <sys/wait.h>    /* Wait for Process Termination */
#include <errno.h>       /* Errors */
#include <string.h>      /* Strings */
#include <signal.h>      /*signal*/
/*
 *   CIS*3110 Assignment1
 *      Program: myShell
 *      Author: Lauren Lobo 1087364
 *      Date of Last Revision: 01/22/2021
 *      References: Based on code found at http://www.amparo.net/ce155/fork-ex.html
 *      compile: gcc -std=gnu99 -Wpedantic -o myShell myShell.c
 *      run: ./myShell
 */

/*Global variables to set directory in terminal and count for done message*/
static int count =0;
char *home;
char *directory;

/*handling sigquit functionality */
void sigquit(int signo) {
    wait(NULL);
    printf("[%d]+ Done\n", count);
    count--;
} 

/*function to split arguments in command line*/
char **tolkenize_input(char *line){
  char *token;
  int i = 0;
  token = strtok(line, " ");
  int buffer = 10000;
  char **tokens = malloc(sizeof(char*)*buffer);
  while (token != NULL) {
    tokens[i] = token;
    i+=1;
    if (i >= buffer) {
      buffer += 1000;
      tokens = realloc(tokens, buffer * sizeof(char*));
    }
    token = strtok(NULL, " ");
  }
  tokens[i] = NULL;
  return tokens;
}
 
/*get current working directory*/ 
char *getCWD(){
    char *temp = malloc(sizeof(char)*100);
    getcwd(temp,100);
    return temp;
}

void pathFunc(char **com){
   int breakflag = 0;
   int breakflag2 = 0;
   int i=0;
   char **arg;
   char *str =malloc(256);
   for (i=0; (com[i]!= NULL) && (breakflag == 0);i++){
        if (strcmp(com[i], "$PATH\n")==0){
            breakflag =1 ; /*break out of flag to hold i as position*/
        }
   }
   for (i=0; (com[i]!= NULL) && (breakflag2 == 0);i++){
        if (strcmp(com[i], "$HISTFILE\n")==0){
            breakflag2 =1 ;
        }
   }
   if(breakflag==1){
      FILE* file = fopen(".CIS3110_profile", "r"); /* should check the result */
    char * line = malloc(256);
      while (fgets(line,256, file)) {
         arg = tolkenize_input(line);
         if(strstr(arg[1],"PATH")!=NULL){
            strcpy(str,arg[1]);
            str+=5;
         }
      }
      printf("%s\n",str);
      fclose(file);
   }if(breakflag2==1){
       FILE* file = fopen(".CIS3110_profile", "r"); /* should check the result */
       char * line = malloc(256);
       while (fgets(line,256, file)) {
         arg = tolkenize_input(line);
         if(strstr(arg[1],"HOME")!=NULL){
            strcpy(str,arg[1]);
            str+=5;
         }
      }
      strtok(str,"\n");
      strcat(str,"/.CIS3110_history"); /*add path to history*/
      printf("%s\n",str);
      fclose(file);
   }
}

void homeFunc(char **com){
   int breakflag = 0;
   int i=0;
   char **arg;
   char *str =malloc(256);
   for (i=0; (com[i]!= NULL) && (breakflag == 0);i++){
        if (strcmp(com[i], "$HOME\n")==0){
            breakflag =1 ;
        }
   }
    FILE* file = fopen(".CIS3110_profile", "r"); /* should check the result */
    char * line = malloc(256);
    while (fgets(line,256, file)) {
        arg = tolkenize_input(line);
        if(strstr(arg[1],"HOME")!=NULL){
           strcpy(str,arg[1]);
           str+=5;
        }
    }
    printf("%s\n",str);

    fclose(file);
}
/*file to save history to history file*/
void saveHistory(char *line){
   
   FILE * fp;
   fp = fopen (".CIS3110_history","ab+");
   fputs(line,fp);
   fclose (fp);
}

/*function called take in argument ist and runs as commands*/
int run_commands(char **com){
   char *cmd = strtok(com[0]," \n");
   char *argv[20];
   argv[0] = cmd;
   int j=0;

         while(com[j]!= NULL){
            com[j] = strtok(com[j]," \n");
            argv[j] = com[j];
        
            j++;
         }
         argv[j] = NULL;
 
         int status_code = execvp(cmd, argv);
      
         if (status_code == -1) {
            printf("bash: %s: command not found \n",com[0]);
         }
 
   return 1;
}

/*checks for piping*/
int isPiping(char **com){
   int o =0;
      for(o=0; com[o] != NULL;o++){
          if(strcmp(com[o],"|")==0){
             return 1;
          }
         }
   return 0;
}
 
 /*checks for path*/
int isPath(char **com){
   int o =0;
      for(o=0; com[o] != NULL;o++){
          if(strcmp(com[o],"$PATH\n")==0){
             return 1;
          }
         }
   return 0;
}

 /*checks for home*/
int isHome(char **com){
   int o =0;
      for(o=0; com[o] != NULL;o++){
          if(strcmp(com[o],"$HOME\n")==0){
             return 1;
          }
         }
   return 0;
}
/*checks for file redirection*/
int isOutputRedirection(char **com){
   int o =0;
      for(o=0; com[o] != NULL;o++){
          if(strcmp(com[o],">")==0){
             return 1;
          }
         }
   return 0;
}
/*checks for history command*/
int isHistory(char **com){
   int o =0;
      for(o=0; com[o] != NULL;o++){
          if(strcmp(com[o],"$HISTFILE\n")==0){
             return 1;
          }if(strcmp(com[o],"history\n")==0||strcmp(com[o],"history")==0){
             return 2;
          }
      }
   return 0;
}

/*checks for change directory mode*/
int isCD(char **com){
   int o =0;
      for(o=0; com[o] != NULL;o++){
          if(strcmp(com[o],"cd")==0){
             return 1;
          }
      }
   return 0;
}
  
  /*checks for file input redirection*/
int isInputRedirection(char **com){
   int o =0;
      for(o=0; com[o] != NULL;o++){
          if(strcmp(com[o],"<")==0){
             return 1;
          }
         }
   return 0;
}


int redirectFromFile(char **com){
   FILE *fp;
   char *newcommands[20];
   int i =0;
   int breakflag = 0;
   char *filename;
 
   for (i=0; (com[i] != NULL) && (breakflag == 0);i++){
        if (strcmp(com[i], "<")==0){
            breakflag =1 ;  
        }
   }
   if(com[i] != NULL){
      filename = strtok(com[i]," \n");
   }else{
      printf("input error: \n");
   }
   int h=0;
   for(h=0;h<(i-1);h++){
      newcommands[h]= com[h];
   }
   newcommands[h] =  NULL;
   fp = freopen(filename,"r",stdin);
   run_commands(newcommands);
   fclose(fp);
   return 0;
}
 
int redirectToFile(char **com){
   FILE *fp;
   char *newcommands[20];
   int i =0;
   int breakflag = 0;
   char *filename;
   for (i=0; (com[i]!= NULL) && (breakflag == 0);i++){
        if (strcmp(com[i], ">")==0){
            breakflag =1 ;
        }
   }
   if(com[i] != NULL){
      filename = strtok(com[i]," \n");
   }else{
      printf("Filename error\n");
   }
   int h=0;
   for(h=0;h<(i-1);h++){
      newcommands[h]= com[h];
   }
   newcommands[h] =  NULL;
   fp =freopen(filename,"w",stdout);
   run_commands(newcommands);
   fclose(fp);
   return 0;
}
 
 
int piping (char **args){
    int flag = 0;
    char * array1[100];
    char * array2[100];
    int p[2];
    int i =0;
    int j=0;
    int s=0;
   while (args[s]!=NULL){
        s ++;
        strtok(args[s]," \n");
    }

    for (i=0;(flag==0)&&(i<s);i++){
        if (strcmp(args[i], "|")==0)
            flag = 1;
    }
 
   for(j=0;j<i-1;j++){
      array1[j] = args[j];
   }
   array1[j] = NULL;
   int d=0;
   int w =0;
      for(d = i ;args[d]!= NULL;d++){
         array2[w] = args[d];
         w++;
      }
   
   array2[w]=NULL;


    if (pipe(p)){
        printf(": Fork Failed\n");
   }
   pid_t pid =fork();
   if (pid == 0){
      close(STDOUT_FILENO);
      dup(p[1]);
      close(p[0]);
      close(p[1]);
      run_commands(array1);
      exit(1);
    }else {
      close(STDIN_FILENO);
      dup(p[0]);
      close(p[1]);
      close(p[0]);
      run_commands(array2);
      exit(1);
    }
    close(p[0]);
    close(p[1]);
    wait(0);
    wait(0);
    return 1;
}

void cdFunc(char **com){
   int breakflag = 0;
   int i=0;
   for (i=0; (com[i]!= NULL) && (breakflag == 0);i++){
        if (strcmp(com[i], "cd")==0){
            breakflag =1 ;
        }
   }
   char * args = com[i];
   char * directNew = malloc(256);
   args = strtok(args," \n");
   args[strlen(args)+1]='\0';
   if(strcmp(com[i],"~")==0){
      chdir(home);
      directNew = getCWD();
   }else{
       chdir(args);
       directNew = getCWD();
   }
   directory = directNew;
}


void forking_process(pid_t childpid, int status, char *line, const char *s, char* token,char **com){
      
    struct sigaction sigact;
    sigact.sa_handler = SIG_DFL;
    int flag =0;
    int processcount = 0;
    int flag2 =0;
      childpid = fork();
      if ( childpid >= 0 ) {   /* fork succeeded */
         if ( childpid == 0 ) { /*child process*/
            int o =0;
            int flag =0;
            for(o=0; com[o] != NULL;o++){
               if(strcmp(com[o],"&\n")==0){
                  flag =1;
                  com[o]=NULL;
               }
            }
            if(flag==1){
               run_commands(com);             
            }if(isOutputRedirection(com)==1){
               redirectToFile(com);
            }if(isInputRedirection(com)==1){
               redirectFromFile(com);
            }if(isPiping(com)==1){
               piping(com);
            }if(isPath(com)==1){
               pathFunc(com);
            }if(isHistory(com)==1){
               pathFunc(com);
            }if(isCD(com)==1){
               flag2 =2;
            }if(isHistory(com)==2){
               FILE *fptr; 
               char c; 
               int h=0;
               int num =0;
               int histflag =0;
               flag2 =1;
               for(h=0;com[h]!= NULL &&(histflag==0);h++){
                  if((strcmp(com[h],"-c\n")==0)|| strcmp(com[h],"-c")==0){
                     fclose(fopen(".CIS3110_history", "w"));
                     histflag=1;
                  }
               }
               if(histflag==0){
                  fptr = fopen(".CIS3110_history", "r"); 
                  if (fptr == NULL){ 
                     printf("Cannot open file \n"); 
                  } 
                  c = fgetc(fptr); 
                  num++;
                  printf( " %d ",num);
                  while (c != EOF) { 
                  
                     printf ("%c", c); 
                     c = fgetc(fptr);
                     if('\n' == c){
                        num++;
                        printf( "\n %d ",num);
                        c = '\0';
                     } 
                  } 
                  
                  fclose(fptr); 
               }
            }if(isHome(com)==1){
               homeFunc(com);
            }if(flag2 ==0){
               run_commands(com);
            }
            exit(status);
         }else{    /*Parent Process*/
            int o =0;
            for(o=0; com[o] != NULL;o++){
               if(strcmp(com[o],"&\n")==0){
                  flag =1;
                  count++;
                  com[o]=NULL; 
               }
            }
           if(isCD(com)==1){
               cdFunc(com);

            }
            if(flag==1){
               processcount+=1;
               printf("[%d] %d\n",processcount,childpid);
              // sigact.sa_handler = sigquit;
               //sigact.sa_handler = SIG_DFL;
               sigaction(SIGCHLD,&sigact,NULL);
               waitpid(childpid,&status,WNOHANG);
               
            }else{
              // sigact.sa_handler = SIG_DFL;
              // flag = 0;
               waitpid(childpid,&status,0);
            }
       } 
      }else {
         perror("fork");
         exit(-1);
      }
}
 

int main ( int argc, char *argv[] ) {
   /*Variable Declarations*/
   pid_t childpid = 0;   /* child's process id */
   int status = 0;   /* for parent process: child's exit status */
   char *line;
   FILE *fp = fopen(".CIS3110_profile","r");
   char * filelines = malloc(256);
   char *string = malloc(256);
   char **arg;
    while (fgets(filelines,256, fp)) {
        arg = tolkenize_input(filelines);
        if(strstr(arg[1],"HOME")!=NULL){
           strcpy(string,arg[1]);
           string+=5;
        }
    }
    string = strtok(string,"\n");
    home = string;
    directory = getCWD();


   const char s[2] = " ";
   char *token= NULL;
   size_t length = 0;
   line = (char *)malloc(256);
   char **returninput;
   printf("%s >",directory);
   char *inputline = line;
   getline(&inputline, &length, stdin);
   returninput = tolkenize_input(inputline);
   while ( (strncmp(returninput[0],"exit",4) != 0) && (strncmp(returninput[0],"quit",4) != 0)  ) {
      
      forking_process(childpid,status,line,s,token,returninput);
      printf("%s >",directory);
      getline(&inputline, &length, stdin);
      saveHistory(inputline);
      returninput = tolkenize_input(inputline);
   }
   printf("myShell terminating...\n\n[Process completed]\n");
   free(line);
   fclose(fp);
   free(inputline);
   free(token);
   exit(0);
}