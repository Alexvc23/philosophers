/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_launch.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvalenci <jvalenci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 15:03:00 by jvalenci          #+#    #+#             */
/*   Updated: 2022/05/06 11:50:59 by jvalenci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philosophers.h"

/* locks sumulation structures and verify if end of simulation */
int	ft_end_of_sim(t_mutex *mutex)
{
	pthread_mutex_lock(&mutex->c_status);
	if (mutex->sim->end_of_simulation == 1)
	{
		pthread_mutex_unlock(&mutex->c_status);
		return (1);
	}
	pthread_mutex_unlock(&mutex->c_status);
	return (0);
}

/* Action to be taking, blocking simulation structure to avoid
data races  */
void	ft_set_up_action(t_philos *p, int s)
{
	pthread_mutex_lock(&p->mutex->c_status);
	if (p->sim->end_of_simulation == 1)
	{
		pthread_mutex_unlock(&p->mutex->c_status);
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
	pthread_mutex_unlock(&p->mutex->c_status);
}

/* before executing the basic algorithm, even philsophers's will sleep
eat time milliseconds before going ahead, in order to let odd philosopher
take their forks, finish their execution and avoid deack lock(all philosophers 
bloking their left fork), then we take each fork, we eat, sleep and think with 
its assigned time */
void	*ft_set_up_philo(void *philo)
{
	t_philos	*tmp;

	tmp = (t_philos *)philo;
	if (tmp->chair % 2 == 0)
		ft_usleep(tmp->sim, 1);
	while (!ft_end_of_sim(tmp->mutex))
	{
		ft_set_up_action(tmp, THINK);
		pthread_mutex_lock(tmp->left_fork);
		ft_set_up_action(tmp, FORK);
		if (tmp->right_fork == tmp->left_fork)
			return (NULL);
		pthread_mutex_lock(tmp->right_fork);
		ft_set_up_action(tmp, FORK);
		ft_set_up_action(tmp, EAT);
		ft_usleep(tmp->sim, tmp->sim->time_to_eat);
		pthread_mutex_unlock(tmp->left_fork);
		pthread_mutex_unlock(tmp->right_fork);
		ft_set_up_action(tmp, SLEEP);
		ft_usleep(tmp->sim, tmp->sim->time_to_sleep);
	}
	return (NULL);
}

/* thread creation passing as argument each philo structure and 
launching ft_check_dead function wich will verify end of simulation */
void	ft_launch_threads(t_mutex *mutex)
{
	t_philos	*p;
	int			i;

	i = -1;
	mutex->sim->start_time = ft_get_time();
	while (++i < mutex->sim->nb_philos)
	{
		p = &mutex->philos[i];
		if (pthread_create(&p->id, NULL, &ft_set_up_philo, p))
		{
			printf("Error creating pthread %d\n", i);
			pthread_mutex_lock(&mutex->c_status);
			mutex->sim->end_of_simulation = 1;
			pthread_mutex_unlock(&mutex->c_status);
			return ;
		}
	}
	ft_check_death(mutex);
}
