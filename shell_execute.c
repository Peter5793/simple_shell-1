#include "holberton.h"
/**
 * shell_execute - executes the built in commands
 * @argv: pointer to the argv
 * @built_in_arr: pointer to the struct with built in comm
 * Return: 1 on success
 **/
int shell_execute(char **argv, built_in_t built_in_arr[])
{
	int i = 0;

	if (argv[0] == NULL)
		return (1);

	while (i < 5)
	{
		if (_strcmp(argv[0], built_in_arr[i].command) == 0)
		{
			return (built_in_arr[i].f(argv));
		}
		i++;
	}
	return (shell_launch(argv));
}
/**
 * shell_launch - creates the process to execute shell commands
 * @argv: pointer to argv
 * Return: 1 on success
 */
int shell_launch(char **argv)
{
	int pid, existence, file_found = -1, c, status;
	char **path = 0, *command = 0, *path_command = 0, *env = 0;

	(void)file_found;
	pid = fork();
	if (pid == 0)
	{
		env = _getenv("PATH");
		path = token_buff(env, ":");
		for (c = 0; path[c]; c++)
		{
			command = _strcat("/", argv[0]);
			path_command = _strcat(path[c], command);
			existence = check_existence(path_command);
			if (existence != -1)
			{
				argv[0] = path_command;
				file_found = 0;
				break;
			}
			else
				free(path_command);
			free(command);
		}

		existence = check_existence(argv[0]);

		if (existence == -1)
			_freeall(argv, path);

		if (execve(argv[0], argv, environ) == -1)
			perror("Error");
		_freeall(argv, path);
	}
	else if (pid < 0)
		perror("hsh");
	else
		wait(&status);
	return (WEXITSTATUS(status));
}
/**
 * _freeall - frees arv and path
 * @argv: Buffer containing the tokens
 * @path: path to look for exec files
 */
void _freeall(char **argv, char **path)
{
	free(argv[0]);
	free(argv);
	free(path);
	exit(127);
}
/**
 * check_existence - checkes whether a file exists
 * @path: pointer to the path to search in
 * Return: 1 on success, -1 if failed
 */
int check_existence(char *path)
{
	int fd = access(path, F_OK | X_OK);

	if (fd == -1)
		return (-1);
	return (1);
}
