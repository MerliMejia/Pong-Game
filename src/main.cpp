//
//  main.cpp
//
//  Created by Merli Mejia on 2/9/22.
//

#include <iostream>
#include "Game/Game.hpp"

int main(int argc, const char * argv[]) {
    
    Game game;
    bool success = game.Initialize();
    
    if(success){
        game.RunLoop();
    }
    
    game.Shutdown();
    
    return 0;
}
