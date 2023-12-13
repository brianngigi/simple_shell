#include "shell.h"

/**
 *global_variable - checks for eviroment variables
 *@input: the input strig
 *@shell: the main struct
 *Return: an intergrated string
 */

char *global_variable(char *input __attribute__ ((unused)), SHELL *shell)
{
	char *status, *id;
	int i;

	for (i = 0; input[i]; i++)
	{
		if (input[i] == '$')
		{
			if (input[i + 1] == '?')
			{
				status = _itoa(shell->status);
				return (exit_status(input, status));
			}

			else if (input[i + 1] == '$')
			{
				id = _itoa(shell->pid);
				return (identity(input, id));
			}

			else
			{
				return (en_var(input, shell));
			}
		}
	}


	return (input);
}

/**
 *exit_status - handles printig of the exit status
 *@input: the input string
 *@status: a status string
 *Return: a string combined
 */

char *exit_status(char *input, char *status)
{
	int i, len, j, l;
	char *new;

	len = (_strlen((input) + _strlen(status)));
	new = malloc(len + 1);

	if (!new)
	{
		write_i("MEMORY ALLOCATION FAILURE\n");
		exit(EXIT_FAILURE);
	}

	for (i = j = 0; input[i]; i++)
	{
		if (input[i] == '$')
		{
			for (l = 0; l < _strlen(status); l++)
			{
				new[j] = status[l];
				j++;
			}
			i++;
		}

		else
		{
			new[j] = input[i];
			j++;
		}
	}

	new[j] = '\0';
	free(status);
	free(input);
	return (new);
}

/**
 *identity - handles the pid
 *@input: string input
 *@id: string id
 *Return: the string with the id intergrated
 */

char *identity(char *input, char *id)
{
	int i, len, j, l;
	char *new;

	len = (_strlen(input) + _strlen(id));
	new = malloc(len + 1);

	if (!new)
	{
		write_i("MEMORY ALLOCATION FAILURE\n");
		exit(EXIT_FAILURE);
	}

	for (i = j = 0; input[i]; i++)
	{
		if (input[i] == '$')
		{
			for (l = 0; l < _strlen(id); l++)
			{
				new[j] = id[l];
				j++;
			}
			i++;
		}

		else
		{
			new[j] = input[i];
			j++;
		}
	}

	new[j] = '\0';
	free(id);
	free(input);
	return (new);
}

/**
 *en_var - this handles enviroment variables
 *@input: string input
 *@shell: the main structure
 *Return: string with enviroment variable integrated
 */

char *en_var(char *input, SHELL *shell __attribute__ ((unused)))
{
	char *ename, *copy, *new;
	int len;

	ename = input, copy = NULL;
	while (*ename)
	{
		if (*ename == '$')
		{
			break;
		}
		ename++;
	}
	ename = _Strtok(ename, " \n");
	if (!*(ename + 1))
	{
		copy = "$";
	}
	else
	{
		ename++;
		copy = _getenv(ename);
	}
	if (copy)
	{
		len = _strlen(input) + (_strlen(copy) - _strlen(ename));
		new = malloc((len + 1) * sizeof(char));
	}
	else
	{
		new = malloc((_strlen((input) + 1) * sizeof(char)));
	}
	if (!new)
	{
		exit(EXIT_FAILURE);
	}
	return (Replace(input, new, copy, (*ename == '$' ? ename : ename - 1)));
}

/**
 *Replace - where the swapping takes place
 *@input: an input string
 *@new: a new mallocated string
 *@ename: the replaced name
 *@copy: enviroment variable copied
 *Return: a itergrated string
 */

char *Replace(char *input, char *new, char *copy, char *ename)
{
	int i, j, k;

	if (copy)
	{
		for (i = j = 0; input[i]; i++)
		{
			if (input[i] == '$')
			{
				for (k = 0; copy[k]; k++)
				{
					new[j] = copy[k];
					j++;
				}
				i += _strlen(ename);
			}
			else
			{
				new[j] = input[i];
				j++;
			}
		}
	}
	else if (copy == NULL)
	{
		for (i = j = 0; input[i]; i++)
		{
			if (input[i] == '$')
			{
				i += _strlen(ename);
			}
			else
			{
				new[j] = input[i];
				j++;
			}
		}
	}
	new[j] = '\0';
	free(input);
	return (new);
}
