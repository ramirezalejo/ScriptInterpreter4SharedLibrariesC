#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <stdbool.h>
#define BUFFER_MAX_LENGTH 1024


char* subString (const char* input, int offset, int len, char* dest)
{
  int input_len = strlen (input);

  if (offset + len > input_len)
  {
     return NULL;
  }

  strncpy (dest, input + offset, len);
  return dest;
}

void processLine(char line[BUFFER_MAX_LENGTH])
{

    if (strlen(line) < 2)
        return;

    void *handle;
    char library[100] = "";
    char command[300] = "";
    int currentHandlerId = 0;
    char libraryPath[200] = "";

    double (*fn)(int *);
    int x; char *error;

    if (strncmp(line, "use", 3) == 0)
    {
        if (subString(line, 4, strlen(line) - 4 , library))
        {

            strcpy(libraryPath, "./");
            strcat(libraryPath,library);
            fprintf(stdout, "%s%s\n", "Loading library ", library);
            handle = dlopen(libraryPath, RTLD_LAZY);
            if (!handle) {
                fprintf(stderr, "%s\n", dlerror());
            }
            else
            {
                fprintf(stdout, "%s%s\n", "Loaded library ", library);
            }
        }

    }
    else if (strncmp(line, "call", 4) == 0)
    {
        if (subString(line, 5, strlen(line) - 5 , command))
        {
            fprintf(stdout, "%s%s\n", "calling ", command);
            if (!handle)
            {
                fprintf(stdout, "%s\n", "Library not Loaded");
            }
            else
            {
                fn = dlsym(handle,command);
                if ((error = dlerror()) != NULL)
                {
                    fprintf(stderr, "%s\n", error); //exit(1);
                    fprintf(stdout, "%s\n", "function not found");
                }
                else
                {
                            (*fn)(&x);

                            //printf("Valx=%d\n",x);
                    fprintf(stdout, "\n%s%s\n", "called ", command );
                }
            }
        }
    }
    else if (line)
    {
        fprintf(stdout, "%s%s\n", "Sintax error or Command not supported : " , line);
    }

}

int main(int argc, char* argv[])
{
    FILE *file = NULL;
    int tempChar;
    unsigned int tempCharIdx = 0U;
    char line[BUFFER_MAX_LENGTH];

    if (argc == 2)
         file = fopen(argv[1], "r");
    else {
         fprintf(stderr, "error: wrong number of arguments\n"
                         "usage: %s textfile\n", argv[0]);
         return -1;
    }

    if (!file) {
         fprintf(stderr, "error: could not open textfile: %s\n", argv[1]);
         return -1;
    }

    /* get a character from the file pointer */
    while(tempChar = fgetc(file))
    {
        /* avoid buffer overflow error */
        if (tempCharIdx == BUFFER_MAX_LENGTH) {
            fprintf(stderr, "error: line is too long. increase BUFFER_MAX_LENGTH.\n");
            return -1;
        }

        /* test character value */
        if (tempChar == EOF) {
            line[tempCharIdx] = '\0';
            //fprintf(stdout, "%s\n", line);
            processLine(line);

            break;
        }
        else if (tempChar == '\n') {
            line[tempCharIdx] = '\0';
            tempCharIdx = 0U;

            //fprintf(stdout, "%s\n", line);
            processLine(line);

            continue;
        }
        else
            line[tempCharIdx++] = (char)tempChar;
    }

    char s[BUFFER_MAX_LENGTH];
    size_t n = 0;
    fgets(s, sizeof s, stdin);
    while(1)
    {
        fprintf(stdout, "%s\n", "Please enter the next command (Type exit to exit)");
	fflush(stdout);
        fgets(s, sizeof s, stdin);
	fflush(stdout);
        n = strlen(s) - 1;
        s[n] = '\0';
        if (strncmp(s, "exit", 4) == 0)
            return 0;
        processLine(s);
    }

    return 0;
}




