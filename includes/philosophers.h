/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvalenci <jvalenci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 09:45:11 by jvalenci          #+#    #+#             */
/*   Updated: 2022/04/26 18:21:34 by jvalenci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

// ─── LIBRARIES ──────────────────────────────────────────────────────────────

/* pthread_create, pthread_join, pthread_mutex_t, pthread_mutex_init
   pthread_mutex_lock, pthread_mutex_unlock */
# include <pthread.h>

/* malloc, free*/
# include <stdlib.h>

/* printf */
# include <stdio.h>

/* gettimeofday */
# include<sys/time.h>

/* uint64_t*/
# include <stdint.h>

/* write */
# include <unistd.h>

# define FORK 1
# define EAT 2
# define SLEEP 3 
# define THINK 4 
# define DIE 5 
# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"

// ─── STRUCTS ────────────────────────────────────────────────────────────────

typedef struct s_simulation	t_simulation;
typedef struct s_philos		t_philos;

struct	s_simulation
{
	uint64_t		start_time;
	int				end_of_simulation;
	int				nb_philos;
	int				wait_time;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	int				nb_simu;
	int				phil_finished;
	pthread_mutex_t	*forks;
	pthread_mutex_t	c_status;
	t_philos		*philos;
};

struct s_philos
{
	int				chair;
	int				nb_sim;
	uint64_t		last_dinner;
	pthread_t		id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_simulation	*sim;
};

// ─── FUNCTIONS ──────────────────────────────────────────────────────────────

int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_atoi(const char *str);
uint64_t	ft_get_time(void);
int			ft_init_sim(int argc, char *argv[], t_simulation *sim);
void		ft_init_phil(t_simulation *sim);
void		ft_usleep(t_simulation *sim, uint64_t wait_time);
void		ft_free_all(t_simulation *t);
void		ft_check_death(t_simulation *sim);
void		ft_launch_threads(t_simulation *sim);
void		ft_msg(t_philos *p, const char *s);

#endif
