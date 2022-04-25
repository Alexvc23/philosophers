/* ************************************************************************** */ /*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvalenci <jvalenci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 09:44:37 by jvalenci          #+#    #+#             */
/*   Updated: 2022/04/23 11:03:24 by jvalenci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philosophers.h"

void ft_msg(t_philos *p, const char *s)
{
    uint64_t time;

    time = (ft_get_time() - p->sim->start_time);
    printf("%lu %d %s \n", time, p->chair, s);
}

int ft_check_vars(t_simulation *sim)
{
    t_philos *p;
    uint64_t current_time;
    int i;

    current_time = ft_get_time();
    i = -1;
    while(++i < sim->nb_philos)
    {
        p = &sim->philos[i];
        if (sim->nb_simu > -1 && (p->nb_sim >= sim->nb_simu))
            sim->phil_finished += 1;
        else if ((current_time - p->last_dinner) > sim->time_to_die)
        {
            ft_msg(p, "died");
            return (0);
        }
    }
    if (sim->nb_simu > -1 && sim->phil_finished >= sim->nb_philos)
        return (0);
    return (1);
}

void    ft_check_death(t_simulation *sim)
{
    while (1)
    {
        pthread_mutex_lock(&sim->c_status);
        sim->phil_finished = 0;
        if (!ft_check_vars(sim))
        {
            sim->end_of_simulation = 1;
            pthread_mutex_unlock(&sim->c_status);
            break ;
        }
        pthread_mutex_unlock(&sim->c_status);
    }
}

void    ft_free_all(t_simulation *t)
{
    int i;
    t_philos *p;
    i = -1;

    while (++i < t->nb_philos)
    {
        p = &t->philos[i];
        if (pthread_join(p->id, NULL))
            printf("Error joining pthread %d\n", i);
    }
    i = -1;
    while (++i < t->nb_philos)
        pthread_mutex_destroy(&t->forks[i]);
    pthread_mutex_destroy(&t->c_status);
    if (t->forks)
        free(t->forks);
    if (t->philos)
        free(t->philos);
}

/* Instead of waiting certain amount of milliseconds in just one
usleep call, we will do several calls passing as argument 0.1 milliseconds
up until we have waited the needed time: For instance, if we need to wait 200
milliseconds which is equal to 200000 microseconds, we will call usleep(100)
2000 times in order to wait 200 milliseconds, with this technique
we'll accomplish accurateness */
void ft_usleep(t_simulation *sim, uint64_t wait_time)
{
    uint64_t    time;

    time = ft_get_time();
    while (ft_get_time() - time < wait_time)
        usleep(sim->wait_time);
}

#include"philosophers.h"

/* Simulation varialbles inicialization which will be conencted to the philophers's struct
having their own variables and letting us keep track of each philosopher's behavior */
int    ft_init_sim(int argc, char *argv[], t_simulation *sim)
{
    int i;
    
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
void ft_init_phil(t_simulation *sim)
{
    int i;
    t_philos *p;

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


void    ft_set_up_action(t_philos *p, int s)
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

void    *ft_set_up_philo(void *p)
{
    t_philos *tmp;

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

void    ft_launch_threads(t_simulation *sim)
{
    t_philos    *p;
    int         i;

    i = -1;
    sim->start_time = ft_get_time();
    ft_init_phil(sim);
    while (++i < sim->nb_philos)
    {
        p = &sim->philos[i];
        if (pthread_create(&p->id, NULL, &ft_set_up_philo, (void*)p))
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

int main(int argc, char *argv[])
{
    t_simulation    sim;

    ft_init_sim(argc, argv, &sim);
    ft_init_phil(&sim);
    ft_launch_threads(&sim);
    ft_free_all(&sim);

    return (0);
}
