/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_of_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvalenci <jvalenci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 15:01:02 by jvalenci          #+#    #+#             */
/*   Updated: 2022/04/25 15:01:24 by jvalenci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int ft_check_vars(t_simulation *sim)
{
    t_philos *p;
    uint64_t current_time;
    int i;

    current_time = ft_get_time();
    i = -1;
    while (++i < sim->nb_philos)
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

void ft_check_death(t_simulation *sim)
{
    while (1)
    {
        pthread_mutex_lock(&sim->c_status);
        sim->phil_finished = 0;
        if (!ft_check_vars(sim))
        {
            sim->end_of_simulation = 1;
            pthread_mutex_unlock(&sim->c_status);
            break;
        }
        pthread_mutex_unlock(&sim->c_status);
    }
}

void ft_free_all(t_simulation *t)
{
    int i;
    t_philos *p;
    i = -1;

    while (++i < t->nb_philos)
    {
        p = &t->philos[i];
        if (!p->id)
        {
            if (pthread_join(p->id, NULL))
                printf("Error joining pthread %d\n", i);
        }
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