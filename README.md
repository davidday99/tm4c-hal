## About 
A WIP HAL for the TM4C, using a Makefile, startup.c, and linker script from https://github.com/shawn-dsilva/tm4c-linux-template and some assembly from https://github.com/rrbutani/tm4c-llvm-toolchain.
Thanks to Shawn and Rahul, respectively.

## Design
Typical HAL features along with some device drivers that use the HAL.

## Todo
### ENC28J60
- Handle DMA transfers greater than 1024 bytes (ethernet frame could be up to 1518 bytes).
- Implement DMA transfer for transmitting frames. Currently the driver only supports DMA transfer of received frames. Transmitting requires transferring to buffer in the main thread.
- Implement ring transmit buffer. Currently a static array is used for transmitting data. Data is tranferred from the host to the NIC, transmission begins. At the beginning of the transmit function the host polls a status bit until transmission of a previous frame has completed. 
- Implement routines to handle ENC28J60 link status change.