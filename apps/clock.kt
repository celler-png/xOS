// =============================================================================
// xOS MOBILE PLATFORM - Vector Clock & Audio Alarm Core (apps/clock.kt)
// =============================================================================

package com.xos.apps.clock

import java.time.LocalTime
import kotlin.math.max

class AlarmAudioController {
    private val AUDIO_HARDWARE_SPEAKER_REG = 0x40005000
    var isAlarmSoundBlasting: Boolean = false

    fun triggerLoudHardwareAlarmPulse() {
        isAlarmSoundBlasting = true
        val audioToneFrequencyHz = 880 // Loud, piercing high-pitched warning beep
        
        println("[xOS HARDWARE AUDIO] !! BREAKING ALARM TRIGGER !!")
        println("[xOS HARDWARE AUDIO] Blasting raw speaker pulse channel at $audioToneFrequencyHz Hz!")
    }

    fun silenceHardwareAlarm() {
        isAlarmSoundBlasting = false
        println("[xOS HARDWARE AUDIO] Alarm dismissed. Speaker channel cleared to silence.")
    }
}

data class HardcodedAlarm(
    val alarmId: Int,
    var targetHour: Int,
    var targetMinute: Int,
    val textLabel: String,
    var isToggledOn: Boolean = true
)

class ClockAppEngine {

    val activeAlarms = mutableListOf<HardcodedAlarm>()
    val audioController = AlarmAudioController()
    var formattedTimeText: String = "12:00:00 AM"
    var isClockAppOpen: Boolean = false
    var activeFiredAlarmLabel: String = ""

    var setupHourWheelValue = 7
    var setupMinuteWheelValue = 30
    var scrollMomentumAccumulator = 0.0f

    val backgroundCreamHex = 0xFFFFFDD0
    val activeAlarmPurpleHex = 0xFF8A2BE2
    val primaryTextSlateHex = 0xFF1C1C1E
    val cardBackgroundWhiteHex = 0xFFFFFFFF
    val alarmBlastingRedHex = 0xFFFF3B30
    val toggledOffGrayHex = 0xFFD1D1D6

    init {
        // Load default premium alarms directly into memory blocks
        activeAlarms.add(HardcodedAlarm(1, 7, 0, "Wake Up Sunrise 🌅"))
        activeAlarms.add(HardcodedAlarm(2, 15, 30, "xOS System Sprints 🐯")) // 3:30 PM target
    }

    // 🕒 ACTIVE SYSTEM TIME COMPARISON ENGINE
    fun processClockTickTelemetry() {
        val currentTime = LocalTime.now()
        val displayHour = if (currentTime.hour % 12 == 0) 12 else currentTime.hour % 12
        val amPmMarker = if (currentTime.hour >= 12) "PM" else "AM"
        
        formattedTimeText = String.format("%02d:%02d:%02d %s", displayHour, currentTime.minute, currentTime.second, amPmMarker)

        // RUN DEEP MATRIX COMPARE: Check if the exact Hour and Minute match an enabled alarm!
        activeAlarms.forEach { alarm ->
            if (alarm.isToggledOn && 
                currentTime.hour == alarm.targetHour && 
                currentTime.minute == alarm.targetMinute && 
                currentTime.second == 0) { 
                
                activeFiredAlarmLabel = alarm.textLabel
                audioController.triggerLoudHardwareAlarmPulse()
            }
        }
    }

    // 🎨 VECTOR GRAPHICS CANVAS DRAW ENGINE PIPELINE
    fun drawClockInterfaceToCanvas(canvas: XosCanvas, widthPixels: Int, heightPixels: Int): List<String> {
        canvas.drawingCommandPayload.clear()

        val viewportLeft = widthPixels * 0.04f
        val viewportTop = heightPixels * 0.06f
        val viewportWidth = widthPixels * 0.92f
        val viewportHeight = heightPixels * 0.88f
        val viewportRight = viewportLeft + viewportWidth
        val viewportBottom = viewportTop + viewportHeight

        canvas.setPixelFillsColor(primaryTextSlateHex)
        canvas.drawRoundRect(0f, 0f, widthPixels.toFloat(), heightPixels.toFloat(), 0f, 0f)
        
        val dynamicCardColor = if (audioController.isAlarmSoundBlasting) alarmBlastingRedHex else backgroundCreamHex
        canvas.setPixelFillsColor(dynamicCardColor)
        canvas.drawRoundRect(viewportLeft, viewportTop, viewportRight, viewportBottom, 48f, 48f)

        val clockCenterX = viewportLeft + (viewportWidth / 2.0f)
        val clockCenterY = viewportTop + 140.0f
        canvas.setPixelFillsColor(primaryTextSlateHex)
        canvas.drawVectorText(formattedTimeText, clockCenterX - 180.0f, clockCenterY, 56.0f)

        if (audioController.isAlarmSoundBlasting) {
            canvas.setPixelFillsColor(cardBackgroundWhiteHex)
            canvas.drawRoundRect(viewportLeft + 40f, clockCenterY + 40f, viewportRight - 40f, clockCenterY + 160f, 24f, 24f)
            canvas.setPixelFillsColor(primaryTextSlateHex)
            canvas.drawVectorText("🚨 !! WAKE UP !! 🚨", clockCenterX - 150f, clockCenterY + 90f, 28f)
            canvas.drawVectorText(activeFiredAlarmLabel, clockCenterX - 120f, clockCenterY + 130f, 20f)
        } else {
            var alarmRowTopY = clockCenterY + 90.0f
            val cardWidth = viewportWidth - 80.0f
            val cardHeight = 96.0f

            activeAlarms.forEach { alarm ->
                val currentCardColor = if (alarm.isToggledOn) activeAlarmPurpleHex else toggledOffGrayHex
                val currentTextColor = if (alarm.isToggledOn) cardBackgroundWhiteHex else primaryTextSlateHex
                
                canvas.setPixelFillsColor(currentCardColor)
                canvas.drawRoundRect(viewportLeft + 40.0f, alarmRowTopY, viewportLeft + 40.0f + cardWidth, alarmRowTopY + cardHeight, 24f, 24f)
                
                canvas.setPixelFillsColor(currentTextColor)
                val displayAlarmHour = if (alarm.targetHour % 12 == 0) 12 else alarm.targetHour % 12
                val displayAmPm = if (alarm.targetHour >= 12) "PM" else "AM"
                val timeString = String.format("%02d:%02d %s", displayAlarmHour, alarm.targetMinute, displayAmPm)
                
                canvas.drawVectorText("⏰ $timeString - ${alarm.textLabel}", viewportLeft + 70.0f, alarmRowTopY + 56.0f, 22.0f)
                alarmRowTopY += cardHeight + 24.0f
            }
        }

        val systemLineY = viewportBottom - 32.0f
        canvas.setPixelFillsColor(primaryTextSlateHex)
        canvas.drawSmoothLine(viewportLeft + 200.0f, systemLineY, viewportRight - 200.0f, systemLineY, 6.0f)

        return canvas.drawingCommandPayload
    }

    fun feedKineticTouchVelocityDelta(deltaY: Float) {
        scrollMomentumAccumulator += deltaY
        if (scrollMomentumAccumulator > 40.0f) {
            setupHourWheelValue = if (setupHourWheelValue >= 12) 1 else setupHourWheelValue + 1
            scrollMomentumAccumulator = 0.0f
        } else if (scrollMomentumAccumulator < -40.0f) {
            setupHourWheelValue = if (setupHourWheelValue <= 1) 12 else setupHourWheelValue - 1
            scrollMomentumAccumulator = 0.0f
        }
    }
}

// Global vector helper context hooks
class XosCanvas {
    val drawingCommandPayload = mutableListOf<String>()
    fun setPixelFillsColor(colorHex: Long) { drawingCommandPayload.add("COLOR: $colorHex") }
    fun drawRoundRect(l: Float, t: Float, r: Float, b: Float, rx: Float, ry: Float) {}
    fun drawVectorText(txt: String, x: Float, y: Float, sz: Float) {}
    fun drawSmoothLine(sx: Float, sy: Float, ex: Float, ey: Float, th: Float) {}
}
