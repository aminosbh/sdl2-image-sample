/*
 * Copyright (c) 2018, 2019 Amine Ben Hassouna <amine.benhassouna@gmail.com>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any
 * person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the
 * Software without restriction, including without
 * limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 * SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>

// Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Define screen dimensions
#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

// Define Tunisia flag image path
#define TN_FLAG_IMAGE_PATH "assets/tn.png"

int main(int argc, char* argv[])
{
    // Unused argc, argv
    (void) argc;
    (void) argv;

    // Initialize SDL2
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL2 could not be initialized!\n"
               "SDL2 Error: %s\n", SDL_GetError());
        return 0;
    }

    // Initialize SDL2_image
    int flags = IMG_INIT_PNG; // Can be: IMG_INIT_JPG | IMG_INIT_PNG
    if((IMG_Init(flags) & flags) != flags) {
        printf("SDL2_image could not be initialized with PNG support!\n"
               "SDL2_image Error: %s\n", IMG_GetError());
        return 0;
    }

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
    // Disable compositor bypass
    if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
    {
        printf("SDL can not disable compositor bypass!\n");
        return 0;
    }
#endif

    // Create window
    SDL_Window *window = SDL_CreateWindow("SDL2_image sample",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if(!window)
    {
        printf("Window could not be created!\n"
               "SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        // Create renderer
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(!renderer)
        {
            printf("Renderer could not be created!\n"
                   "SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            // Load image from file
            SDL_Texture *flag = IMG_LoadTexture(renderer, TN_FLAG_IMAGE_PATH);
            if(!flag)
            {
                printf("Unable to load image '%s'!\n"
                       "SDL_image Error: %s", TN_FLAG_IMAGE_PATH, IMG_GetError());
                return false;
            }

            // Get dimensions
            int w, h;
            SDL_QueryTexture(flag, NULL, NULL, &w, &h);

            // Flag dimensions/position
            SDL_Rect flagRect;

            // Scale the flag dimensions to fit the screen
            if(w > h) {
                flagRect.w = SCREEN_WIDTH * 0.6;
                flagRect.h = h * flagRect.w / w;
            } else {
                flagRect.h = SCREEN_HEIGHT * 0.6;
                flagRect.w = w * flagRect.h / h;
            }

            // Flag position: In the middle of the screen
            flagRect.x = SCREEN_WIDTH / 2 - flagRect.w / 2;
            flagRect.y = SCREEN_HEIGHT / 2 - flagRect.h / 2;

            // Set the border size
            const int flagBorderSize = 5;

            // Declare rect of square
            SDL_Rect squareRect;

            // Square dimensions
            squareRect.w = flagRect.w + flagBorderSize * 2;
            squareRect.h = flagRect.h + flagBorderSize * 2;

            // Square position:
            squareRect.x = flagRect.x - flagBorderSize;
            squareRect.y = flagRect.y - flagBorderSize;


            // Event loop exit flag
            bool quit = false;

            // Event loop
            while(!quit)
            {
                SDL_Event e;

                // Wait indefinitely for the next available event
                SDL_WaitEvent(&e);

                // User requests quit
                if(e.type == SDL_QUIT)
                {
                    quit = true;
                }

                // Initialize renderer color white for the background
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Clear screen
                SDL_RenderClear(renderer);

                // Set renderer color black to draw the square
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

                // Draw filled square
                SDL_RenderFillRect(renderer, &squareRect);

                // Draw the tunisian flag
                SDL_RenderCopy(renderer, flag, NULL, &flagRect);

                // Update screen
                SDL_RenderPresent(renderer);
            }

            // Destroy renderer
            SDL_DestroyRenderer(renderer);
        }

        // Destroy window
        SDL_DestroyWindow(window);
    }

    // Quit SDL_image
    IMG_Quit();

    // Quit SDL
    SDL_Quit();

    return 0;
}
