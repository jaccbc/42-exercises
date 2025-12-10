/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <joandre-@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 20:59:38 by joandre-          #+#    #+#             */
/*   Updated: 2025/12/09 23:24:59 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "life.h"

static bool checkArgs(int argc, char** argv) {
  if (argc != 4) return false;
  for (int c = 1; c < argc; c++) {
    if (!argv[c]) return false;
    for (int i = 0; argv[c][i]; i++) {
      if (!(argv[c][i] >= '0' && argv[c][i] <= '9'))
        return false;
    }
  }
  return true;
}

int main(int argc, char** argv) {
  if (checkArgs(argc, argv) == false) return -1;
  t_life game = {0};
  if (initLife(&game, argv) == false) return 1;
  for (int i = 0; i < game.iter; i++) {
    if (play(&game) == false) {
      cleanLife(game.grid);
      return 2;
    }
  }
  for (int x = 0; x < game.height; x++) {
    for (int y = 0; game.grid[x][y]; y++)
      putchar(game.grid[x][y]);
    putchar('\n');
  }
  return cleanLife(game.grid), 0;
}
