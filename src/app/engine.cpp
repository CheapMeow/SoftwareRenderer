#include "engine.h"

//Dummy constructors and destructors
Engine::Engine(){}
Engine::~Engine(){}

//Starts up subsystems in an order that satifies their dependencies
bool Engine::startUp(){
    bool success = true;
    //Start up of all SDL related content
    if( !gDisplayManager.startUp() ){
        success = false;
        printf("Failed to initialize window manager.\n");
    }
    else{
        //Loads default scene
        if( !gSceneManager.startUp() ){
            success = false;
            printf("Failed to initialize scene manager.\n");
        }
        else{
            //Initializes renderer and connects it to the window manager
            //Also gets pointer to current scene
            if( !gRenderManager.startUp(gDisplayManager, gSceneManager) ){
            success = false;
            printf("Failed to initialize render manager.\n");
            
            }
            else{
                if ( !gInputManager.startUp(gSceneManager) ){
                    success = false;
                    printf("Failed to initialize input manager.\n");
                }
            }
        }
    }
    return success;
}

//Closing in opposite order to avoid dangling pointers
void Engine::shutDown(){
    gInputManager.shutDown();
    printf("Closed input manager.\n");

    gRenderManager.shutDown();
    printf("Closed renderer manager.\n");
    
    gSceneManager.shutDown();
    printf("Closed Scene manager.\n");
    
    gDisplayManager.shutDown();
    printf("Closed display manager.\n");
}

//Runs main application loop
void Engine::run(){

    //Main flags
    bool done = false;

    //Iteration and time keeping counters
    int count = 0;
    unsigned int deltaT = 0;
    unsigned int start = 0;;
    unsigned int total = 0;

    printf("Entered Main Loop!\n");
    while(!done){
        ++count;
        start = SDL_GetTicks();

        //Handle all user input
        //Switches scene too
        gInputManager.processInput(done, deltaT);
        
        //Update all models, camera and lighting in the current scene
        gSceneManager.update(deltaT);

        //Update Rendering Queue and draw each item 
        gRenderManager.render();

        //Stats about frame
        deltaT = SDL_GetTicks() - start;
        printf("%2.1d: Loop elapsed time (ms):%d\n",count, deltaT);
        total += deltaT;
        if(count == 500) break;
    }

    printf("Closing down engine.\n");
    printf("Average frame time over %2.1d frames:%2.fms.\n", count, total/(float)count);
    
}
