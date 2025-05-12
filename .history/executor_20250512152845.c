/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:17:53 by lalves-d          #+#    #+#             */
/*   Updated: 2025/04/07 15:17:58 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void executor( t_token tokens)
{
    if(ft_strncmp(tokens->value, "exit", 4) == 0 && (tokens->value[4] == ' ' || tokens->value[4] == '\0'))
      ft_exit(tokens, input);
    if(ft_strncmp(tokens->value, "echo", 4) == 0)
        ft_echo(tokens);
    if(ft_strncmp(tokens->value, "pwd", 3) == 0)
        ft_pwd();
    if(ft_strncmp(tokens->value, "cd", 2) == 0)
        ft_cd(tokens, path_name);
    if(ft_strncmp(tokens->value, "export", 6) == 0)
        ft_export(tokens);
    if(ft_strncmp(tokens->value, "unset", 5) == 0)
        ft_unset(tokens);
    if(ft_strncmp(tokens->value, "env", 3) == 0)
        ft_env();
}
