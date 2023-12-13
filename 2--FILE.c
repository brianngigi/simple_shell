#include "shell.h"

/**
 * _executables - a function that checks for _executables.
 * @input: a Pointer to the string to which the executable is checked.
 *
 * Return: -1 on failure.
 */

int _executables(char *input)
{
	char *check = input;
	int i;
	struct stat Stat;

	for (i = 0; check[i]; i++)
	{
		if (check[i] == '.')
		{
			if (check[i + 1] == '.')
				return (0);
			else if (check[i + 1] == '/')
				continue;
		}
		else if (check[i] == '/' && i != 0)
		{
			if (check[i + 1] == '.')
				continue;

			i++;
			break;
		}
		else
			break;
	}

	if (i == 0)
		return (0);

	if (stat(input + i, &Stat) == 0)
		return (i);

	return (-1);
}

/**
 * which - Search for a command in the directory listed in the Path
 * environment variable.
 * @input: a command to search for.
 *
 * Return: the pointer to the full path of the command if found, else Null
 */

char *which(char *input)
{
	char *path, *copy, *tok;
	char *dir;
	int string_len;
	struct stat Stat;

	path = _strdup(_getenv("PATH"));
	copy = path;
	tok = _Strtok(copy, ":");

	while (tok)
	{
		string_len = _strlen(input) + _strlen(tok) + 2;
		dir = malloc(string_len * sizeof(char));
		if (!dir)
		{
			free(dir);
			return (NULL);
		}
		dir = _strcpy(dir, tok);
		dir = _strcat(dir, "/");
		dir = _strcat(dir, input);
		dir = _strcat(dir, "\0");
		if (stat(dir, &Stat) == 0)
		{
			free(path);
			return (dir);
		}
		else
		{
			free(dir);
			tok = _Strtok(NULL, ":");
		}
	}
	if (stat(input, &Stat) == 0)
	{
		free(path);
		return (_strdup(input));
	}
	free(path);
	return (NULL);
}


/**
 * _access - Checks if a command is accessible or executable.
 * @input: a command to check.
 * @shell: Pointer to shell data structure.
 * Return: ) upon success, -1 if the command is not accessible.
 */

int _access(char *input, SHELL *shell)
{
	if (input == NULL)
	{
		/*print error to the console*/
		command_unfound(shell);
		shell->status = 127;
		return (-1);
	}

	else if (access(input, X_OK) == -1)
	{
		/*print error to the console*/
		path_errors(shell);
		return (-1);
	}

	return (0);
}
/**
 * syntax - checks the syntax of a command for correctnes.
 * @input: command to check for syntax.
 * @shell: Pointer to shell data structure.
 *
 * Return: 0 upon success, -1 if there is a syntax error.
 */

int syntax(char *input, SHELL *shell)
{
	if (_first(input, shell) == -1 || syntax_error(input, shell) == -1)
	{
		return (-1);
	}

	return (0);

}

/**
 * _first - Checks the syntax of first token in the command.
 * @input: command to check the first token.
 * @shell: Pointer to shell data structure.
 * Return: 1 up on success, -1 if there is a syntax error.
 */

int _first(char *input, SHELL *shell)
{
	int i;
	char *Error_txt = NULL;

	for (i = 0; input[i]; i++)
	{
		while (input[i] == ' ' || input[i] == '\t')
		{
			i++;
		}

		if (input[i] == ';')
		{
			Error_txt = (input[i + 1] == ';' ? ";;" : ";");
			break;
		}

		else if (input[i] == '|')
		{
			Error_txt = (input[i + 1] == '|' ? "||" :  "|");
			break;
		}

		else if (input[i] == '&')
		{
			Error_txt = (input[i + 1] == '&' ? "&&" : "&");
			break;
		}

		else
		{
			break;
		}
	}
	if (Error_txt)
	{
		flush_error(Error_txt, shell);
		return (-1);
	}
	return (1);
}
