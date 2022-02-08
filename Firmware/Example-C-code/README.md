#STM32CubeIDE Firmware Code

This is a set of files generated and edited using the STM32CubeIDE sotfware. The custom firmware code can be found in the "main.c" files located in "Core/Src". Other alterations that were necessary are the keyboard descriptors in "HID" class
files that can be found in the "Middlewares" folder. The other change done to the generated code was "USBD_PID_FS" value in "usbd_desc.c". It was changed from 22315 to 22316 as per the STM USB training video instructions. It's not clear that the computer uses this value though.
