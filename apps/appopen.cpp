
// =============================================================================
// xOS MOBILE INTERFACE CORE - Image-Based App Engine & Swipe-Up Home Gesture
// =============================================================================

// Structure to track actual icon images loaded from the system storage
struct ApplicationImage {
    unsigned int* pixel_data_pointer; // Points directly to the icon pixel array in memory
    int image_width;                  // Width of your custom aesthetic icon file
    int image_height;                 // Height of your custom aesthetic icon file
};

// Isolated individual folder container that wraps around every single application
struct AppContainerFolder {
    const char* app_name;             // Name of the specific app (e.g., "Roblox", "Store")
    ApplicationImage app_icon;        // The unique aesthetic icon image file for this app
    bool is_currently_running;        // Monitors if the app window is actively executing on screen
    
    // Systems operation controls (Pointers to the exact machine tasks)
    void (*open_execution_routine)();  // Code logic that runs the second the icon is touched
    void (*close_execution_routine)(); // Code logic that cleans up memory and closes the app window
};

// Define the dimensions of our visual home swipe line at the bottom
struct HomeGestureBar {
    int screen_y_trigger;  // The row coordinate where the bar sits (bottom edge)
    int bar_width_pixels;  // Horizontal size of the gesture line
    bool is_being_dragged; // Tracks if the user's hand is actively pulling it up
};

// Global system control instances
HomeGestureBar system_home_bar = {240, 200, false}; // Positioned at the base of the display screen

// Master configuration to switch individual apps between running and closed states
void request_app_state_change(AppContainerFolder &target_app, bool open_request) {
    if (open_request) {
        target_app.is_currently_running = true;
        target_app.open_execution_routine();   // Instantly fire up the app's internal screen layout!
    } else {
        target_app.is_currently_running = false;
        target_app.close_execution_routine();  // Instantly terminate and free up the system RAM!
    }
}

// Track the touchscreen path coordinates to catch a full pull-up motion to close apps
void process_bottom_swipe_gesture(int start_y, int end_y, AppContainerFolder &active_app) {
    // Verify if the drag motion started right on the bottom home bar line trigger space
    if (start_y >= system_home_bar.screen_y_trigger) {
        // Measure if the pull distance was moved upwards significantly by the finger
        int pull_distance = start_y - end_y; 
        if (pull_distance > 40) { 
            system_home_bar.is_being_dragged = false;
            request_app_state_change(active_app, false); // Gracefully pull the user back to the home grid!
        }
    }
}
