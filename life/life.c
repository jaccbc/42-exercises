/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <joandre-@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:14:57 by joandre-          #+#    #+#             */
/*   Updated: 2025/12/10 00:37:28 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "life.h"

void cleanLife(char** board) {
  if (!board) return ;
  for (int i = 0; board[i]; i++) free(board[i]);
  free(board);
}

void fillLifeGrid(t_life* game) {
  bool draw = false;
  char buffer = 0;
  int w = 0;
  int h = 0;
  while (read(STDIN_FILENO, &buffer, 1) == 1) {
    bool cmd = true;
    switch (buffer) {
      case 'w':
        --h; break;
      case 's':
        ++h; break;
      case 'a':
        --w; break;
      case 'd':
        ++w; break;
      case 'x':
        draw = !draw; break;
      default:
        cmd = false; break;
    }
    if (draw && cmd && w >= 0 && w < game->width && h >= 0 && h < game->height)
      game->grid[h][w] = 'O';
  }
}

bool initLife(t_life* game, char** argv) {
  game->width = atoi(argv[1]);
  game->height = atoi(argv[2]);
  game->iter = atoi(argv[3]);
  game->grid = malloc((game->height+1)*sizeof(char*));
  if (game->grid == NULL) return false;
  for (int i = 0; i < game->height+1; i++) game->grid[i] = NULL;
  for (int x = 0; x < game->height; x++) {
    game->grid[x] = calloc(game->width+1, sizeof(char));
    if (game->grid[x] == NULL) return cleanLife(game->grid), false;
    for (int i = 0; i < game->width; i++) game->grid[x][i] = ' ';
  } 
  return fillLifeGrid(game), true;
}


static int neighborCount(t_life* game, int x, int y) {
  int count = 0;
  for (int h = -1; h < 2; h++) {
    for (int w = -1; w < 2; w++) {
      if (h == 0 && w == 0) continue;
      int nH = x + h;
      int nW = y + w;
      if (nH >= 0 && nH < game->height && nW >= 0 && nW < game->width)
        if (game->grid[nH][nW] == 'O') ++count;
    }
  }
  return count;
}

bool play(t_life* game) {
  char** board = malloc((game->height+1)*sizeof(char*));
  if (board == NULL) return false;
  for (int i = 0; i <= game->height; ++i) board[i] = NULL;
  for (int i = 0; i < game->height; ++i) {
    board[i] = calloc(game->width+1, sizeof(char));
    if (board[i] == NULL) return cleanLife(board), false;
  }
  for (int x = 0; x < game->height; x++) {
    for (int y = 0; y < game->width; y++) {
      int n = neighborCount(game, x, y);
      if (game->grid[x][y] == 'O')
        (n == 2 || n == 3) ? (board[x][y] = 'O') : (board[x][y] = ' ');
      else
        (n == 3) ? (board[x][y] = 'O') : (board[x][y] = ' ');
    }
  }
  cleanLife(game->grid);
  game->grid = board;
  return true;
}
