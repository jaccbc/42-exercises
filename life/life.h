/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <joandre-@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 20:58:19 by joandre-          #+#    #+#             */
/*   Updated: 2025/12/09 23:20:56 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIFE_H
# define LIFE_H

#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_life {
  int width;
  int height;
  int iter; 
  char** grid;

} t_life;

void cleanLife(char**);
bool initLife(t_life*, char**);
bool play(t_life*);

#endif
