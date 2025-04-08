/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:16:55 by lalves-d          #+#    #+#             */
/*   Updated: 2025/04/07 15:17:18 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char *argv[])
{
    char *input;

    while(1)
    {
        input = readline("minishell$");
        if(!input)
        {
            exit(0);
        }
        else
            printf("%s \n", input);
        free(input);
    }
}








