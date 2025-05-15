# Car Sound System

A feature-rich **Car Sound System** project built using embedded components to provide a high-quality and interactive in-car audio experience. The system integrates a **Mini MP3 player** to play songs from an SD card, displays song information and progress on a **TFT display**, and includes fallback manual controls along with advanced features like **FOTA (Firmware Over-the-Air)**, **status display via 7-segment**, and **interactive DAC-driven audio feedback**.

## 📦 Project Overview

This project emulates a full-fledged car audio entertainment system with both visual and physical interaction capabilities. It is designed with embedded hardware and provides flexibility through multiple control methods and feedback channels.

## 🎵 Features

- **MP3 Playback via SD Card**  
  Plays music files stored on an SD card using a Mini MP3 module.

- **TFT Display Integration**  
  Shows song metadata, playback status (e.g., progress bar, current track), and system settings in real time.

- **Firmware Over-the-Air (FOTA)**  
  Update the system firmware wirelessly without needing to access the physical hardware.

- **Status Indication on 7-Segment Display**  
  Displays current system status or error codes for quick diagnostics and feedback.

- **Interactive Sound Effects via DAC**  
  Uses a DAC module to generate system sounds like clicks, notifications, or warnings for enhanced user interaction.

- **Infrared (IR) Remote Control**  
  Allows wireless control of music playback and system settings.

- **Manual Button Controls**  
  Physical buttons provide an alternative control method in case the IR remote is unavailable or non-functional.

- **Configurable Settings**  
  Adjust volume, change tracks, toggle modes, and more via remote or manual input.

## 🛠️ Hardware Components

- Mini MP3 Player Module (with SD card support)
- TFT Display Module
- 7-Segment Display
- DAC Module
- IR Receiver and Remote Control
- Push Buttons for manual controls
- Microcontroller (e.g., STM32, Arduino, or similar)
- Supporting power supply and connectors

## 📈 System Flow

1. On startup, the system initializes all hardware modules.
2. The MP3 player begins playing tracks from the SD card.
3. Song details are displayed on the TFT screen.
4. User input is accepted via IR remote or manual buttons.
5. Status codes (e.g., playback errors or system states) are shown on the 7-segment display.
6. System audio cues are played using the DAC.
7. Firmware updates can be performed via the FOTA interface.

## 🔧 Controls

| Control Type     | Function                          |
|------------------|-----------------------------------|
| IR Remote        | Play/Pause, Next/Previous, Volume |
| Manual Buttons   | Same functions as IR (fallback)   |
| TFT Interface    | View information and feedback     |

## 🚀 Getting Started

1. Upload firmware to your microcontroller.
2. Load your MP3 files onto an SD card and insert it into the MP3 module.
3. Connect hardware as per circuit diagram.
4. Power on and enjoy!
