#include <SDL.h>

int main(int argc, char* argv[])
{
    // ========================================================================
    // Initialize the SDL along with desired subsystems.
    // SDL_INIT_TIMER............Include SDL timer support.
    // SDL_INIT_AUDIO............Include SDL audio (???) support.
    // SDL_INIT_VIDEO............Include SDL video/graphics support.
    // SDL_INIT_JOYSTICK.........Include SDL joystick support.
    // SDL_INIT_HAPTIC...........Include SDL haptic (force feedback) support.
    // SDL_INIT_GAMECONTROLLER...Include SDL game controller support.
    // SDL_INIT_EVENTS...........Include SDL event system.
    // SDL_INIT_EVERYTHING.......Include all SDL subsystems.
    // 
    // Some subsystems also automatically imply other subsystems.
    // SDL_INIT_GAMECONTROLLER...Implies SDL_INIT_JOYSTICK.
    // SDL_INIT_VIDEO............Implies SDL_INIT_EVENTS.
    // SDL_INIT_JOYSTICK.........Implies SDL_INIT_EVENTS.
    //
    // Definitions can be OR'd together (i.e. SDL_INIT_TIMER | SDL_INIT_AUDIO)
    // ========================================================================
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

    // ========================================================================
    // SDL offers a way to check which SDL subsystems has been initialized.
    // Uses the same macros than what are used with SDL_Init (see above).
    // 
    // Definitions can be OR'd together (i.e. SDL_INIT_TIMER | SDL_INIT_AUDIO)
    // ========================================================================
    SDL_Log("Initialized SDL subsystems:\n");
    SDL_Log("[%d] Timer\n", SDL_WasInit(SDL_INIT_TIMER) != 0);
    SDL_Log("[%d] Audio\n", SDL_WasInit(SDL_INIT_AUDIO) != 0);
    SDL_Log("[%d] Video\n", SDL_WasInit(SDL_INIT_VIDEO) != 0);
    SDL_Log("[%d] Joystick\n", SDL_WasInit(SDL_INIT_JOYSTICK) != 0);
    SDL_Log("[%d] Haptic\n", SDL_WasInit(SDL_INIT_HAPTIC) != 0);
    SDL_Log("[%d] Game controller\n", SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 0);
    SDL_Log("[%d] Events\n", SDL_WasInit(SDL_INIT_EVENTS) != 0);

    // ...

    // ========================================================================
    // Shut down all SDL subsystems.
    SDL_Quit();
    
    return 0;
}