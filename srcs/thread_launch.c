/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_lauch.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvalenci <jvalenci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 15:03:00 by jvalenci          #+#    #+#             */
/*   Updated: 2022/04/25 15:03:02 by jvalenci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philosophers.h"

void	ft_set_up_action(t_philos *p, int s)
{
	pthread_mutex_lock(&p->sim->c_status);
	if (p->sim->end_of_simulation == 1)
	{
		pthread_mutex_unlock(&p->sim->c_status);
		return ;
	}
	else if (s == FORK)
		ft_msg(p, MSG_FORK);
	else if (s == EAT)
	{
		ft_msg(p, MSG_EAT);
		p->last_dinner = ft_get_time();
	}
	else if (s == SLEEP)
		ft_msg(p, MSG_SLEEP);
	else if (s == THINK)
	{
		ft_msg(p, MSG_THINK);
		p->nb_sim += 1;
	}
	pthread_mutex_unlock(&p->sim->c_status);
}

void	*ft_set_up_philo(void *p)
{
	t_philos	*tmp;

	tmp = p;
	if (tmp->chair % 2 == 0)
		ft_usleep(tmp->sim, tmp->sim->time_to_eat);
	while (!tmp->sim->end_of_simulation)
	{
		pthread_mutex_lock(tmp->left_fork);
		ft_set_up_action(p, FORK);
		pthread_mutex_lock(tmp->right_fork);
		ft_set_up_action(tmp, FORK);
		ft_set_up_action(tmp, EAT);
		ft_usleep(tmp->sim, tmp->sim->time_to_eat);
		pthread_mutex_unlock(tmp->left_fork);
		pthread_mutex_unlock(tmp->right_fork);
		ft_set_up_action(tmp, SLEEP);
		ft_usleep(tmp->sim, tmp->sim->time_to_sleep);
		ft_set_up_action(tmp, THINK);
	}
	return (NULL);
}

void	ft_launch_threads(t_simulation *sim)
{
	t_philos	*p;
	int			i;

	i = -1;
	sim->start_time = ft_get_time();
	ft_init_phil(sim);
	while (++i < sim->nb_philos)
	{
		p = &sim->philos[i];
		if (pthread_create(&p->id, NULL, &ft_set_up_philo, (void *)p))
		{
			printf("Error creating pthread %d\n", i);
			pthread_mutex_lock(&sim->c_status);
			sim->end_of_simulation = 1;
			pthread_mutex_unlock(&sim->c_status);
			return ;
		}
	}
	ft_check_death(sim);
}
