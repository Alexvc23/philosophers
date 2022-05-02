/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_of_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvalenci <jvalenci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 15:01:02 by jvalenci          #+#    #+#             */
/*   Updated: 2022/05/02 15:12:22 by jvalenci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_check_vars(t_mutex *mutex)
{
	t_philos	*p;
	uint64_t	current_time;
	int			i;
	int			phil_finished;

	current_time = ft_get_time();
	i = -1;
	phil_finished = 0;
	while (++i < mutex->sim->nb_philos)
	{
		p = &mutex->philos[i];
		if (p->sim->nb_simu > -1 && (p->nb_sim >= p->sim->nb_simu))
			phil_finished += 1;
		else if ((current_time - p->last_dinner) > p->sim->time_to_die)
		{
			ft_msg(p, "died");
			return (1);
		}
	}
	if (p->sim->nb_simu > -1 && phil_finished == p->sim->nb_philos)
		return (1);
	return (0);
}

void	ft_check_death(t_mutex *mutex)
{
	while (1)
	{
		pthread_mutex_lock(&mutex->c_status);
		if (ft_check_vars(mutex))
		{
			mutex->sim->end_of_simulation = 1;
			pthread_mutex_unlock(&mutex->c_status);
			break ;
		}
		pthread_mutex_unlock(&mutex->c_status);
	}
}

void	ft_free_all(t_mutex *mutex)
{
	int			i;
	t_philos	*p;

	i = -1;
	if (ft_end_of_sim(mutex))
	{
		while (++i < mutex->sim->nb_philos)
		{
			p = &mutex->philos[i];
			if (pthread_join(p->id, NULL))
				printf("Error joining thread: %d\n", i);
		}
		i = -1;
		while (++i < mutex->sim->nb_philos)
			pthread_mutex_destroy(&mutex->sim->forks[i]);
		if (mutex->sim->forks)
			free(mutex->sim->forks);
		if (mutex->philos)
			free(mutex->philos);
	}
}
