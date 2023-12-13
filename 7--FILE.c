#include "shell.h"

/**
 *_process - _processes the command for forking and execution
 *@shell: the main structure
 *Return: nothing
 */

int _process(SHELL *shell)
{
	int Exec, i, numbuilts;
	char *Dir,  *bul = shell->toks[0], *builtin[] = {"cd", "exit"};

	Exec = _executables(shell->toks[0]);

	numbuilts = sizeof(builtin) / sizeof(builtin[0]);
	for (i = 0; i < numbuilts; i++)
	{
		if (_strcmp(bul, builtin[i]) == 0)
			return (0);
	}
	if (Exec == -1)
	{
		command_unfound(shell);
		shell->status = 127;
		return (1);
	}
	else if (Exec == 0)
	{
		Dir = which(shell->toks[0]);
	}
	else
	{
		Dir = shell->toks[0] + Exec;
	}
	if (_access(Dir, shell) == -1)
	{
		shell->status = 126, free(Dir);
		return (1);
	}
	fork_(Dir, shell);
	if (Exec == 0)
	{
		free(Dir);
	}
	return (0);
}

/**
 *fork_ - perform the execution of the commands
 *@input: path of the executable
 *@shell: the main structure
 */

void fork_(char *input, SHELL *shell)
{
	pid_t Pid;
	int state;
	char **tok;


	Pid = fork();
	tok = shell->toks;

	if (Pid == 0)
	{
		execve(input, tok, environ);
	}

	else if (Pid < 0)
	{
		perror(tok[0]);
		return;
	}

	else
	{
		do {
			waitpid(Pid, &state, WUNTRACED);
		} while (!WIFEXITED(state) && !WIFSIGNALED(state));
	}

	shell->status = state / 256;
}
/**
 *file_as_arg - this handles commands in a file
 *@shell: main structure
 *Return: nothing
 */

void file_as_arg(SHELL *shell)
{
	FILE *reads;
	size_t len;
	char *line = NULL;


	reads = fopen(shell->av[1], "r");

	if (reads == NULL)
	{
		exit(EXIT_FAILURE);
	}

	while ((getline(&line, &len, reads)) != -1)
	{
		shell->loop_count++;
		_source(shell, line);
	}
	if (line)
	{
		free(line);
	}

	fclose(reads);
	exit(0);
}
/**
 *frees - it frees the main structure
 *@eshell: the main structure
 *Return: nothig
 */

void frees(SHELL *eshell)
{
	int j;

	if (eshell->toks)
	{
		for (j = 0; eshell->toks[j]; j++)
		{
			free(eshell->toks[j]);
		}

		free(eshell->toks);
	}

	for (j = 0; eshell->_environ[j]; j++)
	{
		free(eshell->_environ[j]);
	}

	free(eshell->_environ);

	eshell->toks = NULL;
}

/**
 *free_log - this frees the logical operators linked lists
 *@head: head of the liked lists
 *Return: nothing
 */

void free_log(log *head)
{
	log *current, *tmp;

	if (head == NULL)
	{
		return;
	}

	current = head;

	while (current)
	{
		free(current->com);
		tmp = current->next;
		free(current);
		current = tmp;
	}
}
