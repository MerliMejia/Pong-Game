//
//  Game.hpp
//
//  Created by Merli Mejia on 2/9/22.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include "SDL2/SDL.h"

class Game{
public:
    bool Initialize();
    void RunLoop();
    void Shutdown();
    Game();
    
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    bool mIsRunning;
};

#endif /* Game_hpp */
