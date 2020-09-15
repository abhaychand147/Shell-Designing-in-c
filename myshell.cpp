#include <sys/wait.h>
#include<readline/readline.h>
#include<sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include<readline/history.h>
#include<time.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<dirent.h>
#define MAX 128
#define delimeter " \t\r\n\a"

void command_cd(char *com,int length)   //Function to implement "cd" command
{
if(length>=3)                           //checking the number of arguements passed in terminal
printf("too many arguements!\n");       //If exceeds then print too many arhuements!
else if(chdir(com))                     //It changes the directory and returns zero and the condition becomes false so err message will not get printed
printf("Directory doesn't Exist!\n");
}

int hidden(char * filename)            //function for checking the hidden files
{
    return filename[0] == '.' &&       //checking if the filename starts with '.' or filename is not ',' and '..'
         strcmp(filename, ".") != 0 &&
         strcmp(filename, "..") != 0;
}

void command_ls()  //function for implementing "ls -l" command
{
struct dirent **list;
int n;
n=scandir(".",&list,NULL,alphasort); 
while (n--)                            //loop for printing all the files and its information
{ 
      printf("%s\n",list[n]->d_name);
      free(list[n]);  
}
free(list);
}

void command_ls1()    //function for implementing "ls" command
{
struct dirent **list;
int n;
//n=scandir(".",&list,NULL,alphasort); 
n=scandir(".",&list,NULL,alphasort);   //storing the number of files/directories
while (n--)                             //loop for printing all the directories and files
  { 
    if(!hidden(list[n]->d_name))
      printf("%s\n",list[n]->d_name); 
    free(list[n]); }
     free(list); 
   }
void command_cp(char * com,char * com1,int cn)    //Function for implementing "cp" command. It is used to copy content of one file to another
{
  FILE *fp1,*fp2;                                 //declaring file pointers
  char content;
  struct stat buff1,buff2;                        //declaring two buffers to store information about files
  stat(com,&buff1);                               //obtains the info about source file and stores in the buff1
  stat(com1,&buff2);                              //obtains the info about destination file and stores in the buff2
  fp1=fopen(com,"r");                             //opening source file in read mode
  fp2=fopen(com1,"w");                            //opening destination file in write mode
  if(fp1==NULL)  
  {
    printf("Source File doesn't exist!\n");
  }
  else if(fp2==NULL)
  {
    printf("Destination File Doesn't exist!\n");
  }
  else
  {
    if(cn || (!cn && ctime(&buff1.st_mtime)>ctime(&buff2.st_mtime))){  //checking the modified time of both the files
    content=fgetc(fp1);                                                //copying the file content into "content" if condition holds
    while(content!=EOF)
    {
      fputc(content,fp2);                                              //putting the content into destination file
      content=fgetc(fp1);
    }
  }
  fclose(fp1);
  fclose(fp2);
  }
}
void command_cat(char * com,int cn)                                   //Function to implement "cat" command! To display contents
{
  int l_num=1;
  char check;
  FILE *fp;                                                          //declaring file pointer
  char display;
  fp=fopen(com,"r");                          
  if(fp==NULL){                                                      //checking if the file exist! if not print the error message.
    printf("File doesn't exist!\n");
  }
  else{
   display=fgetc(fp);                                                //getting the first character of file
   if(!cn)                                                           // checking the "cn" variable if 0 then print the line number i.e. execute "cat -n " command
   {
    printf("%d  ",l_num);                                            //printing line number
   }
   while(display!=EOF){                                              //checking if end of file is reached
    if(display!='\n' || (display=='\n' && cn ))                      //checking if character is not next line i.e. \n and "cn=0" then simply display the contents
      {printf("%c",display);}
    display=fgetc(fp);
    if(!cn){                                                          //implementing "cat -n" function
      if(display=='\n'){
            l_num++;
            check=fgetc(fp);
            if(check==EOF){break;}
              else{//printf("\n%d  ",l_num);

            printf("\n%d  %c",l_num,check);}   
      }
    }
   }
   fclose(fp);
  }
  printf("\n");
}
void command_grep(char* com1,char *com2,int cn)             //Fucntion to implement "grep" and "grep -n" command. List the lines of a file if particular string is matched
{
  FILE* fp;                                                 //File pointer declaration
  int bufferLength=260;
  int l_num=1;
  char *read_l;
  fp=fopen(com2,"r");
  if(fp==NULL){                                            //checking if the file exist! if not print the error message.
    printf("File doesn't exist!\n");
  }
  else{
  while(fgets(read_l, bufferLength, fp)) {                 //Reading the file contents "line by line"
     if(strstr(read_l,com1)!=NULL){                        //checking if the string contains the particular substring to be matched 
      if(!cn)                                              //checking if the command is "grep -n"
      {
        printf("%d:%s",l_num,read_l);                      //Print the matched lines along with the line numbers
      }
      else                                                 //else print only the matched lines
        printf("%s",read_l);
      }
      l_num++;
      } 
       fclose(fp);  
   }
}
void command_mkdir1(char* com,char *com2)                 //function to implement "mkdir -m" command
{
  int confirm;
  int permission=atoi(com2);                              //converting the string permission into integer
  //printf("%d\n",permission);
  confirm=mkdir(com,0777);                                //making directory with particular permission
}
void command_mkdir2(char* com)                            //function to implement "mkdir" command
{
  int confirm;
  confirm=mkdir(com,0777);                                //making directory with particular permission
  if(confirm)                                             //checking if the directory is created successfully
  {
   printf("Directory not created!\n");
  }
}
void command_sort(char * com)                            //Function to implement "sort" command
{
  FILE *fp;                                              //declaring file ponter
  char myline[365];                                      //character array to store file data line by line
  char **data=NULL;                                      //declaring a charcter pointer to pointer to store the data of the file
  int i,j;
  int l_num=0;
  fp=fopen(com,"r");
  if(fp==NULL)                                           //checking if the file exists, if not display an error message
  {
    printf("No such file exists!\n");
  }
  else
  {
   while(fgets(myline,365,fp)!=NULL)                     //loop to copy the data of file into array
   {
    if(strchr(myline,'\n'))                           
      myline[strlen(myline)-1]='\0';
    data=(char**)realloc(data,sizeof(char**)*(l_num+1));  //dynamically allocating the memory
    data[l_num]=(char*)calloc(365,sizeof(char));
    strcpy(data[l_num],myline);
    l_num++;
   }
   for(i=0;i<(l_num-1);i++)                             //Sorting the contents of the file
   {
    for(j=0;j<(l_num-i-1);j++)
    {
      if(strcmp(data[j],data[j+1])>0)
      {
        strcpy(myline,data[j]);
        strcpy(data[j],data[j+1]);
        strcpy(data[j+1],myline);
      }
    }
   }
   fclose(fp);                                        //closing the file!
   fp=fopen(com,"w");                                 //Reopening the file in write mode
   for(i=0;i<l_num;i++)                               //loop to write the data of the array into file
   {
    fprintf(fp,"%s\n",data[i]);
    printf("%s\n",data[i]);                           //print the content on terminal
   }
   for(i=0;i<l_num;i++)
   {
    free(data[i]);
   }
   free(data);                                      //deallocating the memory
   fclose(fp);                                      //closing the file pointer
  }
}
 void command_sort_r(char* com)                    //function to implement "sort -r" command to sort the file contents in reverse order
 {
  FILE *fp1;                                       //declaring file pointer
  char myline[365];                                //character array to read file content line by line
  char **data=NULL;                                //declaring a charcter pointer to pointer to store the data of the file i.e. in array
  int i,j;
  int l_num=0;
  fp1=fopen(com,"r");                              //opening the file in read modified
  if(fp1==NULL)
  {                                                //checking if file exist if not then print error message
    printf("No such file exist!\n");
  }
  else
  {
   while(fgets(myline,365,fp1)!=NULL)             //loop to store the file content by reading file content line by line into an array
   {
    if(strchr(myline,'\n'))
      myline[strlen(myline)-1]='\0';
    data=(char**)realloc(data,sizeof(char**)*(l_num+1));
    data[l_num]=(char*)calloc(365,sizeof(char));
    strcpy(data[l_num],myline);
    l_num++;
   }
   for(i=0;i<(l_num-1);i++)                       //sorting the file content in reverse order
   {
    for(j=0;j<(l_num-i-1);j++)
    {
      if(strcmp(data[j],data[j+1])<0)
      {
        strcpy(myline,data[j]);
        strcpy(data[j],data[j+1]);
        strcpy(data[j+1],myline);
      }
    }
   }
   fclose(fp1);                                //closing the file
   fp1=fopen(com,"w");                         //reopening the file in write mode
   for(i=0;i<l_num;i++)                        //loop to print the array content back into the file
   {
    fprintf(fp1,"%s\n",data[i]);
    printf("%s\n",data[i]);                    //printing the conetnt on terminal
   }
   for(i=0;i<l_num;i++)
   {
    free(data[i]);                            //dellocating the memory
   }
   free(data);
   fclose(fp1);                               //closing the file pointer
  }
 }
 void get_directory()                // fucntion for getting the present directory
 {
  char directory[250];
  getcwd(directory,sizeof(directory));   //getting the directory name
  printf(":~%s",directory);  //printing the directory
 }

void find_hostname()                      //function to print the hostname
{
  char hostname[100];
  gethostname(hostname,100);              //getting the hostname
  printf("%s",hostname);
}

int exit_from_terminal(char * read_line)  //program for exiting from the terminal
{
  if(strcmp(read_line,"exit")==0)       //checking if user is exiting from the terminal
  return 0;
}

int main()                               //MAIN FUNCTION
{
char *read_line;
char **command;
char *cmp;
char directory[100];
char *user=getenv("USER");             //getting the user name
system("clear");
do{  
printf("%s",user);
printf("@");
find_hostname();                      //calling the hostname functyion to print the hostname                
get_directory();                     //Calling the function to print the directory         
read_line=readline("$         ");           
int len=sizeof(read_line)/sizeof(read_line[0]);  //calculating the size of readline from terminal
if(len>0)
{
add_history(read_line);           //adding the commands in history so that upper and lower arrow key can store them
}
int count=0;
while(count!=len)
{
 if(read_line[count]=='|')      //checking if pipe is given as command
  exit(1);
count++;
}
int checking=exit_from_terminal(read_line);  //storing the return value from exit_from_terminal
if(checking==0)
  break;                                     //exit from the loop
int length=0;
  while(1)
    {
      command[length]=strsep(&read_line," ");  //using the " " as separator we are storing the commands in command array
      if(!command[length])
      break;
      if(command[length]==" ")
      length--;
      length++;
    }

if(strcmp(command[0],"cd")==0)               //checking if the command is "cd"
{
 command_cd(command[1],length);              //going to the function command_cd()
}

else if(strcmp(command[0],"ls")==0)               //checking if the function is ls
{
  if(length>1){
  if(strstr(command[1],"-")!=NULL)           //checking if it contains an extra sub-command
  {
    if(strcmp(command[1],"-a")==0)
    {
      command_ls();
    }
    else
    {
      printf("Arguement cannot be handled!\n");  //if it contains a command other than "-a" than display an error message
    }
  }
}
  else
  {
     command_ls1();                             //calling "ls" command function
  }
}

else if(strcmp(command[0],"cp")==0)                 //checking if the command is "cp"
{
  if(strstr(command[1],"-")!=NULL){            //checking the sub-command
    if(length>4)
    {
      printf("too many arguements!\n");
    }
    else if(length<4)
    {
      printf("Misiing operand!\n");
    }
    else
    {
    if(strcmp(command[1],"-u")==0)
    {
     command_cp(command[2],command[3],0);      //calling command_cp function
    }
    else{
      printf("Arguement not handled!\n");
    }
  }
  }
  else
  {
    if(length>3)
    {
      printf("too many arguements!\n");
    }
    else if(length<3)
    {
      printf("Missing destination file operand!\n");
    }
    else{
      command_cp(command[1],command[2],1);     //calling command_cp() function but with different arguements
    }
  }
}
else if(strcmp(command[0],"cat")==0)                //checking if the command is "cat"
{
  if(strstr(command[1],"-")!=NULL)             //checking the sub-command
  {
    if(length>3)                               //if the entered command length is greater than 3 then show an error message
    {
      printf("too many arguements!\n");
    }
    else
    {
      if(strcmp(command[1],"-n")==0){         //calling the command_cat function()
        command_cat(command[2],0);
      }
      else{
        printf("Arguements can't be handled!\n");
      }
    }
  }
  else
  {
    if(length>2){
      printf("too many arguements!\n");       //display an error message if it contains more than 2 arguements without sub command
    }
    else
    {
        command_cat(command[1],1);
    }
  }
  }
 else if(strcmp(command[0],"grep")==0)          //checking if the command is "grep"
  {
    if(strstr(command[1],"-")!=NULL){        //checking if there is any sub command
      if(length>4)
      {
        printf("too many arguements!\n");
      }
      else if(length<4)
      {
        printf("Missing operand!\n");
      }
      else{
        if(strcmp(command[1],"-n")==0)
        {
        command_grep(command[2],command[3],0);     //calling command_grep() function
        }
        else
        {
          printf("Arguement not handled!Invalid option!\n");  //printing an error message if there is any other sub command/option
        }
      }
    }
    else
    {
      if(length>3)
      {
        printf("too many arguements!\n");
      }
      else if(length<3)
      {
        printf("Operand missing!\n");
      }
      else
      {
        command_grep(command[1],command[2],1);    //calling grep_command() function but with different arguements
      }
    }
  }
 else if(strcmp(command[0],"mkdir")==0)               //checking if the command is "mkdir"
  {
    for(int i=0;i<10;i++){}
    if(strstr(command[1],"-")!=NULL)             //checking if the command contains any option
    {
      if(length>4)
      {
        printf("too many arguements!\n");        //checking if there are more arguements than required
      }
      else if(length<4)
      {
        printf("Missing Operand!\n");
      }
      else
      {
        if(strcmp(command[1],"-m")==0)
        {
          command_mkdir1(command[3],command[2]);   //calling command_mkdir1() function
        }
        else
        {
          printf("Arguement cannot be handled!Invalid option!\n");
        }
      }
    }
    else
    {
     
      if(length<2)
      {
        printf("Missing Operand!\n");
      }
      else
      {
        for(int i=1;i<length;i++)
        {
        command_mkdir2(command[i]);   }   //calling command_mkdir2() function
      }
    }
  } 
 else if(strcmp(command[0],"sort")==0)         //checking if the command is "sort"
  {
   if(strstr(command[1],"-")!=NULL)        //checking if there is any option
   {
    if(strcmp(command[1],"-r")==0)          //checking if the option is "-r"
    {
    if(length>3)
    {
      printf("too many arguements!\n");
    }
    else if(length<3)
    {
      printf("Missing Operand!\n");
    }
    else
    {
      command_sort_r(command[2]);
    }
  }
  else
    {printf("Arguement can not be handled! Invalid option!\n");}   //if its not "-r" print an error message
   }
   else
   {
    if(length<2)
    {
      printf("Missing operand!\n");
    }
    else
    command_sort(command[1]);
    }                 //calling sort command function without any error
  }
  else
  {
    printf("Command not found!\n");
  }
}while(1);                                    //ending loop
return 0;
}
