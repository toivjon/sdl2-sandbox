// ============================================================================
// SDL2 Sandbox
// ============================================================================
// This application provides a sandbox to play with different kinds of SDL2
// features along with the additional features provided by the SDL2 add-ons.
//
// The base SDL2 framework contains following functionality categories:
// 
//  1. Basics
//  2. Video
//  3. Input
//  4. Force Feedback
//  5. Audio
//  6. Threads
//  7. Timers
//  8. File abstraction
//  9. Shared object support
// 10. Platform and CPU information
// 11. Power Management
// 12. Platforms and standard library functionality
// ============================================================================
#include <SDL.h>

static SDL_atomic_t sAtomicInt;

// ============================================================================
// TIMERS
// ============================================================================
// An example SDL timer callback function.
//
// This function is as a callback function that is called by the SDL framework
// timer functionality on periodic intervals after being registered with the
// SDL_AddTimer function.
//
// Function return value specifies when SDL should call the function for the
// next time. If the function returns 0, then the timer is being cancelled.
// ============================================================================
static Uint32 timer_callback(Uint32 interval, void* param)
{
    SDL_Log("\tSDL called the timer callback function!");
    return interval;
}

// ============================================================================
// THREADS
// ============================================================================
// An example SDL thread function.
// 
// This function is an example of a SDL thread function. Functions of this kind
// will be called when they are used along the SDL_CreateThread function.
//
// Function return value will be passed to issuer via the SDL_WaitThread.
// ============================================================================
static int thread_function(void* data)
{
    SDL_Delay(*static_cast<int*>(data));
    SDL_Log("\tSDL called a thread function on thread %d!", SDL_ThreadID());

    SDL_AtomicAdd(&sAtomicInt, 1);
    return 0;
}

int main(int argc, char* argv[])
{
    // ========================================================================
    // SDL allows configuration variables to be used as configuration hints.
    // They may or may not be supported or applicable on any given platform.
    // However, they can be used as hints to note how the SDL should behave.
    //
    // The full list of hints: https://wiki.libsdl.org/CategoryHints
    //
    // Hints can be either provided with normal or priorited way. Prioritized
    // hints will force the hint to be handled in a desired importance level.
    //
    // Note that hint state changes can also be listened with callbacks.
    // ========================================================================
    auto hintResult = SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_Log("[%d] SDL uses OpenGL\n", hintResult);

    // ========================================================================
    // SDL provides an easy 3-function interface to indicate errors.
    // Errors are also automatically added by the SDL if SDL functions fail.
    // ========================================================================
    SDL_Log("SDL error management testing:\n");
    SDL_Log("\tInitially: %s\n", SDL_GetError());
    SDL_SetError("Custom error message!");
    SDL_Log("\tAfter set: %s\n", SDL_GetError());
    SDL_ClearError();
    SDL_Log("\t  Cleared: %s\n", SDL_GetError());

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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
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

    // ========================================================================
    // SYSTEM INFORMATION
    // ========================================================================
    // SDL is capable to detect various things about the client system.
    //
    // 1. Platform name (Windows, Mac OS X, Linux, iOS or Android).
    // 1. Absolute application path (guaranteed to end with path separator).
    // 2. Preference path for user data (guaranteed to end with path separator).
    // 3. Number of logical CPU cores.
    // 4. CPU L1 cache line size.
    // 5. Amount of RAM.
    // 6. Support for different kinds of CPU features.
    // ========================================================================
    SDL_Log("SDL system information testing:\n");
    auto* basePath = SDL_GetBasePath();
    auto* prefPath = SDL_GetPrefPath("organization_name", "application_name");
    SDL_Log("\tPlatform: %s\n", SDL_GetPlatform());
    SDL_Log("\tBase path: %s\n", (basePath == NULL ? "null" : basePath));
    SDL_Log("\tPref path: %s\n", (prefPath == NULL ? "null" : prefPath));
    SDL_Log("\tLogical CPU cores: %d\n", SDL_GetCPUCount());
    SDL_Log("\tCPU L1 cache line: %d bytes\n", SDL_GetCPUCacheLineSize());
    SDL_Log("\tRAM: %d MB\n", SDL_GetSystemRAM());
    SDL_Log("SDL system information about CPU feature support:");
    SDL_Log("\t[%d] 3DNow\n", SDL_Has3DNow());
    SDL_Log("\t[%d] AVX\n", SDL_HasAVX());
    SDL_Log("\t[%d] AVX2\n", SDL_HasAVX2());
    SDL_Log("\t[%d] AltiVec\n", SDL_HasAltiVec());
    SDL_Log("\t[%d] MMX\n", SDL_HasMMX());
    SDL_Log("\t[%d] RDTSC\n", SDL_HasRDTSC());
    SDL_Log("\t[%d] SSE\n", SDL_HasSSE());
    SDL_Log("\t[%d] SSE2\n", SDL_HasSSE2());
    SDL_Log("\t[%d] SSE3\n", SDL_HasSSE3());
    SDL_Log("\t[%d] SSE41\n", SDL_HasSSE41());
    SDL_Log("\t[%d] SSE42\n", SDL_HasSSE42());
    SDL_free(basePath);
    SDL_free(prefPath);

    // ========================================================================
    // ASSERTIONS
    // ========================================================================
    // SDL contains three (+ disabled) different levels of assertions.
    // 
    // SDL_assert_release....A release level assertion.
    // SDL_assert............A debug level assertion.
    // SDL_assert_paranoid...A trace level assertion.
    //
    // Assertion level is defined by redefining the SDL_ASSERT_LEVEL value.
    //
    // 0...Disables all assertions.
    // 1...Enables SDL_assert_release (default for release).
    // 2...Enables SDL_assert and SDL_assert_release (default for debug).
    // 3...Enables SDL_assert_paranoid, SDL_assert and SDL_assert_release.
    // 
    // Assertions are being tracked by the SDL. While assertions can be ignored
    // by continuing the program execution, we are able to get assertion report
    // from the framework. This report can be used to describe all failures.
    //
    // Note that SDL also allows setting a custom assertion handler if desired.
    // ========================================================================
    SDL_assert_release(true == true);
    SDL_assert(true == true);
    SDL_assert_paranoid(true == true);
    const auto* item = SDL_GetAssertionReport();
    while (item) {
        SDL_Log("%s %s (%s:%d) triggered %u times, always ignore: %s.\n",
                item->condition,
                item->function,
                item->filename,
                item->linenum,
                item->trigger_count,
                item->always_ignore ? "yes" : "no");
        item = item->next;
    }

    // ========================================================================
    // TIMERS
    // ========================================================================
    // SDL contains a support the following timer features.
    //
    // Timer.................Add/remove timer called on a specified interval.
    // Delay.................Make the current thread to wait for some time.
    // Performance Counter...A high resolution timer value and frequency.
    // Ticks.................The number of millis since SDL init.
    // ========================================================================
    SDL_Log("Testing SDL timer features:\n");
    SDL_Log("\tPerformance counter frequency: %u\n", SDL_GetPerformanceFrequency());
    SDL_Log("\tPerforming a small SDL one second delay and using timers.\n");
    SDL_Log("\t%04u --- %u\n", SDL_GetTicks(), SDL_GetPerformanceCounter());
    SDL_Delay(1000);
    SDL_Log("\t%04u --- %u\n", SDL_GetTicks(), SDL_GetPerformanceCounter());

    SDL_Log("\tChecking how the SDL periodic timer works.\n");
    SDL_Log("\tCreating a timer with 500 millisecond interval.\n");
    auto timerId = SDL_AddTimer(500, timer_callback, nullptr);
    if (timerId == 0) {
        SDL_Log("\tUnable to create SDL timer: %s\n", SDL_GetError());
    }
    SDL_Delay(1010);
    if (SDL_RemoveTimer(timerId) == SDL_FALSE) {
        SDL_Log("\tSDL was unable to find a timer with id: %d\n", timerId);
    }
    SDL_Log("\tRemoved the timer.\n");

    // ========================================================================
    // THREADS
    // ========================================================================
    // SDL contains the following inbuilt support for multithreading.
    //
    // 1. Threads
    // 2. Synchronization primitives
    // 3. Atomic operations
    //
    // Thread management contains the following functionality.
    // 
    // 1. Thread creation.
    // 2. Thread waiting.
    // 3. Thread detaching.
    // 4. Thread-local storage.
    // 5. Thread priorities (LOW, NORMAL[default] and HIGH).
    // 
    // Synchronization primitives contain the following structures.
    //
    // 1. Condition variables
    // 2. Mutexes
    // 3. Semaphores
    //
    // Atomic operations support the following 
    // 
    // !!! IMPORTANT NOTE !!!
    // Note that window creation, rendering or event receiving cannot be done
    // in any other thread than within the main thread of the application.
    // ========================================================================
    SDL_Log("Testing SDL threading features:\n");
    auto threadDelay = 2000;
    auto thread1 = SDL_CreateThread(thread_function, "foo-1", &threadDelay);
    auto thread2 = SDL_CreateThread(thread_function, "foo-2", &threadDelay);
    auto thread3 = SDL_CreateThread(thread_function, "foo-3", &threadDelay);

    auto threadReturn = 0;
    SDL_DetachThread(thread1);
    SDL_WaitThread(thread2, &threadReturn);
    SDL_WaitThread(thread3, &threadReturn);
    SDL_Log("\tAll threads have processed their work.\n");
    SDL_Log("\tAtomic integer is now to %d.\n", SDL_AtomicGet(&sAtomicInt));

    // ========================================================================
    // DATA I/O ABSTRACTION
    // ========================================================================
    // SDL has a support for data reading and writing from various sources.
    //
    // 1. Reading from a read-only memory buffer (const void*).
    // 2. Reading and writing with a FILE pointer (not available in Windows!).
    // 3. Reading and writing from a file based on a provided filename.
    // 4. Reading and writing with a memory buffer (void*).
    //
    // All reading and writing does actually use a SDL_RWops structure. It can
    // be used to perform different kinds of basic file operations including.
    //
    // 1. Allocate
    // 2. Free
    // 3. Allocate from source (inc. Allocate).
    // 4. Close (inc. Free).
    // 5. Read
    // 6. Seek
    // 7. Get stream size.
    // 8. Get current stream pointer location.
    // 9. Write
    // 
    // SDL also contains support for reading and writing individual 1,2,4 and 8
    // bytes with a byte-order conversion, which ensures byte order correctness.
    // ========================================================================
    SDL_Log("Testing SDL data I/O abstraction features:\n");
    char buffer[] = "foo";
    SDL_Log("\tbuffer content before write: %s\n", buffer);

    // read buffer into a SDL specific data structure.
    auto data = SDL_RWFromMem(buffer, sizeof(buffer));
    if (data == NULL) {
        SDL_Log("\tFailed to load buffer to structure: %s\n", SDL_GetError());
    } else {
        // append data to buffer.
        const char* addition = "bar";
        auto len = SDL_strlen(addition);
        if (SDL_RWwrite(data, addition, sizeof(char), len) != len) {
            SDL_Log("\tFailed to write buffer data: %s\n", SDL_GetError());
        }
    }

    // close (and free) the SDL specific data structure.
    if (SDL_RWclose(data) != 0) {
        SDL_Log("\tFailed to close the structure buffer: %s\n", SDL_GetError());
    }
    SDL_Log("\tbuffer content after write: %s\n");

    // ========================================================================
    // GRAPHICS CARD MANAGEMENT
    // ========================================================================
    // SDL is capable to query some very basic information about video drivers.
    // 
    // 1. The number of available drivers.
    // 2. A name for each available driver.
    // 3. The name of the currently used video driver. 
    // ========================================================================
    auto numVideoDrivers = SDL_GetNumVideoDrivers();
    
    SDL_Log("Testing SDL graphics card features:\n");
    SDL_Log("\tNumber of video drivers: %d\n", numVideoDrivers);
    for (auto i = 0; i < numVideoDrivers; i++) {
        SDL_Log("\t\t[%d] driver: %s\n", i, SDL_GetVideoDriver(i));
    }
    SDL_Log("\tCurrent video driver: %s\n", SDL_GetCurrentVideoDriver());

    // ========================================================================
    // DISPLAY MANAGEMENT
    // ========================================================================
    // SDL is capable to query some abstract information about the displays.
    //
    // 1. The number of available displays.
    // 2. The name for each available display.
    // 3. Diagonal, horizontal and vertical dots-per-inch (DPI).
    // 4. The currently active display mode.
    // 5. The currently active OS desktop display mode.
    // 6. Enumeration of all display modes for a display.
    // 7. System and usable boundaries for each display.
    // 8. Finding a closest matching display mode for a provided mode.
    // ========================================================================
    auto numVideoDisplays = SDL_GetNumVideoDisplays();

    SDL_Log("Testing SDL display features:\n");
    SDL_Log("\tNumber of displays: %d\n", numVideoDisplays);
    for (auto i = 0; i < numVideoDisplays; i++) {
        SDL_Log("\tDisplay [%d] information:\n", i);
        SDL_Log("\t\tName: %s\n", SDL_GetDisplayName(i));

        auto ddpi = 0.f;
        auto hdpi = 0.f;
        auto vdpi = 0.f;
        if (SDL_GetDisplayDPI(i, &ddpi, &hdpi, &vdpi) == 0) {
            SDL_Log("\t\tDisplay v-dpi: %.2f h-dpi: %.2f d-dpi: %.2f",
                vdpi,
                hdpi,
                ddpi);
        }

        SDL_DisplayMode mode;
        if (SDL_GetCurrentDisplayMode(i, &mode) == 0) {
            SDL_Log("\t\tCurrent mode: %d bpp %dx%d %dhz",
                SDL_BITSPERPIXEL(mode.format),
                mode.w,
                mode.h,
                mode.refresh_rate);
        }

        if (SDL_GetDesktopDisplayMode(i, &mode) == 0) {
            SDL_Log("\t\tDesktop mode: %d bpp %dx%d %dhz",
                SDL_BITSPERPIXEL(mode.format),
                mode.w,
                mode.h,
                mode.refresh_rate);
        }

        SDL_Rect rect;
        if (SDL_GetDisplayBounds(i, &rect) == 0) {
            SDL_Log("\t\tDisplay bounds position: %dx%d size: %dx%d",
                rect.x, rect.y,
                rect.w, rect.h);
        }

        if (SDL_GetDisplayUsableBounds(i, &rect) == 0) {
            SDL_Log("\t\tUsable display bounds position: %dx%d size: %dx%d",
                rect.x, rect.y,
                rect.w, rect.h);
        }

        auto numDisplayModes = SDL_GetNumDisplayModes(i);
        SDL_Log("\t\tNumber of display modes: %d\n", numDisplayModes);
        if (numDisplayModes >= 1) {
            for (auto j = 0; j < numDisplayModes; j++) {
                SDL_GetDisplayMode(i, j, &mode);
                SDL_Log("\t\t\t%d bpp %dx%d %dhz",
                    SDL_BITSPERPIXEL(mode.format),
                    mode.w,
                    mode.h,
                    mode.refresh_rate);
            }
        }

    }

    // ========================================================================
    // WINDOW MANAGEMENT
    // ========================================================================
    // SDL uses an own abstraction layout on top of the traditional OS window
    // handles. The framework provides three functions to create new windows.
    // 
    // SDL_CreateWindow..............Builds a SDL window.
    // SDL_CreateWindowAndRenderer...Builds a SDL window and default renderer.
    // SDL_CreateWindowFrom..........Builds a SDL window from a native window.
    //
    // SDL allows the usage of window construction flags when building a window
    // from scratch (not from native window). Here is a list of those flags.
    //
    // SDL_WINDOW_FULLSCREEN...........Fullscreen window.
    // SDL_WINDOW_FULLSCREEN_DESKTOP...Fullscreen window with desktop resolution.
    // SDL_WINDOW_OPENGL...............OpenGL context supported window.
    // SDL_WINDOW_HIDDEN...............Window which is not visible.
    // SDL_WINDOW_BORDERLESS...........Window without decorations.
    // SDL_WINDOW_RESIZABLE............Window that can be resized.
    // SDL_WINDOW_MINIMIZED............Window that is minimized.
    // SDL_WINDOW_MAXIMIZED............Window that is maximized.
    // SDL_WINDOW_INPUT_GRABBED........Window that has grabbed input focus.
    // SDL_WINDOW_ALLOW_HIGHDPI........Window with high-DPI mode (if supported)
    //
    // There are also some additional window flags that can be used when doing
    // a query for the current state of the SDL window. See the following list.
    //
    // SDL_WINDOW_SHOWN...........Window is visible.
    // SDL_WINDOW_INPUT_FOCUS.....Window has focus.
    // SDL_WINDOW_MOUSE_FOCUS.....Window has mouse focus.
    // SDL_WINDOW_FOREIGN.........Window is not created by SDL.
    // SDL_WINDOW_MOUSE_CAPTURE...Window has mouse captured.
    // SDL_WINDOW_ALWAYS_ON_TOP...[X11] Window is always top on others.
    // SDL_WINDOW_SKIP_TASKBAR....[X11] Window is not in taskbar.
    // SDL_WINDOW_UTILITY.........[X11] Window is an utility window.
    // SDL_WINDOW_TOOLTIP.........[X11] Window is a tooltip.
    // SDL_WINDOW_POPUP_MENU......[X11] Window is a popup menu.
    //
    // In addition to previously mentioned window flags, SDL provides a way to
    // define and query some following special management for each SDL window.
    //
    // * Visibility
    // * Borders
    // * Window owner display brightness (gamma).
    // * An arbitary named pointer with window.
    // * A fullscreen window display mode.
    // * A window fullscreen mode (real / desktop / none).
    // * The gamma ramp for the window owner display.
    // * Grap input to target window.
    // * Callbacks to define window special properties.
    // * Window icon from a SDL_Surface.
    // * Input focus state of a window.
    // * The maximum size of the window.
    // * The minimum size for the window.
    // * The parent window for a window to act modal.
    // * The opacity of the window (directFB, X11, Cocoa, Windows).
    // * The position for the window.
    // * Whether a user is able to resize the window.
    // * The size of the window client area.
    // * The title of the window.
    // * Set window on top of other windows.
    // * Get the numeric ID of the window (for logging purposes).
    // * Get the index of the parent display of the window.
    // * An ability to update fully/partially the window surface on the screen.
    // 
    // SDL also contains a way to toggle the screensaver state for the duration
    // of the application execution and also provides a way to show small info
    // message boxes that can contain informative messages for the users.
    // ========================================================================
    SDL_Log("Testing SDL window management features:\n");
    
    // construct a new SDL window with the name, position, size and flags.
    auto window = SDL_CreateWindow("foo", 50, 50, 800, 600, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        SDL_Log("Failed to create a new SDL window: %s\n", SDL_GetError());
    } else {
        SDL_SetWindowResizable(window, SDL_TRUE);
        SDL_SetWindowMaximumSize(window, 1024, 768);
        SDL_SetWindowMinimumSize(window, 640, 480);
        SDL_SetWindowTitle(window, "foobar");
        SDL_RaiseWindow(window);
    }

    // enable to show a super simple message box to user.
    // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Foo!", "Bar!", window);

    // ========================================================================
    // RECTANGLES AND POINTS
    // ========================================================================
    // SDL contains a support for two kinds of geometric primitives.
    //
    // 1. SDL_Point......A two-dimensional point used also to define a size.
    // 2. SDL_Rect.......A two-dimensional definition of a rectangle.
    //
    // SDL has a in-built support for some minor functions that can be used to 
    // perform some basic tasks. See the following list of functions.
    //
    // 1. Calculate the minimumal rectangle that encloses the set of points.
    // 2. Check whether two rectangles intersect.
    // 3. Calculate the intersection rectangle between two intersecting rects.
    // 4. Calculate the intersection point between a line and a rectangle.
    // 5. Check whether the given point resides inside a rectangle.
    // 6. Check whether the given rectangle has not area.
    // 7. Equality of two rectangles.
    // 8. Union of two rectangles.
    // ========================================================================
    SDL_Log("Testing SDL rect features:\n");
    SDL_Rect rect1 = {100, 200, 300, 400};
    SDL_Rect rect2 = {200, 100, 300, 400};
    SDL_Log("\rect1: x=%d y=%d w=%d h=%d\n", rect1.x, rect1.y, rect1.w, rect1.h);
    SDL_Log("\rect2: x=%d y=%d w=%d h=%d\n", rect2.x, rect2.y, rect2.w, rect2.h);
    SDL_Rect rect3;
    SDL_UnionRect(&rect1, &rect2, &rect3);
    SDL_Log("\t\tunion: x=%d y=%d w=%d h=%d\n", rect3.x, rect3.y, rect3.w, rect3.h);

    // ========================================================================
    // EVENTS
    // ========================================================================
    // SDL uses an event queue to store and distribute events. This system is
    // being initialized when the framework init contains SDL_INIT_EVENTS type.
    // (NOTE: The SDL_INIT_EVENTS can be implied by defining other subsystems.)
    // 
    // SDL events are built as unions where the structure is following:
    // --- type
    // --- union-structure that contains type specific substructure.
    //
    // The full list of types: https://wiki.libsdl.org/SDL_EventType
    //
    // Each event type is related to a single strcture within the event e.g.
    // on event SDL_KEYDOWN the details are located in the event.key structure.
    //
    // Note that SDL can be set to ignore (i.e. disable) unwanted event types.
    //
    // Events can be also filtered/handled from the queue with custom filters.
    // ========================================================================
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);

    auto running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
        // ...
    }

    // ========================================================================
    // Shut down all SDL subsystems.
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}