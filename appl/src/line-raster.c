//#include <SDL.h>
#include <stdlib.h>
#include "screen.h"

void dda_line_raster(screen_t *screen, int x1, int y1, int x2, int y2, color_t color)
{
    // points distance
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);

    // slope [coefficiente angolare della retta passante per i 2 punti]
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int steps = (dx > dy) ? dx : dy;

    // !!! Il cast NON ha costo !!!
    // increment x, y for each step with relative slope
    float incr_x = (float)dx * (float)sx / (float)steps;
    float incr_y = (float)dy * (float)sy / (float)steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; ++i)
    {
        screen_put_pixel(screen, x, y, 0.f, color);
        x += incr_x;
        y += incr_y;
    }
}
