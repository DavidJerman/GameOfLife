#define OLC_PGE_APPLICATION
#include "game.h"

int main()
{
    game demo;
    if (demo.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, PIXEL_WIDTH, PIXEL_HEIGHT, false, true))
        demo.Start();

    return 0;
}