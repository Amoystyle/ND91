#include "shell.h"

static pid_t    *childpid = NULL;  
static int      maxfd;  

int ExecuteShellCallSystem(char* cmd)
{
	//DebugNdsh("ExecuteShellBySystem(\"%s\")\n", cmd);

	int status = system(cmd);
    
	if (-1 == status)
	{
		ErrorNdsh("ExecuteShellCallSystem fail ! errno: %d\n", errno);
		return -1;
	}
	else
	{
		if (WIFEXITED(status))
		{
			if (0 == WEXITSTATUS(status))
			{
				//DebugNdsh("WEXITSTATUS(%d) true\n", status);

				return 1;
			}
			else
			{
				ErrorNdsh("WEXITSTATUS(%d) false !    errno: %d\n", status, errno);

				return -1;
			}
		}
		else
		{
			ErrorNdsh("WIFEXITED(%d) false !    errno: %d\n", status, errno);

			return -1;
		}
	} 
}

struct nd_data ExecuteShellCallPopen(char* cmd)
{
	//DebugNdsh("ExecuteWithResult(char* cmd)\ncmd: %s\n", cmd);

	struct nd_data data_result = {0, 0, NULL};
	if (cmd == NULL)
	{
		const char* msg = "cmd is null\n";
		ErrorNdsh(msg);
		WriteIntToBuffer(&data_result, UNKNOW_ERROR);

		return data_result;
	}

	FILE* stream = NULL;
	if ((stream = popen(cmd, "r")) == NULL)
	{
		ErrorNdsh("popen(\"%s\", \"r\") fail !    errno: %d\n", cmd, errno);
		WriteIntToBuffer(&data_result, UNKNOW_ERROR);

		return data_result;
	}

	char* content;
	int count = ReadFromStreamEnd(stream, &content, 3 * 1000 * 1000);
	WriteIntToBuffer(&data_result, SUCCESS);
	WriteStringToBuffer(&data_result, content, count);
	pclose(stream);
	return data_result;
}

int ExecuteShellCallPeopnForStr(char* cmd, char** result, int timeout)
{
	//DebugNdsh("ExecuteShellCallPeopnForStr(char* cmd)\ncmd: %s\n", cmd);

	if (cmd == NULL)
	{
		ErrorNdsh("cmd if null\n");

		return -1;
	}

	FILE* stream = NULL;
	if ((stream = popen(cmd, "r")) == NULL)
	{
		ErrorNdsh("popen(\"%s\", \"r\") fail !    errno: %d\n", cmd, errno);

		return -1;
	}

	int readLength = ReadFromStreamEnd(stream, result, timeout);
	int status = pclose(stream);

	return readLength;
}
struct nd_data ExecuteShellForStr(struct nd_data* data_receive)
{
    char *cmd=ReadStringFromBuffer(data_receive);
	struct nd_data data_result = {0, 0, NULL};
	if (cmd == NULL)
	{
		ErrorNdsh("cmd is null\n");
		WriteIntToBuffer(&data_result, UNKNOW_ERROR);

		return data_result;
	}

	FILE* stream = NULL;
	if ((stream = c_popen(cmd, "r")) == NULL)
	{
		ErrorNdsh("popen(\"%s\", \"r\") fail !    errno: %d\n", cmd, errno);
		WriteIntToBuffer(&data_result, UNKNOW_ERROR);

		return data_result;
	}

	char* content;
	int count = ReadFromStreamEnd(stream, &content, 3 * 1000 * 1000);
	WriteIntToBuffer(&data_result, SUCCESS);
	WriteStringToBuffer(&data_result, content, count);
	c_pclose(stream);
	return data_result;
}

struct nd_data ExecuteShell(struct nd_data* data_receive)
{
    char *cmd=ReadStringFromBuffer(data_receive);
	struct nd_data data_result = {0, 0, NULL};
	if (cmd == NULL)
	{
		ErrorNdsh("cmd is null\n");
		WriteIntToBuffer(&data_result, UNKNOW_ERROR);

		return data_result;
	}
	execcmd(cmd); 
	WriteIntToBuffer(&data_result, SUCCESS);
	return data_result;
}

int execcmd(const char *cmd)
{
    pid_t   pid; 
    if ( (pid = fork()) < 0)  
        return(-1);  
    else if (pid == 0) {  
	    const char *args[4] = {"sh","-c",cmd, NULL};
		execvp("sh", (char**)args);  
        _exit(127);  
    }  
	return 0;
}


FILE *c_popen(const char *cmdstring, const char *type)  
{  
    int     i, pfd[2];  
    pid_t   pid;  
    FILE    *fp;  
    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {  
        errno = EINVAL;    
        return(NULL);  
    }  
    if (childpid == NULL) {   
        maxfd = OPEN_MAX_GUESS;  
        if ( (childpid = (pid_t*)calloc(maxfd, sizeof(pid_t))) == NULL)  
		if ( (childpid = (pid_t*)maxfd) == NULL)  
            return(NULL);  
    }  
    if (pipe(pfd) < 0)  
        return(NULL);   
    if ( (pid = fork()) < 0)  
        return(NULL);  
    else if (pid == 0) {  
        if (*type == 'r') {  
            close(pfd[0]);  
            if (pfd[1] != STDOUT_FILENO) {  
                dup2(pfd[1], STDOUT_FILENO);  
                close(pfd[1]);  
            }  
        } else {  
            close(pfd[1]);  
            if (pfd[0] != STDIN_FILENO) {  
                dup2(pfd[0], STDIN_FILENO);  
                close(pfd[0]);  
            }  
        }  
        for (i = 0; i < maxfd; i++)  
            if (childpid[ i ] > 0)  
                close(i);  
        //execl(SHELL, "sh", "-c", cmdstring, (char *) 0);  
		const char *args[4] = {"sh","-c",cmdstring, NULL};
		execvp("sh", (char**)args); 
        _exit(127);  
    }  
    if (*type == 'r') {  
        close(pfd[1]);  
        if ( (fp = fdopen(pfd[0], type)) == NULL)  
            return(NULL);  
    } else {  
        close(pfd[0]);  
        if ( (fp = fdopen(pfd[1], type)) == NULL)  
            return(NULL);  
    }  
    childpid[fileno(fp)] = pid;
    return(fp);  
}  
  
int  c_pclose(FILE *fp)  
{  
  
    int     fd, stat;  
    pid_t   pid;  
    if (childpid == NULL)  
        return(-1);   
    fd = fileno(fp);  
    if ( (pid = childpid[fd]) == 0)  
        return(-1);    
    childpid[fd] = 0;  
    if (fclose(fp) == EOF)  
        return(-1);  
    while (waitpid(pid, &stat, 0) < 0)  
        if (errno != EINTR)  
            return(-1); 
    return(stat);  
}  