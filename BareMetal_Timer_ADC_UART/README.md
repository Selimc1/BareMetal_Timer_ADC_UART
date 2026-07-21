# BareMetal Timer ADC UART

![STM32](https://img.shields.io/badge/Microcontroller-STM32F407VG-blue)
![Architecture](https://img.shields.io/badge/Architecture-Bare%20Metal-orange)

## 📌 Project Overview
A sophisticated hardware synchronization project designed to eliminate software blocking (delays). It combines hardware Timers (TIM), Analog-to-Digital Conversion (ADC), and Universal Asynchronous Receiver-Transmitter (UART) to create a highly deterministic, real-time measurement system without utilizing any HAL or LL abstraction libraries. 

## 🚀 Key Features
- **Hardware Timer (TIM3):** Configured to generate exact 1-millisecond resolution interrupts, acting as a highly precise system stopwatch.
- **Non-Blocking Execution:** Eliminates `delay()` functions entirely. Time-critical events (such as setting a GPIO pin high at exactly 500ms) are executed in the background via the Timer interrupt.
- **Continuous ADC Streaming:** While the timer manages critical timing in the background, the main loop handles a continuous, "waterfall-like" transmission of ADC data to a PC via UART.
- **Pure Register Access:** Total control over the STM32's RCC, TIM, ADC, and USART registers.

## 🛠️ Hardware Requirements
- **STM32F407G-DISC1** Development Board.
- **Analog Sensor / Potentiometer** (Connected to the configured ADC pin).
- **USB-to-TTL Serial Converter** (Connected to USART pins).

## ⚙️ How It Works
1. **Timer Setup:** TIM3 is configured with a specific Prescaler and Auto-Reload Register (ARR) value to overflow exactly every 1 ms, generating an interrupt.
2. **Interrupt Service:** Inside `TIM3_IRQHandler`, a millisecond counter increments. If it hits exactly 500, it toggles a specific GPIO output (PA5).
3. **Main Process:** Concurrently, the ADC continuously samples analog voltage. The value is immediately written to the USART Data Register (DR), ensuring a massive throughput of data to the monitoring terminal without interrupting the TIM3 logic.

## 💻 Getting Started
Open the project in **STM32CubeIDE**, build, and flash via ST-LINK. Connect a logic analyzer or oscilloscope to PA5 to verify the exact 500ms timing, and open a serial terminal to view the ADC data stream.
