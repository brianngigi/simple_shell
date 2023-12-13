#include "shell.h"

#define ARRAY_SIZE 128

/**
 * Tokenize - Splits a line into token based on the specified mode.
 * @line: a line to be Tokenized.
 * @mode: mode of tokenization ie 1 for normal, 2 for special
 * characters.
 * Return: dynamically allocate array of strings containing the tokens,
 * or NULL if an error occured.
 */

char **Tokenize(char *line, int mode)
{
	char **word_t, *word;
	int size = ARRAY_SIZE, i = 0;
	char *Del;

	if (line == NULL)
	{
		perror("hsh");
		return (NULL);
	}
	if (mode == 1)
	{
		Del = " \n";
	}
	else
	{
		Del = "|&;";
	}
	word_t = malloc(size * sizeof(char *));
	if (word_t == NULL)
	{
		free(word_t);
		return (NULL);
	}
	word = strtok(line, Del);
	while (word != NULL)
	{
		if (i == size - 1)
		{
			word_t = _realloc(word_t, size * sizeof(char *), size * 2 * sizeof(char *));
			if (!word_t)
			{
				write(STDERR_FILENO, "MEMORY ALLOCATION ERROR\n", 24);
				exit(EXIT_FAILURE);
			}
			size = size * 2;
		}
		word_t[i] = strdup(word), word = strtok(NULL, Del), i++;
	}
	word_t[i] = NULL;
	return (word_t);
}

/**
 * syntax_error - Checks for syntax error in the command.
 * @input: command to check for syntax errors.
 * @shell: Pointer to the shell data structure.
 * Return: 0 upon success, -1 if a syntax error present.
 */

int syntax_error(char *input, SHELL *shell)
{
	int i;
	char *error_txt;

	error_txt = NULL;
	i = 1;

	for (; input[i]; i++)
	{
		if (input[i] == ';')
		{
			if (input[i + 1] == ';')
			{
				error_txt = ";;";
				break;
			}

			else if (input[i + 1] == '|')
			{
				error_txt = "|";
				break;
			}

			else if (input[i + 1] == '&')
			{
				error_txt = "&";
				break;
			}
		}
	}
	if (error_txt)
	{
		flush_error(error_txt, shell);
		return (-1);
	}
	else if (_pipe(input, shell) == -1 || handle_amp(input, shell) == -1)
	{
		return (-1);
	}

	return (0);
}

/**
 *_pipe - Checks for syntax error related to _pipes in command.
 * @input: command to check for _pipe-related syntax error.
 * @shell: Pointer to shell data structure.
 * Return: 0 upon success, -1 if syntax error is encountered.
 */

int _pipe(char *input, SHELL *shell)
{
	int i;
	char *error_txt = NULL;

	for (i = 1; input[i]; i++)
	{
		if (input[i] == '|' && input[i + 1] == '|')
		{
			i += 2;
		}

		if (input[i] == '|')
		{
			if (input[i + 1] == ';')
			{
				error_txt = (input[i + 2] == ';' ? ";;" : ";");
				break;
			}

			else if (input[i + 1] == '&')
			{
				error_txt = (input[i + 2] == '&' ? "&&" : "&");
				break;
			}

			else if (input[i] == '|')
			{
				i++;
				error_txt = (input[i + 1] == '|' ? "||" : "|");
				break;
			}
		}
	}

	if (error_txt)
	{
		flush_error(error_txt, shell);
		return (-1);
	}

	return (0);
}

/**
 * handle_amp - Checks for syntax error related to handle_amp in the command.
 * @input: command checks for handle_amp-related syntax error.
 * @shell: Pointer to shell data structure.
 * Return: 0 upon success, -1 if syntax error is encountered.
 */

int handle_amp(char *input, SHELL *shell)
{
		int i;
	char *error_txt = NULL;

	for (i = 1; input[i]; i++)
	{
		if (input[i] == '&' && input[i + 1] == '&')
		{
			i += 2;
		}

		if (input[i] == '&')
		{
			if (input[i + 1] == ';')
			{
				error_txt = (input[i + 2] == ';' ? ";;" : ";");
				break;
			}

			else if (input[i + 1] == '|')
			{
				error_txt = (input[i + 2] == '|' ? "||" : "|");
				break;
			}

			else if (input[i] == '&')
			{
				i++;
				error_txt = (input[i + 1] == '&' ? "&&" : "&");
				break;
			}
		}
	}

	if (error_txt)
	{
		flush_error(error_txt, shell);
		return (-1);
	}

	return (0);
}
/**
 * exit_ - this exits shell
 * @shell: the main structure
 * Return: nothing
 */


void exit_(SHELL *shell)
{
	int exit_status;

	if (shell->toks[1] != NULL)
	{
		exit_status = _atoi(shell->toks[1]);
		shell->status = exit_status;
		exit(shell->status);
	}
	else
	{
		shell->status = 0;
		exit(shell->status);
	}
}
