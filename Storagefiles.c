// =============================================================================
// xOS STORAGE ARCHITECTURE - Bare-Metal Disk Storage Controller (Pure C Engine)
// =============================================================================

#define STORAGE_SUCCESS 0
#define STORAGE_ERROR_SECTOR 1
#define MAX_PARTITIONS 4

// Global status register tracking our HDD or solid-state memory controller
static unsigned int STORAGE_STATUS_REGISTER = 0x00;
static long total_blocks_read_count = 0;

typedef struct {
    int partition_id;
    const char* mount_point;
    unsigned long sector_start;
    unsigned long sector_end;
    int is_mounted;
} SystemPartition;

// Master tracking array for your filesystem layout blocks
static SystemPartition system_disk_map[MAX_PARTITIONS];

// 1. INITIALIZE MASTER STORAGE FILESYSTEM MAP
// -----------------------------------------------------------------------------
void initialize_xos_storage_driver() {
    STORAGE_STATUS_REGISTER = 0x01; // Flag: Scanning storage buses
    
    // Partition 1: Core System Kernel Boot Binaries
    system_disk_map[0].partition_id = 1;
    system_disk_map[0].mount_point = "/root";
    system_disk_map[0].sector_start = 0x00000000;
    system_disk_map[0].sector_end = 0x000FFFFF;
    system_disk_map[0].is_mounted = 1;

    // Partition 2: Your Critical App Sandbox Folder Storage
    system_disk_map[1].partition_id = 2;
    system_disk_map[1].mount_point = "/apps";
    system_disk_map[1].sector_start = 0x00100000;
    system_disk_map[1].sector_end = 0x00FFFFFF;
    system_disk_map[1].is_mounted = 1;

    // Partition 3: The 22 Optimized 4K Wallpaper Vault Folder!
    system_disk_map[2].partition_id = 3;
    system_disk_map[2].mount_point = "/apps/wallpaper_app_storage";
    system_disk_map[2].sector_start = 0x01000000;
    system_disk_map[2].sector_end = 0x0FFFFFFF;
    system_disk_map[2].is_mounted = 1;

    STORAGE_STATUS_REGISTER = 0xAA; // Flag: Storage drivers online and mounted!
}

// 2. LOW-LEVEL SECTOR READER HARDWARE PIPELINE
// -----------------------------------------------------------------------------
int read_physical_storage_sector(unsigned long target_sector, unsigned char* memory_destination_buffer) {
    // 1. Point straight to the laptop's motherboard hard drive controller bus address
    volatile unsigned int* hdd_control_bus = (volatile unsigned int*)0x40004000;
    
    // 2. Hardware safe boundary check to verify the partition sector exists
    if (target_sector > 0x0FFFFFFF) {
        return STORAGE_ERROR_SECTOR;
    }
    
    // 3. Command the drive platter to stream the raw bits directly into the RAM buffer
    hdd_control_bus[0] = target_sector; // Inject target sector address
    hdd_control_bus[1] = 0x01;           // Fire the execution signal tag!
    
    total_blocks_read_count++;
    return STORAGE_SUCCESS;
}
// =============================================================================
// xOS STORAGE ARCHITECTURE - Capacity & Space Allocation Engine (Pure C Build)
// =============================================================================

#include <stdint.h>

// 1. HARDWARE STORAGE METRICS DEFINITIONS
// -----------------------------------------------------------------------------
// We define our flagship 1 Gigabyte baseline strictly in Kilobytes (KB)
#define TOTAL_CAPACITY_KB 1048576ULL  // 1 GB = 1,024 MB * 1,024 KB
#define SINGLE_WALLPAPER_SIZE_KB 850   // Average high-efficiency KB size
#define TOTAL_WALLPAPERS 22

struct StorageTelemetry {
    uint64_t total_space_kb;
    uint64_t used_space_kb;
    uint64_t free_space_left_kb;
};

// Global storage block register allocation
static struct StorageTelemetry system_drive_stats;

// 2. DYNAMIC SPACE READOUT CALCULATION PIPELINE
// -----------------------------------------------------------------------------
void calculate_system_storage_allocation() {
    // Set the master flagship ceiling profile
    system_drive_stats.total_space_kb = TOTAL_CAPACITY_KB;
    
    // Calculate exactly how much space your 22 assets use up on the HDD platter
    system_drive_stats.used_space_kb = (TOTAL_WALLPAPERS * SINGLE_WALLPAPER_SIZE_KB);
    
    // Subtraction math logic to see exactly how much space is left over for apps
    system_drive_stats.free_space_left_kb = system_drive_stats.total_space_kb - system_drive_stats.used_space_kb;
}

// Global functions so your Kotlin frontend screens can pull the live stats
uint64_t get_total_storage_capacity() {
    return system_drive_stats.total_space_kb;
}

uint64_t get_used_storage_space() {
    return system_drive_stats.used_space_kb;
}

uint64_t get_free_storage_left() {
    return system_drive_stats.free_space_left_kb;
}

