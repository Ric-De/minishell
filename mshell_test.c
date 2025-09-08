/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:07:54 by rdellaza          #+#    #+#             */
/*   Updated: 2025/09/08 11:12:55 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

struct s_command
{
	int	_number_of_available_simple_commands;
	int	_number_of_simple_commands;
	simple_command	**_simple_commands;
	char	*_out_file;
	char	*_input_file;
	char	*_err_file;
	int	_background;
	void	prompt();
	void	print();
	void	execute();
	void	clear();
	command();
	void	insert_simple_command(simple_command *simple_command);
	static	command	_current_command;
	static	simple_command	*_current_simple_command;
}	s_command;
