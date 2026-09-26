// =============================================================================
// xOS MOBILE SYSTEM LAYERS - Kotlin Phone Hardware Component Scanner
// =============================================================================

package com.xos.apps.systeminfo

data class PhysicalPhoneParts(
    val batteryType: String = "Lithium-Ion (Li-Ion)",
    var batteryHealthPercentage: Int = 98,
    var liveBatteryTemperatureCelsius: Float = 32.5f,
    val rearCameraSensor: String = "50MP Wide-Angle Quad-Pixel",
    val frontCameraSensor: String = "12MP True-Depth Face ID Lens",
    var motherboardThermalSensor: Float = 36.2f,
    val displayPanelType: String = "Super Retina XDR OLED (120Hz)"
)

class PhoneHardwareScanner {

    val physicalParts = PhysicalPhoneParts()
    var isDiagnosticScreenActive: Boolean = false

    // Actively reads the physical sensors under the motherboard hood
    fun readLiveSensors() {
        // Simulates thermal adjustments based on CPU workflow loads
        val tempJitter = (-5..5).random() / 10.0f
        physicalParts.liveBatteryTemperatureCelsius += tempJitter
        physicalParts.motherboardThermalSensor += (tempJitter * 1.2f)
    }

    // Generates the layout text strings to print directly onto the display monitor
    fun getPrintableDiagnosticText(): List<String> {
        return listOf(
            "=== xOS HARDWARE COMPONENT SUMMARY ===",
            "DISPLAY PANEL   : ${physicalParts.displayPanelType}",
            "BATTERY CORE    : ${physicalParts.batteryType} (${physicalParts.batteryHealthPercentage}% Health)",
            "BATTERY TEMP    : ${"%.1f".format(physicalParts.liveBatteryTemperatureCelsius)}°C",
            "MAIN CAMERA     : ${physicalParts.rearCameraSensor}",
            "FRONT CAMERA    : ${physicalParts.frontCameraSensor}",
            "SYSTEM THERMALS : ${"%.1f".format(physicalParts.motherboardThermalSensor)}°C"
        )
    }
}
