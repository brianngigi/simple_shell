#include "shell.h"


/**
 * _sigint - Functions that handles CTRL C.
 * @signum: represent the single number that triggers the signal.
 * Return: void.
 */

void _sigint(int signum __attribute__((unused)))
{
	write(STDOUT_FILENO, "\n", 1);
	if (isatty(STDIN_FILENO))
	{
		write_i("$");
		fflush(stdout);
	}
}

/**
 *main - the entry point
 *@ac: arguement count
 *@argv: argurment vector
 *Return: the status of the exectutions
 */

int main(int ac __attribute__ ((unused)), char **argv)
{
	char *line;
	int mode = 1;
	SHELL eshell;

	eshell.av = argv;
	fill_shell(&eshell);

	if (argv[1])
	{
		file_as_arg(&eshell);
	}
	signal(SIGINT, _sigint);

	while (mode)
	{
		line = NULL;
		eshell.loop_count++;

		if (isatty(STDIN_FILENO))
			write_i("Entry/User$");

		line = _getline();
		line = global_variable(line, &eshell);

		if (line[0] == '\0' || !line)
		{
			continue;
		}

		if (syntax(line, &eshell) == -1)
		{
			free(line);
			continue;
		}

		mode = _source(&eshell, line);
		free(line);
		line = NULL;
	}
	return (eshell.status);
}


/**
 * fill_shell - Functions that fill the structure members with respective
 * content.
 * @eshell: structure to be filled.
 * Return: Void.
 */

void fill_shell(SHELL *eshell)
{
	eshell->pid = getpid();
	eshell->status = 0;
	eshell->_environ = NULL;
	eshell->loop_count = 0;
}


/**
 * write_i - Function that writes content given to the POSIX.
 * @input: Pointer to content to be written out.
 * Return: Void.
 */

void write_i(char *input)
{
	int i;

	for (i = 0; input[i]; i++)
		;

	write(STDOUT_FILENO, input, i);
}
/**
 *sep - determine which command should be executed next
 *@head: head of linked list
 *@line: Tokenized command
 *Return: nothing
 */

void sep(log **head, char *line)
{
	int i;
	log *current;
	char a;


	line_shuffler(line, 1);
	current = *head;

	for (i = 0; line[i]; i++)
	{
		a = line[i];

		if (a == '|')
		{
			current->sep = '|';
			current = current->next;
		}

		else if (a == '&')
		{
			current->sep = '&';
			current = current->next;
		}

		else if (a == ';')
		{
			current->sep = ';';
			current = current->next;
		}

	}

	line_shuffler(line, 2);
}
