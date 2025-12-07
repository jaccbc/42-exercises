/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <joandre-@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:35:04 by joandre-          #+#    #+#             */
/*   Updated: 2025/12/07 18:54:11 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsq.h"

int main(int argc, char** argv) {
  if (argc == 1) processMap(stdin);
  else {
    for (int i = 1; i < argc; i++) {
      FILE* fp = fopen(argv[i], "r");
      if (!fp) { fprintf(stderr, "map error\n"); continue; }
      processMap(fp); fclose(fp);
      if (i != argc-1) fputs("\n", stdout);
    }
  }
  return 0;
}
