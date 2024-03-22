#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>

//+
// File:	shell.c
//
// Pupose:	This program implements a simple shell program. It does not start
//		processes at this point in time. However, it will change directory
//		and list the contents of the current directory.
//
//		The commands are:
//		   cd name -> change to directory name, print an error if the directory doesn't exist.
//		              If there is no parameter, then change to the home directory.
//		   ls -> list the entries in the current directory.
//			      If no arguments, then ignores entries starting with .
//			      If -a then all entries
//		   pwd -> print the current directory.
//		   exit -> exit the shell (default exit value 0)
//				any argument must be numeric and is the exit value
//
//		if the command is not recognized an error is printed.
//-

#define CMD_BUFFSIZE 1024
#define MAXARGS 10

int splitCommandLine(char * commandBuffer, char* args[], int maxargs);
int doInternalCommand(char * args[], int nargs);
int doProgram(char * args[], int nargs);

//+
// Function:	main
//
// Purpose:	The main function. Contains the read
//		eval print loop for the shell.
//
// Parameters:	(none)
//
// Returns:	integer (exit status of shell)
//-

int main() {

    char commandBuffer[CMD_BUFFSIZE];
    // note the plus one, allows for an extra null
    char *args[MAXARGS+1];

    // print prompt.. fflush is needed because
    // stdout is line buffered, and won't
    // write to terminal until newline
    printf("%%> ");
    fflush(stdout);

    while(fgets(commandBuffer,CMD_BUFFSIZE,stdin) != NULL){
        //printf("%s",commandBuffer);

        // remove newline at end of buffer
        int cmdLen = strlen(commandBuffer);
        if (commandBuffer[cmdLen-1] == '\n'){
            commandBuffer[cmdLen-1] = '\0';
            cmdLen--;
                //printf("<%s>\n",commandBuffer);
        }

        // split command line into words.(Step 2)

        int nargs = splitCommandLine(commandBuffer, args, MAXARGS);
        // add a null to end of array (Step 2)

        // TODO

        // debugging
        // printf("%d\n", nargs);
        // int i;
        // for (i = 0; i < nargs; i++){
        // printf("%d: %s\n",i,args[i]);
        // }
        // // element just past nargs
        // printf("%d: %x\n",i, args[i]);

        // TODO: check if 1 or more args (Step 3)
        if (nargs >= 1) {
            int outputdIC = doInternalCommand(args, nargs);
            if (outputdIC == 0) {
                int outputdP = doProgram(args, nargs);
                if(outputdP == 0) {
                    printf("Error: Command '%s' not found. \n", args[0]);
                }
            }
        }
   
        // TODO: if one or more args, call doInternalCommand  (Step 3)
            
            // TODO: if doInternalCommand returns 0, call doProgram  (Step 4)
            
            // TODO: if doProgram returns 0, print error message (Step 3 & 4)
            // that the command was not found.

        // print prompt
        printf("%%> ");
        fflush(stdout);
    }
    return 0;
}

////////////////////////////// String Handling (Step 1) ///////////////////////////////////

//+
// Function:	skipChar
//
// Purpose:	This function skips over a given char in a string
//		For security, will not skip null chars.
//
// Parameters:
//    charPtr	Pointer to string
//    skip	character to skip
//
// Returns:	Pointer to first character after skipped chars
//		ID function if the string doesn't start with skip,
//		or skip is the null character
//-

char * skipChar(char * charPtr, char skip){
    // TODO: contents of function
    if (skip == '\0') {
        return charPtr;
    }

    while (skip == *charPtr) {
        charPtr++;
        // check for null character
        if (skip == NULL) {
            return charPtr;
        }
    }
    // TODO: replace null with proper value
    return charPtr;
}

//+
// Funtion:	splitCommandLine
//
// Purpose:	TODO: This function separates the command line into 
//              individual words. An array of pointers pointing to
//              the start of each word will be stored. All spaces ' ', 
//              and delimiters '\0', will be ignored. The purpose of this
//              function is to separate the command line into words or commands
//              that will be recognizable (or not) by the shell.

// Parameters:
//	TODO: commandBuffer is the user input that will be modified such that
//      a delimiter '\0' will immediatley follow each word, replacing the space
//      separating this word from the one that follows. 
//      args[] is the array of pointers that will be populated throughout the 
//      execution of the function. The function will store a pointer to the beginning 
//      of each individual word.
//
// Returns:	Number of arguments (< maxargs).
//
//-

int splitCommandLine(char * commandBuffer, char* args[], int maxargs){

    int numargs = 0;
    char * currentposition;
    currentposition = skipChar(commandBuffer, ' ');

    while (*currentposition != '\0') {
        //   currentposition = skipChar(currentposition, ' ');

        if (numargs != maxargs) {
            args[numargs] = currentposition;
            numargs++;

            //Pointer to next space
            // If empty, exit
            currentposition = strchr(currentposition, ' ');
        } else {
            fprintf(stderr, "Error: The Maximum Amount of Argument has been reached");
            return numargs;
        }

        if (currentposition == NULL) {
            return numargs;
        }

        *currentposition = '\0';
        currentposition++;
        currentposition = skipChar(currentposition, ' ');

    }
        
    // TODO: return proper value
    //return 0;
    return numargs;
}

////////////////////////////// External Program  (Note this is step 4, complete doeInternalCommand first!!) ///////////////////////////////////

// list of directorys to check for command.
// terminated by null value
char * path[] = {
    ".",
    "/usr/bin",
    NULL
};

//+
// Funtion:	doProgram
//
// Purpose:	TODO: The purpose of this function is to display the current path is to either display the 
//              expected output in the command line if a command is recognized or display an error
//              if the shell is not able to perform the command
//
// Parameters:
//	TODO: args[] contains the pointer to the beginning of each tokenized word in the command line which is used
//          to determine which set of command instructions to execute or which error to throw.
//          nargs contains the number of arguments in the command line. 
//
// Returns	int
//		1 = found and executed the file
//		0 = could not find and execute the file
//-

int doProgram(char * args[], int nargs){
  // find the executable
  // TODO: add body.

  struct stat my_stats;
  char * cmd_path;
  int i, currentDir = 0;
  while (path[currentDir] != NULL) {
      
    cmd_path = (char *)malloc(strlen(path[currentDir]) + strlen(args[0]) + strlen("/"));
      
    // create command path
    sprintf(cmd_path, path[currentDir]);
    sprintf(cmd_path+strlen(cmd_path), "/");
    sprintf(cmd_path+strlen(cmd_path), args[0]);

    // if file does not exist
    if (stat(cmd_path, &my_stats) != 0) { printf("Error: File does NOT exist\n"); }
    // If regular file
    else if (S_ISREG(my_stats.st_mode)) {
        if (my_stats.st_mode&S_IXUSR) {
            break;
        } else {
            fprintf(stderr, "Error: File found is not executable\n");
            return 0;
        }
    }
    else { fprintf(stderr, "File is Executable\n"); }

    currentDir++;
  }
  
  if (cmd_path == NULL) {
      printf("Exit");
      return 0;
  }

  int processID = fork();

  if (processID == -1) {
      fprintf(stderr, "Child process could not be created");
      free(cmd_path);
      return 0;
  } else if (processID == 0) {
      // Execute command
      execv(cmd_path, args);
  } else {
      wait(NULL);
  }

  free(cmd_path);
  return 1;
}

////////////////////////////// Internal Command Handling (Step 3) ///////////////////////////////////

// define command handling function pointer type
typedef void(*commandFunc)(char * args[], int nargs);

// associate a command name with a command handling function
struct cmdStruct{
   char 	* cmdName;
   commandFunc 	cmdFunc;
};

// prototypes for command handling functions
// TODO: add prototype for each comamand function

void exitFunc(char * args[], int nargs);
void pwdFunc(char * args[], int nargs);
void lsFunc(char * args[], int nargs);
void cdFunc(char * args[], int nargs);

// list commands and functions
// must be terminated by {NULL, NULL} 
// in a real shell, this would be a hashtable.
struct cmdStruct commands[] = {
    // // TODO: add entry for each command
    {"exit", exitFunc}, 
    {"pwd", pwdFunc}, 
    {"ls", lsFunc}, 
    {"cd", cdFunc},
    { NULL, NULL}		// terminator
};

//+
// Function:	doInternalCommand
//
// Purpose:	TODO: The purpose of this function is to recognize commands that have been tokenized 
//              from the command line by the function split command line. If a command is recognized, 
//              the corresponding instructions will execute. If a command is not recognized the shell will
//              return an error. If the command is recognized but is accompanied by another word that
//              renders the command unexecutable another error will be returned. 
//
// Parameters:
//	TODO: args[] holds the separated words from the command line and allows the function to determine which
//          command to execute through recognizing a defined command and then performing its correpsonding instructions.
//          nargs inidcated the number of arguments that have been read from the command line. If the number of 
//          arguments is 0 then doProgram will be called.
//
// Returns	int
//		1 = args[0] is an internal command
//		0 = args[0] is not an internal command
//-

int doInternalCommand(char * args[], int nargs){

    // TODO: function contents (step 3)
    for (int i = 0; commands[i].cmdName != NULL; i++) {
        if (strcmp(commands[i].cmdName, args[0]) == 0) {
            commands[i].cmdFunc(args, nargs);
            return 1;
        }
    }
    return 0;
}

// comand Handling Functions //

// TODO: a function for each command handling function
// goes here. Also make sure a comment block prefaces
// each of the command handling functions.


/*  Exit function: this is used to exit the shell and with status 0
*/
void exitFunc(char * args[], int nargs) {
    exit(0);
}
/* Pwd function: this is used to print the full path name of the current working directory 
*/
void pwdFunc(char * args[], int nargs) {
    char * cwd = getcwd(NULL,0);
    if (cwd == NULL) {
        printf("Error in cwd");
        return;
    }
    printf("%s\n", cwd);
    free(cwd);
}
/* Cd function: move to the directory specified after the cd command
*/
void cdFunc(char * args[], int nargs) {
    char* new_directory;
    if (nargs == 1) {
        struct passwd *pw = getpwuid(getuid());
        if (pw == NULL) {
            printf("Error: could not get the home directory");
            return;
        }
        new_directory = pw->pw_dir;
    } else {
        // nargs >= 1 is accounted for in main
        new_directory = args[1];
    }
    //chdir(new_directory) assign to variable to check and access
    int outputChdir = chdir(new_directory);
    if (outputChdir != 0) {
        printf("Error: could not change to target directory");
    }
}

int checkDot(const struct dirent * d) {
    if (d->d_name[0] == '.') {
        return 0;
    }
    return 1;
}

/* Ls function: list all files in the current directory 
*/
void lsFunc(char * args[], int nargs) {
    if (nargs == 2 && strcmp(args[1], "-a") == 0) {
        struct dirent ** namelist;
        int numEnts = scandir(".",&namelist,NULL,NULL);
        for (int i = 0; i < numEnts; i++) {
            char* name = namelist[i]->d_name;
            printf("%s\n", name);
        }
    } else { // ls
        struct dirent ** namelist;
        int numEnts = scandir(".",&namelist,checkDot,NULL);
        for (int i = 0; i < numEnts; i++) {
            char* name = namelist[i]->d_name;
            printf("%s\n", name);
        }
    }
    
}

