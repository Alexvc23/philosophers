/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvalenci <jvalenci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 09:45:11 by jvalenci          #+#    #+#             */
/*   Updated: 2022/04/25 10:51:02 by jvalenci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PHILOSOPHERS_H
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

typedef struct  s_simulation    t_simulation;
typedef struct  s_philos        t_philos;
    
struct  s_simulation 
{
    uint64_t        start_time;
    int             end_of_simulation;
    int             nb_philos;
    int             wait_time; // microseconds to be used in our usleep function, read comment on function
    uint64_t        time_to_die; 
    uint64_t        time_to_eat;
    uint64_t        time_to_sleep;
    int             nb_simu; // # of simulation if provided in arguments
    int             phil_finished; // if # of simulations provided. Have all the philosophers eaten certain amount of times? 
    pthread_mutex_t *forks; 
    pthread_mutex_t c_status; 
    t_philos        *philos;
} ;

struct s_philos
{
    int         chair; // philo number
    int         nb_sim; // # of times this philo has eaten or made the simulation
    int         alive;
    uint64_t   last_dinner; // last time this philo has eaten
    pthread_t   id; // current thread id executing this philosopher 
    pthread_mutex_t *left_fork; // pointer towars mutex index representing current philo [i]
    pthread_mutex_t *right_fork; // pointer towar mutex index representing next philo fork + 1 [(i + 1) % nb_philos] 
    t_simulation *sim; // connextion to simuation structure
};

// ─── FUNCTIONS ──────────────────────────────────────────────────────────────

int ft_strncmp(const char *s1, const char *s2, size_t n);
int ft_atoi(const char *str);
uint64_t ft_get_time(void);
    
#endif