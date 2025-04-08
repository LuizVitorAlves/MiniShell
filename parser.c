/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:17:43 by lalves-d          #+#    #+#             */
/*   Updated: 2025/04/07 15:17:45 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    **put_args_array(char *input)
{
    char    **args;
   
    printf("%s \n", input);

    args = ft_split(input, ' ');
    return(args);
}
