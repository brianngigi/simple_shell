#include "shell.h"

#define buffsize 1024

/**
 * _realloc - the function that reallocates memory using malloc.
 * @ptr: Pointer of the memory to be reduced.
 * @old_size: old size of the memory to be changed.
 * @size: new size to change to.
 * Return: nothing.
 */

void *_realloc(void *ptr, size_t old_size, size_t size)
{
	void *new_ptr;
	char *old_ptr = (char *)ptr;
	char *new_char_ptr;
	size_t i;

	if (!ptr)
	{
		return (malloc(size));
	}
	else if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	else
	{
		new_ptr = malloc(size);
		if (!new_ptr)
		{
			return (NULL);
		}
		if (size < old_size)
		{
			old_size = size;
		}
		new_char_ptr = (char *)new_ptr;
		for (i = 0; i < old_size; i++)
		{
			new_char_ptr[i] = old_ptr[i];
		}
		free(ptr);
		return (new_ptr);
	}
}


/**
 * _getline - the Function that fetches the entire input from the output
 * stream.
 * Return: input fetched from the stream.
 */


char *_getline()
{
	char *buffer, r = 0;
	int i = 0, charsRead, buffer_size;

	buffer_size = buffsize,	buffer = malloc(buffer_size * sizeof(char));

	if (buffer == NULL)
	{
		free(buffer);
		return (NULL);
	}

	while (r != EOF && r != '\n')
	{
		fflush(stdin);
		charsRead = read(STDIN_FILENO, &r, 1);
		if (charsRead == 0)
		{
			free(buffer);
			exit(EXIT_SUCCESS);
		}
		buffer[i] = r;
		if (buffer[0] == '\n')
		{
			free(buffer);
			return ("\0");
		}

		i++;
	}
	buffer[i] = '\0', buffer = (buffer);
	if (buffer == NULL)
	{
		return ("\0");
	}
	return (buffer);
}

/**
 * handle_hash - handles the case where # has been used in a command.
 * @buffer: pointer to where the user input is stored.
 * Return: the buffer after the #has been ignored hence a comment.
 */

char *handle_hash(char *buffer)
{
	int i;
	bool quotes = false;

	if (buffer[0] == '#')
	{
		free(buffer);
		return (NULL);
	}

	else
	{
		for (i = 0; buffer[i]; i++)
		{
			if (buffer[i] == 34)
			{
				quotes = !quotes;
			}

			if (!quotes && buffer[i] == '#' && buffer[i - 1] == ' ')
			{
				buffer[i] = '\0';
				break;
			}
		}
	}

	return (buffer);
}

/**
 * _Strtok - a function that Tokenizes a string based on whatever it is given.
 * @str: Pointer to the string to be Tokenized.
 * @delim: delimiter which strtok function use to Tokenize the string.
 * Return: The Tokenized words, else return NULL on failure.
 */

char *_Strtok(char *str, const char *delim)
{
	static char *nextToken;
	char *token = NULL;
	int isDelimiter;

	if (str != NULL)
		nextToken = str;
	if (nextToken == NULL || *nextToken == '\0')
		return (NULL);
	while (*nextToken != '\0')
	{
		isDelimiter = 0;
		ext(nextToken, delim, &isDelimiter);
		if (!isDelimiter)
			break;
		nextToken++;
	}
	if (*nextToken == '\0')
		return (NULL);
	token = nextToken;
	while (*nextToken != '\0')
	{
		isDelimiter = 0;
		ext(nextToken, delim, &isDelimiter);
		if (isDelimiter)
		{
			*nextToken = '\0', nextToken++;
			break;
		}
		nextToken++;
	}
	return (token);
}


/**
 * _getenv - this gets environment variables
 * @name: a name of the variable
 *
 * Return: NULL on failure, else pointer to the value of the
 * environment variable.
 **/

char *_getenv(const char *name)
{
	size_t name_length = 0;
	int match;
	char **env;
	size_t i;

	if (name == NULL || *name == '\0')
	{
		return (NULL);
	}
	while (name[name_length] != '\0')
		name_length++;
	for (env = environ; *env != NULL; env++)
	{
		match = 1;
		for (i = 0; i < name_length; i++)
		{
			if ((*env)[i] != name[i])
			{
				match = 0;
				break;
			}
		}
		if (match && (*env)[name_length] == '=')
		{
			return (&(*env)[name_length + 1]);
		}
	}
	return (NULL);
}
