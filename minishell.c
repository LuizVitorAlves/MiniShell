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

#include "minishell.h"

int main(int argc, char *argv[])
{
    char *input;
    char    **args;

    (void )argv;
    (void )argc;
    while(1)
    {
        input = readline("minishell$");
        if(!input)
        {
            exit(0);
        }
        else
        {
            args = put_args_array(input);
        }
        if (input[0])
			add_history(input);
        
        if(ft_strncmp(args[0], "exit", 4) == 0 && (args[0][4] == '\0' || args[0][4] == ' '))
        {
            //liberar a matriz args
            //liberar a matriz input
            exit(0);
        }
        // tokens = tokenize(input);
		// print_tokens(tokens);
		// free_tokens(input);
        free(input);
    }
}








