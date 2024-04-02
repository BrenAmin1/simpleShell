#include <sys/types.h>
#include<sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 128
#define delimiter " \t\r\n\a"
void strcat_c (char *str, char c) {
	for (;*str;str++); 
	*str++ = c; 
	*str++ = 0;
}
void setup(char inputBuffer[], char *args[], int* background) {
	/*char line[MAX_LINE];
	scanf("%128[^\n]", line);
	inputBuffer=line;
	*/
	char *command = NULL;
	ssize_t commandBufferSize = 0;
	int tokenBufferSize = 64, curPos = 0;
	char **tokens = malloc(tokenBufferSize * sizeof(char*));
	char *token;
	pid_t pid;
	if(getline(&command, &commandBufferSize, stdin) == -1) {
		if(feof(stdin)) {
			exit(EXIT_SUCCESS);
		}
	}
	
	token = strtok(command, delimiter);
	while(token != NULL) {
		tokens[curPos] = token;
		curPos++;
		
		if(curPos >= tokenBufferSize) {
			tokenBufferSize += 64;
			tokens = realloc(tokens, tokenBufferSize * sizeof(char*));
		}
		token = strtok(NULL, delimiter);
	}
	tokens[curPos] = NULL;
	if(tokens[0] == NULL) {
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if(pid == 0) { 
		if(execvp(tokens[0],tokens) == -1) {
			perror("error");
		}
		exit(EXIT_FAILURE);
	}
	else {
		if(tokens[sizeof(tokens)-1] == "&") {
			wait(0);
		}
	}
}

int main(void) {
	char inputBuffer[MAX_LINE];
	int background;
	char *args[MAX_LINE/2+1];
	while(1) {
		background = 0;
		printf("COMMAND->");
		fflush(stdout);
		
		setup(inputBuffer, args, &background);
	}
}


/*
void commandLine(char inputBuffer[]) {
	printf("value of inputBuffer: %s", inputBuffer);

	int i,count;
	char* token;
	char delimiter[] = " ";
	scanf("%256[^\n]", inputBuffer);
	printf("%s", inputBuffer);
	
	token = strtok(inputBuffer,delimiter);
	while(token != NULL) {
		token = strtok(NULL,delimiter);
		args[count] = token;
		count++;
	}
	//printf("%s", args[0]);
	
	for(i = 0; i<sizeof(inputBuffer); i++) {
		if(inputBuffer[i] == ' ') {
			temp2[0] = (char*)temp;
			args[count] = temp2[0];
			count++;
			temp[0] = '\0';
		}
		else if(i == sizeof(inputBuffer)-1) {
			strcat_c(temp,inputBuffer[i]);
			temp2[0] = (char*) temp;
			args[count] = temp2[0];
		}
		else {
			strcat_c(temp,inputBuffer[i]);
		}
	}
	printf("%s\n", args[0]);
}

int main(void) {
	char inputBuffer[MAX_LINE];
	fgets(inputBuffer, sizeof(inputBuffer), stdin);
	commandLine(inputBuffer);
	return 0;
}*/
