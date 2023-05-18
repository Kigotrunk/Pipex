/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kortolan <kortolan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 18:44:12 by kortolan          #+#    #+#             */
/*   Updated: 2023/05/17 18:08:07 by kortolan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "libft/libft.h"

char	*pathname(char **path, char *name);
char	*ft_strjoin2(char const *s1, char const *s2);
void	pip1();
void	pip2();
void	free_split(char **split);

int	main(int argc, char**argv, char* envp[])
{
	int		pid1;
	int		pid2;
	int 	fd[2];

	if (argc != 5)
		return (ft_putstr_fd("Invalid Number arguments", 2));	
	if (pipe(fd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 < 0)
		return (2);
	if (pid1 == 0)
		pip1(envp, argv, fd);
	pid2 = fork();
	if (pid2 < 0)
		return (3);
	if (pid2 == 0)
		pip2(envp, argv, fd);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}


void	pip1(char *envp[], char **argv, int fd[])
{
	char	*test;
	char	**split;
	int		file;

	file = 0;
	file = open(argv[1], O_RDONLY);
	if (file == -1)
		return ;
	test = pathname(envp, argv[2]);
	dup2(file, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	split = ft_split(argv[2], ' ');
	execve(test, split, envp);
	free(test);
	free_split(split);
}

void	pip2(char *envp[], char **argv, int fd[])
{
	char	**split;
	char	*test;
	int		file2;

	unlink(argv[4]);
	file2 = open(argv[4], O_WRONLY | O_CREAT, 0666);
	if (file2 == -1)
		return;
	test = pathname(envp, argv[3]);
	dup2(fd[0], STDIN_FILENO);
	dup2(file2, 1);
	close(fd[0]);
	close(fd[1]);
	split = ft_split(argv[3], ' ');
	execve(test, split, envp);
	free (test);
	free_split(split);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free (split);
}

char	*pathname(char **path, char *name)
{
	int		i;
	int		j;
	char	**split;
//	char	*f;
	char	*temp;

	i = 0;
	j = 1;
	while (ft_strnstr(path[i], "PATH", 5) == NULL)
		i++;
	split = ft_split(path[i] + 5, ':');
	i = 0;
	while (split[i])
	{
		temp = ft_strjoin2(split[i], name);
		if (access(temp, F_OK | X_OK) == 0)
			break ;
		free (temp);
		i++;
	}
	free_split(split);
//	while (access(ft_strjoin2(split[j], name), F_OK | X_OK) != 0)
	//	j++;
	//f = ft_strjoin2(split[j], name);
	return (temp);
}

char	*ft_strjoin2(char const *s1, char const *s2)
{
	int		x;
	int		y;
	char	*s3;

	x = ft_strlen((char *)s1);
	y = ft_strlen((char *)s2);
	if (!s1 && !s2)
		return (0);
	s3 = (char *)malloc((x + y + 2) * sizeof(char));
	if (!s3)
		return (0);
	y = -1;
	while (s1[++y])
		s3[y] = s1[y];
	y = -1;
	s3[x] = '/';
	x++;
	while (s2[++y] && s2[y] != ' ')
	{
		s3[x] = s2[y];
		x++;
	}
	s3[x] = '\0';
	return (s3);
}
