/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvalenci <jvalenci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 10:58:55 by jvalenci          #+#    #+#             */
/*   Updated: 2022/04/25 15:15:01 by jvalenci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philosophers.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*ptr1;
	unsigned char	*ptr2;

	ptr1 = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	if (!n)
		return (0);
	while (*ptr1 && *ptr2 && *ptr1 == *ptr2 && --n)
	{
		ptr1++;
		ptr2++;
	}
	return (*ptr1 - *ptr2);
}

uint64_t	ft_get_time(void)
{
	uint64_t		milliseconds;
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	milliseconds = tp.tv_sec * 1000;
	milliseconds += tp.tv_usec / 1000;
	return (milliseconds);
}

void	ft_msg(t_philos *p, const char *s)
{
	uint64_t	time;

	time = (ft_get_time() - p->sim->start_time);
	printf("%lu %d %s \n", time, p->chair, s);
}

/* Simulation varialbles inicialization which will be conencted to the philoso
   pher's struct having their own variables and letting us keep track of each
   philosopher's behavior */
int	ft_init_sim(int argc, char *argv[], t_simulation *sim)
{
	int	i;

	i = -1;
	sim->start_time = ft_get_time();
	sim->nb_philos = ft_atoi(argv[1]);
	sim->wait_time = 100;
	sim->phil_finished = 0;
	if (sim->nb_philos > 100)
		sim->wait_time = 1000;
	sim->time_to_die = ft_atoi(argv[2]);
	sim->time_to_eat = ft_atoi(argv[3]);
	sim->time_to_sleep = ft_atoi(argv[4]);
	sim->nb_simu = -1;
	sim->end_of_simulation = 0;
	if (argc == 6)
		sim->nb_simu = ft_atoi(argv[5]);
	sim->forks = calloc(sim->nb_philos, sizeof(pthread_mutex_t));
	sim->philos = calloc(sim->nb_philos, sizeof(t_philos));
	if (!sim->philos || !sim->forks)
		return (write(2, "Error allocating memory 'Calloc'\n", 34));
	pthread_mutex_init(&sim->c_status, NULL);
	while (++i < sim->nb_philos)
		pthread_mutex_init(&sim->forks[i], NULL);
	return (0);
}

/* Philosopher's struct variables inicialization */
void	ft_init_phil(t_simulation *sim)
{
	int			i;
	t_philos	*p;

	i = -1;
	while (++i < sim->nb_philos)
	{
		p = &sim->philos[i];
		p->alive = 1;
		p->last_dinner = sim->start_time;
		p->chair = i + 1;
		p->left_fork = &sim->forks[i];
		p->right_fork = &sim->forks[(i + 1) % sim->nb_philos];
		p->nb_sim = 0;
		p->sim = sim;
	}
}
