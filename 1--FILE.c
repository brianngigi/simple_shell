#include "shell.h"
/**
 *_source - controls or parses the input
 *@shell: the main structure
 *@line: the input
 *Return: 0 incase of commad exit else 1
 */

int _source(SHELL *shell, char *line)
{
	log *head, *current;
	char *word, *copy;
	int result;

	head = NULL, copy = _strdup(line), word = strtok(copy, "|&;\n");

	while (word)
	{
		_log(&head, word), word = strtok(NULL, "|&;\n");
	}
	sep(&head, line),	current = head;
	while (current)
	{
		shell->toks = Tokenize(current->com, 1), get_env(shell);
		if (!shell->toks)
		{
			continue;
		}
		result = handle_builtins(shell);
		if (result == 1)
		{
			free(copy), free_log(head), frees(shell);
			return (0);
		}
		_process(shell),	log_current(shell, &current);
		if (current)
		{
			current = current->next;
		}
		frees(shell);
	}
	free(copy), free_log(head);
	return (1);
}

/**
 *line_shuffler - swap one of logcal operators for non printables and back
 *@line: input
 *@mode: 1 for swaping to non printable and 2 for the undo
 *Return: nothing
 */

void line_shuffler(char *line, int mode)
{
	int i;
	char a;

	if (mode == 1)
	{
		for (i = 0; line[i]; i++)
		{
			a = line[i];
			if (a == '|')
			{
				line[i] = 1, i++;
			}
			else if (a == '&')
			{
				line[i] = 2, i++;
			}
		}
	}

	else if (mode == 2)
	{
		for (i = 0; line[i]; i++)
		{
			a = line[i];
			if (a == 1)
			{
				line[i] = '|';
			}
			else if (a == 2)
			{
				line[i] = '&';
			}
		}
	}
}
/**
 *log_current - control logical operator according to exit status
 *@shell: main structure
 *@current: logical operators linked list
 *Return: void
 */

void log_current(SHELL *shell, log **current)
{
	char sp;
	int stat;

	stat = shell->status;
	sp = (*current)->sep;

	if (stat == 0)
	{
		if (sp == '&' || sp == ';')
		{
			return;
		}
		else if (sp == '|')
		{
			*current = (*current)->next;
		}
	}

	else if (stat != 0)
	{
		if (sp == '|' || sp == ';')
		{
			return;
		}
		else if (sp == '&')
		{
			*current = (*current)->next;
		}
	}
}
/**
 *handle_builtins - this handles builtins
 *@shell: main structure
 *Return: 1 case of exit, 0 upon success and -1 for fail
 */
int handle_builtins(SHELL *shell)
{
	unsigned long int i;
	int exit_status;

	builtin builtin_commands[] = {
		/*{"env", env_},*/
		{"exit", exit_},
		{"cd", _chdir}
	};

	for (i = 0; i < sizeof(builtin_commands) / sizeof(builtin); i++)
	{
		if (_strcmp(shell->toks[0], builtin_commands[i].command) == 0)
		{
			if (_strcmp(builtin_commands[i].command, "exit") == 0)
			{
				if (shell->toks[1])
				{
					exit_status = _atoi(shell->toks[1]);
					shell->status = exit_status;
				}
				else
					shell->status = 0;
				return (1);
			}
			builtin_commands[i].function(shell);
			return (0);
		}
	}
	return (-1);
}


/**
 *env_ - this prints the enviroment variables.
 *@shell: mian structure.
 *Return: nothing.
 */
void env_(SHELL *shell)
{
	char **_env = shell->_environ;

	while (*_env)
	{
		write_i(*_env);
		write_i("\n");
		_env++;
	}
}
