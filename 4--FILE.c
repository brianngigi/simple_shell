#include "shell.h"

/**
 *_chdir - this changes directories
 *@shell: the main structure
 *Return: nothing
 */
void _chdir(SHELL *shell)
{
	int result;
	char cwd[1024];
	const char *oldpwd;

	if (!shell->toks[1] || _strcmp(shell->toks[1], "~") == 0)
	{
		result = chdir(_getenv("HOME"));
	}
	else if (_strcmp(shell->toks[1], "-") == 0)
	{
		oldpwd = _getenv("OLDPWD");
		if (oldpwd[0] == '\0')
		{
			perror("hsh");
			return;
		}
		result = chdir(oldpwd);
	}
	else
	{
		result = chdir(shell->toks[1]);
	}

	if (result == -1)
	{
		perror(shell->av[0]);
		return;
	}
	else if (result != -1)
	{
		getcwd(cwd, sizeof(cwd));
		setenv("OLDPWD", _getenv("PWD"), 1);
		setenv("PWD", cwd, 1);
	}
}
/**
 *get_env - this extracts the enviroment varaibles
 *@shell: the main structure
 *Return: void
 */

void get_env(SHELL *shell)
{
	int env_count = 0, i, j;

	while (environ[env_count] != NULL)
		env_count++;
	shell->_environ = malloc(sizeof(char *) * (env_count + 1));
	if (!shell->_environ)
		return;
	for (i = 0; i < env_count; i++)
	{
		shell->_environ[i] = _strdup(environ[i]);
		if (!shell->_environ[i])
		{
			for (j = 0; j < i; j++)
				free(shell->_environ[j]);
			free(shell->_environ);
			return;
		}
	}
	shell->_environ[env_count] = NULL;
}
/**
 *command_unfound - this prints the command unfound error
 *@shell: the main structure
 *Return: nothing
 */

void command_unfound(SHELL *shell)
{
	char *loop_count;

	loop_count = _itoa(shell->loop_count);
	write_i(shell->av[0]);
	write_i(": ");
	write_i(loop_count);
	write_i(": ");
	write_i(shell->toks[0]);
	write_i(": not found\n");

	free(loop_count);
}

/**
 *path_errors - this prints the path error
 *@shell: the main structure
 *Return: nothing
 */

void path_errors(SHELL *shell)
{
	char *loop_count;


	loop_count = _itoa(shell->loop_count);

	write_i(shell->av[0]);
	write_i(": ");
	write_i(loop_count);
	write_i(": ");
	write_i(shell->toks[0]);
	write_i(": Permission denied\n");

	free(loop_count);
}

/**
 *flush_error - this writes error to the console
 *@error: errror to be printed.
 *@shell: main structure.
 *Return: nothing.
 */

void flush_error(char *error, SHELL *shell)
{
	char *count = _itoa(shell->loop_count);

	write_i(shell->av[0]);
	write_i(": ");
	write_i(count);
	write_i(": ");
	write_i("syntax error");
	write_i(": \"");
	write_i(error);
	write_i("\" unexpected\n");

	free(count);
}
