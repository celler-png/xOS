struct PeekGlassPanel { int x_pos, y_pos, width, height; unsigned char frosted_alpha_tint; };
struct AppIcon { const char* app_name; int grid_slot; unsigned char color_hex; bool has_secure_handshake; };

unsigned char current_glass_transparency = 0x7F;

void render_peek_glass_layer(PeekGlassPanel panel) {
    volatile unsigned char* video_memory = (volatile unsigned char*)0xB8000;
    const int screen_width = 80;
    const int screen_height = 25;
    if (panel.width <= 0 || panel.height <= 0) {
        return;
    }

    const long long requested_x_end = (long long)panel.x_pos + panel.width;
    const long long requested_y_end = (long long)panel.y_pos + panel.height;
    const int x_begin = panel.x_pos < 0 ? 0 : panel.x_pos;
    const int y_begin = panel.y_pos < 0 ? 0 : panel.y_pos;
    const int x_end = requested_x_end > screen_width
        ? screen_width : (int)requested_x_end;
    const int y_end = requested_y_end > screen_height
        ? screen_height : (int)requested_y_end;

    if (x_begin >= x_end || y_begin >= y_end) {
        return;
    }

    for (int y = y_begin; y < y_end; ++y)
        for (int x = x_begin; x < x_end; ++x) {
            const int attribute_offset = (y * screen_width + x) * 2 + 1;
            const unsigned char existing_attribute = video_memory[attribute_offset];
            const unsigned char glass_background = panel.frosted_alpha_tint & 0x70;
            video_memory[attribute_offset] =
                (existing_attribute & 0x0F) | glass_background;
        }
}

void render_home_screen() {
    volatile unsigned char* video_memory = (volatile unsigned char*)0xB8000;
    AppIcon store_app = {"Store", 1, 0x0B, true};
    PeekGlassPanel home_screen_glass = {0, 0, 80, 25, current_glass_transparency};
    render_peek_glass_layer(home_screen_glass);

    int store_offset = (12 * 80 + 38) * 2;
    video_memory[store_offset] = 'S';
    video_memory[store_offset + 1] = store_app.color_hex;
}

// Interactive function to let the system change the glass depth layer dynamically
void adjust_peek_glass_transparency(int choice_level) {
    if (choice_level == 1) {
        current_glass_transparency = 0x2F;
    } else if (choice_level == 2) {
        current_glass_transparency = 0x7F;
    } else if (choice_level == 3) {
        current_glass_transparency = 0xBF;
    }
}

// =============================================================
// xOS MOBILE INTERFACE PIPELINE - Render Home Gesture Line
// =============================================================

// Draw the sleek swipe-up indicator line right at the bottom row
void draw_home_gesture_bar_line() {
    char* video_memory = (char*)0xB8000;
    
    // Position the bar at row 24 (the very bottom edge of an 80x25 text grid)
    int row_start_offset = 24 * 80 * 2; 
    
    // Centered horizontal line spanning from column 30 to column 50
    for (int col = 30; col < 50; col++) {
        int offset = row_start_offset + (col * 2);
        
        video_memory[offset] = '_';     // Draw a clean horizontal line character
        video_memory[offset + 1] = 0x0F; // Bright white visibility color tag
    }
}




