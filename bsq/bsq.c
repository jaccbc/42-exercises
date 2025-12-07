/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsq.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <joandre-@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:42:22 by joandre-          #+#    #+#             */
/*   Updated: 2025/12/07 22:05:30 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsq.h"

// cleans up the map grid
static void cleanMap(char** map) {
  if (!map) return ;
  size_t i = 0;
  while (map[i]) free(map[i++]);
  free(map);
}

/*
  reads the map from the FILE* (fp) to the map->grid
  every call to getline needs further memory deallocation
  len to pass as argument, read = n chars read on the line, r = number of rows
  assign NULL to every char* in map->grid for clean memory deallocation afterwards
  reads the file until getline() return -1
*/
static bool readMap(FILE* fp, t_bsq* map) {
  char* line = NULL;
  size_t len = 0;
  int read, r = 0;
  map->grid = malloc((map->rows+1) * sizeof(char*));
  if (map->grid == NULL) return false;
  for (int i = 0; i < map->rows + 1; i++) map->grid[i] = NULL;
  while ((read = getline(&line, &len, fp)) != -1LL) {
    if (line[read-1] == '\n') line[read-1] = '\0';
    if (r == 0) {
      if (read == 0) { return free(line), false; }
      map->cols = read;
    }
    else if (read != map->cols) { free(line); return false; }
    for (int i = 0; line[i]; i++) {
      if (line[i] != map->e && line[i] != map->o) {
        return free(line), false;
      }
    }
    if (map->rows == r) return free(line), false; 
    map->grid[r++] = line;
    line = NULL;
  }
  return free(line), true;
}

/*
  checks the map as per subject requeriments
  reads the first line of the file (FILE*) with getline()
  uses sscanf() to read the content of the line char*
  and assigns the values to the t_bsq int values (obstacles, empty, full)
  checks if any char repeats among them int values
  returns true if all the chars are printable
*/
static bool isHeaderValid(t_bsq *map, FILE* fp) {
  char* line = NULL;
  size_t n = 0;
  if (getline(&line, &n, fp) == -1
    || sscanf(line, "%d %c %c %c", &map->rows, &map->e, &map->o, &map->f) != 4)
    return free(line), false;
  free(line);
  if (map->rows <= 0) return false;
  if (map->e == map->o || map->e == map->f || map->o == map->f) return false;
  return ((map->e >= ' ' &&  map->e <= '~') && (map->f >= ' ' && map->f <= '~')
  && (map->o >= ' ' && map->o <= '~'));
}

// returns minimum of 3 values
static int min3(int a, int b, int c) {
  int m = a < b ? a : b;
  return m < c ? m : c;
}

/*
  finds the square in the map
  a = row index of the bottom right corner of the best square
  b = col index of the bottom right corner of the best square
  size = side len of the best square
*/
static void findSquare(t_bsq* map, int* a, int* b, int* size) {
  int* prev = calloc(map->cols + 1, sizeof(int));
  int* cur = calloc(map->cols + 1, sizeof(int));
  if (!prev || !cur) {
    free(prev); free(cur);
    fprintf(stderr, "map error\n");
    return ;
  }
  int max_sz = 0, max_a = -1, max_b = -1;
  for (int i = 0; i < map->rows; i++) {
    for (int j = 0; j < map->cols; j++) {
      if (map->grid[i][j] == map->o) cur[j+1] = 0;
      else {
        int v = 1 + min3(prev[j+1], cur[j], prev[j]);
        cur[j+1] = v;
        if (v > max_sz) {
          max_sz = v;
          max_a = i;
          max_b = j;
        }
      }
    }
    int* tmp = prev;
    prev = cur;
    cur = tmp;
  }
  free(prev); free(cur);
  *a = max_a;
  *b = max_b;
  *size = max_sz;
}

// write the square in the map->grid
static void writeSquare(t_bsq* map, int a, int b, int size) {
  if (size <= 0) return ;
  int start_a = a - size + 1;
  int start_b = b - size + 1;
  for (int i = start_a; i <= a; i++) {
    for (int j = start_b; j <= b; j++) {
      map->grid[i][j] = map->f;
    }
  }
}

// prints the map to the stdout until map->grid NULL pointer
static void printMap(t_bsq* map) {
  for (int i = 0; map->grid[i]; i++) {
    fputs(map->grid[i], stdout);
    fputs("\n", stdout);
  }
}
/*
  do the all the heavy lifting
  initializes the struct that stores the map values
  checks the header and its chars and parses the map to the map.grid
  finds the square in the map.grid
  writes the square in the map.grid
  prints the map and free its memory
*/
void processMap(FILE* fp) {
  t_bsq map = {0};
  map.grid = NULL;
  if (!isHeaderValid(&map, fp) || !readMap(fp, &map)) {
    fprintf(stderr, "map error\n");
    cleanMap(map.grid);
    return ;
  }
  int a, b, size = 0;
  findSquare(&map, &a, &b, &size);
  writeSquare(&map, a, b, size);
  printMap(&map);
  cleanMap(map.grid);
}
