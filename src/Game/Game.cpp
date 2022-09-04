//
//  Game.cpp
//
//  Created by Merli Mejia on 2/9/22.
//

#include "iostream"
#include "Game.hpp"

const int THICKNESS = 15;
const int WIDTH = 1024;
const int HEIGHT = 768;
const float PADDLE_VEL = 300.0f;
const int PADDLE_HEIGHT = 100.0f;

Uint32 mThicksCount;

struct Vector2{
    float x;
    float y;
};

Vector2 mPAddlePos;
int mPaddleDir = 0;

Vector2 mBallPos;
Vector2 mBallVell;
Vector2 mBallDir;

Game::Game()
:mWindow(nullptr)
,mIsRunning(true)
{
    
    mBallPos.x = WIDTH / 2;
    mBallPos.y = HEIGHT / 2;
    
    mPAddlePos.x = 50;
    mPAddlePos.y = (HEIGHT - PADDLE_HEIGHT) / 2;
    
    mBallVell.x = 400.0f;
    mBallVell.y = 400.0f;
    
    mBallDir.x = 1;
    mBallDir.y = -1;
    
}

bool Game::Initialize(){
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if(sdlResult != 0){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    
    mWindow = SDL_CreateWindow("Game Loop", 100, 100, WIDTH, HEIGHT, 0);
    
    if(!mWindow){
        SDL_Log("Failed to create a window: %s", SDL_GetError());
        return false;
    }
    
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    
    if(!mRenderer){
        SDL_Log("Failed to create the renderer: %s", SDL_GetError());
        return false;
    }
    return true;
}

void Game::Shutdown(){
    mIsRunning = false;
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::RunLoop(){
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
                
            default:
                break;
        }
    }
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]){
        mIsRunning = false;
    }
    
    mPaddleDir = 0;
    
    if(state[SDL_SCANCODE_W]){
        mPaddleDir -= 1;
    }
    
    if(state[SDL_SCANCODE_S]){
        mPaddleDir += 1;
    }
}

void Game::UpdateGame(){
    //Game time
    
    // Wait until 16ms has elapsed since last frame.
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mThicksCount + 16));
    
    float deltaTime = (SDL_GetTicks() - mThicksCount) / 1000.0f;
    mThicksCount = SDL_GetTicks();
    
    if(deltaTime > 0.5f){
        deltaTime = 0.5f;
    }
    
    //Paddle movements.
    if(mPaddleDir != 0){
        mPAddlePos.y += mPaddleDir * PADDLE_VEL * deltaTime;
        
        if(mPAddlePos.y < (PADDLE_HEIGHT / 2) - (THICKNESS * 2)){
            mPAddlePos.y = (PADDLE_HEIGHT / 2) - (THICKNESS * 2);
        }else if(mPAddlePos. y > (HEIGHT) - PADDLE_HEIGHT){
            mPAddlePos.y = (HEIGHT) - PADDLE_HEIGHT;
        }
    }
    
    //Basic collision system.
    
    //Top wall
    if(mBallDir.y < 0){
        float ballY = mBallPos.y;
        float wallY = THICKNESS;
        
        float distance = ballY - wallY;
        if(distance <= 0){
            mBallDir.y = 1;
        }
    }

    //Bottom wall.
    if(mBallDir.y > 0){
        float ballY = mBallPos.y;
        float wallY = HEIGHT - THICKNESS;

        float distance = wallY - ballY;
        if(distance <= 0){
            mBallDir.y = -1;
        }
    }

    //Right wall.
    if(mBallDir.x > 0){
        float ballX = mBallPos.x;
        float wallX = WIDTH - THICKNESS;
        
        float distance = wallX - ballX;
        if(distance <= 0){
            mBallDir.x = -1;
        }
    }
    
    //Left wall and Paddle.
    if(mBallDir.x < 0){
        float ballX = mBallPos.x;
        float ballY = mBallPos.y;
        float wallX = THICKNESS;
        float paddleX = mPAddlePos.x + THICKNESS / 2;
        float paddleY = mPAddlePos.y + (PADDLE_HEIGHT / 2);
        
        float wallDistanceX = ballX - wallX;
        float paddleXDistance = ballX - paddleX;
        float paddleYDistance = paddleY - ballY;
        
        if(paddleXDistance <= 0 && paddleYDistance <= (PADDLE_HEIGHT / 2) && paddleYDistance >= -(PADDLE_HEIGHT / 2)){
            mBallDir.x = 1;
        }
        
        if(wallDistanceX <= 0){
            Shutdown();
        }
    }
    
    //Ball movements.
    mBallPos.x += (mBallVell.x * mBallDir.x) * deltaTime;
    mBallPos.y += (mBallVell.y * mBallDir.y) * deltaTime;
    
    mBallVell.x += 0.1f;
    mBallVell.y += 0.1f;
}

void Game::GenerateOutput(){
    
    // Set draw color to blue
    SDL_SetRenderDrawColor(
        mRenderer,
        0,        // R
        0,        // G
        255,    // B
        255        // A
    );
    
    // Clear back buffer
    SDL_RenderClear(mRenderer);

    // Walls color.
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    
    SDL_Rect topWall{
        0,            // Top left x
        0,            // Top left y
        WIDTH,        // Width
        THICKNESS    // Height
    };
    SDL_RenderFillRect(mRenderer, &topWall);
    
    SDL_Rect bottomWall{
        0,            // Top left x
        HEIGHT - THICKNESS,           // Top left y
        WIDTH,        // Width
        THICKNESS    // Height
    };
    
    SDL_RenderFillRect(mRenderer, &bottomWall);
    
    //Left wall will bi red.w
    SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
    
    SDL_Rect leftWall{
        0,            // Top left x
        0,           // Top left y
        THICKNESS,        // Width
        HEIGHT    // Height
    };
    
    SDL_RenderFillRect(mRenderer, &leftWall);
    
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    
    SDL_Rect rightWall{
        WIDTH - THICKNESS,            // Top left x
        0,           // Top left y
        THICKNESS,        // Width
        HEIGHT      // Height
    };
    
    SDL_RenderFillRect(mRenderer, &rightWall);
    
    SDL_Rect ball{
        static_cast<int>(mBallPos.x - THICKNESS/2),
        static_cast<int>(mBallPos.y - THICKNESS/2),
        THICKNESS,
        THICKNESS
    };
    
    SDL_RenderFillRect(mRenderer, &ball);
    
    SDL_Rect paddle{
        static_cast<int>(mPAddlePos.x - THICKNESS / 2),
        static_cast<int>(mPAddlePos.y - THICKNESS / 2),
        THICKNESS,
        PADDLE_HEIGHT
    };
    
    SDL_RenderFillRect(mRenderer, &paddle);
    
    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
    return;
}
