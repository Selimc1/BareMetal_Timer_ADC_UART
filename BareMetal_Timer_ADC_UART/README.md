# BareMetal Timer ADC UART

This project is a hardware-based (bare-metal) application that uses a Timer (TIM3) to execute tasks requiring precise timing via periodic interrupts. It synchronizes signal generation and ADC reading operations.

**Hardware & Features:**
- Hardware interrupts and stopwatch functionality with 1 ms resolution using TIM3
- Output of a high signal (step) on PA5 exactly at the 500th millisecond without relying on blocking delays
- Continuous, waterfall-like transmission of ADC readings to a PC via UART
