/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvalenci <jvalenci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 18:40:05 by jvalenci          #+#    #+#             */
/*   Updated: 2022/04/26 18:40:07 by jvalenci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philosophers.h"

/* Instead of waiting certain amount of milliseconds in just one usleep call,
   we will do several calls passing as argument 0.1 milliseconds up until we
   have waited the needed time: For instance, if we need to wait 200 millise
   conds which is equal to 200000 microseconds, we will call usleep(100) 2000
   times in order to wait 200 milliseconds, with this technique we'll accompl
   ish accurateness */
void	ft_usleep(t_simulation *sim, uint64_t wait_time)
{
	uint64_t	time;

	time = ft_get_time();
	while (ft_get_time() - time < wait_time)
		usleep(sim->wait_time);
}

int	ft_check_args(int argc, char *argv[])
{
	int	i;
	int	j;
	int	check;

	i = 0;
	while (++i < argc)
	{
		j = -1;
		while (argv[i][++j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				write(2, "Wrong character provided: ", 26);
				return (write(2, &argv[i][j], 1));
			}
		}
		if (argv[i][0] != '0')
		{
			check = ft_atoi(argv[i]);
			if (!check)
				return (1);
		}
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_simulation	sim;

	if (argc < 5 || argc > 6)
	{
		write(2, "Wrong number of arguments provided\n", 36);
		return (1);
	}
	if (!ft_check_args(argc, argv))
	{
		if (argv[1][0] == '0')
		{
			write(2, "At least one philosopher should be specified\n", 46);
			return (1);
		}
		ft_init_sim(argc, argv, &sim);
		ft_init_phil(&sim);
		ft_launch_threads(&sim);
		ft_free_all(&sim);
	}
	return (0);
}
