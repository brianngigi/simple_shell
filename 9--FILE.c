#include "shell.h"

/**
 *_atoi- changing a string to int
 *@nptr: the string number
 *Return: the integer
 */
int _atoi(const char *nptr)
{
	int result = 0, sign = 1, i = 0;

	if (nptr[0] == '-')
	{
		sign = -1;
		i = 1;
	}
	while (nptr[i] != '\0')
	{
		if (nptr[i] >= '0' && nptr[i] <= '9')
		{
			result = result * 10 + (nptr[i] - '0');
			i++;
		}
		else
		{
			return (0);
		}
	}
	return (sign * result);
}
/**
 *_strlen - function that count the numbers of characters in a given string.
 * @input: Pointer string whose characters are to be counted.
 *
 * Return: number of character count in the string.
 **/

int _strlen(char *input)
{
	int i;

	for (i = 0; input[i]; i++)
		;
	return (i);
}


/**
 * ext - extesnion for strtok
 * @nextToken: the next token
 * @delim: delimiters used
 * @isDelimiter: tracker for the delimiters
 * Return: nothing
 */

void ext(char *nextToken, const char *delim, int *isDelimiter)
{
	int i;

	for (i = 0; delim[i] != '\0'; i++)
	{
		if (*nextToken == delim[i])
		{
			*isDelimiter = 1;
			break;
		}
	}
}

/**
 * _strlen0 - function to count the number of characters in a given string.
 * @s: Pointer to the string characters to be counted.
 * Return: number of characters counted in the string.
 **/

size_t _strlen0(const char *s)
{
	size_t len = 0;

	while (s[len] != '\0')
	{
		len++;
	}
	return (len);
}
/**
 * _log -  linked list for logical operators
 * @head: head of the linked list
 * @command: input command
 * Return: nothing
 */

void _log(log **head, char *command)
{
	log *current, *new;

	new = malloc(sizeof(log));

	new->com = _strdup(command);
	new->sep = 'a';
	new->next = NULL;

	if (*head == NULL)
	{
		*head = new;
		return;
	}

	current = *head;

	while (current->next != NULL)
	{
		current = current->next;
	}

	current->next = new;
}
