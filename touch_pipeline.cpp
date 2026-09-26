// =============================================================================
// xOS HARDWARE LAYER - Dynamic Touch Screen Register Translation Engine
// =============================================================================

#include <stdint.h>

// Physical I/O memory address registers for standard mobile touch chips
#define REG_TOUCH_STATUS   ((volatile uint32_t*)0x40051000) // 0 = Idle, 1 = Pressed
#define REG_TOUCH_RAW_X    ((volatile int32_t*)0x40051004)  // Live finger horizontal pixel
#define REG_TOUCH_RAW_Y    ((volatile int32_t*)0x40051008)  // Live finger vertical pixel

// Spatial tracking state preservation registers
static int32_t last_registered_y_pixel = 0;
static int32_t active_delta_y_movement = 0;
static bool touch_was_previously_engaged = false;

// Global flag to clear movement loops safely
static float INTERNALS_KINETIC_FRICTION = 0.88f;

// 1. TOUCHSCREEN REFRESH CORE PIPELINE
// -----------------------------------------------------------------------------
// This function must be fired continuously by your kernel clock interrupt loop
extern "C" void poll_hardware_touchscreen_registers() {
    uint32_t current_touch_status = *REG_TOUCH_STATUS;
    int32_t current_raw_y = *REG_TOUCH_RAW_Y;

    // Condition A: The very microsecond a finger makes contact with the glass matrix
    if (current_touch_status == 1 && !touch_was_previously_engaged) {
        last_registered_y_pixel = current_raw_y;
        active_delta_y_movement = 0;
        touch_was_previously_engaged = true;
    }
    // Condition B: The finger is actively sliding across the screen matrix rows
    else if (current_touch_status == 1 && touch_was_previously_engaged) {
        // Calculate the exact travel vector distance
        active_delta_y_movement = current_raw_y - last_registered_y_pixel;
        
        // Lock the current pixel location as the new calculation anchor point
        last_registered_y_pixel = current_raw_y;
    }
    // Condition C: The finger lifts up from the glass surface layer
    else {
        // Apply kinetic friction decay so the screen rolls smoothly to a stop
        if (active_delta_y_movement != 0) {
            active_delta_y_movement = (int32_t)(active_delta_y_movement * INTERNALS_KINETIC_FRICTION);
            if (active_delta_y_movement < 2 && active_delta_y_movement > -2) {
                active_delta_y_movement = 0; // Complete rest state anchor
            }
        }
        touch_was_previously_engaged = false;
    }
}

// 2. EXPORT FUNCTIONS FOR KOTLIN VECTOR LINKING
// -----------------------------------------------------------------------------
// Your AppMarketUI.kt code calls this float hook to slide the infinite scroll view
extern "C" float get_hardware_scroll_delta_y() {
    return (float)active_delta_y_movement;
}

extern "C" bool is_finger_touching_screen() {
    return touch_was_previously_engaged;
}
