#include "xil_printf.h"
#include "xparameters.h"
#include "xuartps.h"

#define DDR_BASE_ADDR   0x00100000   // Update with your target memory base address
#define ROWS            10  // Number of rows in the 2D array
#define COLS            10  // Number of columns in the 2D array

// Function to print binary representation of a byte
void PrintBinary(u8 data) {
    for (int i = 7; i >= 0; i--) {
        xil_printf("%d", (data >> i) & 1);

    }
}

int main() {
    u8 (*TxBufferPtr)[ROWS][COLS] = (u8 (*)[ROWS][COLS])DDR_BASE_ADDR; // Pointer to a 2D array
    u8 read_data;  // Variable to hold the read data
    int RecvChar;  // Variable for received character
    XUartPs Uart_Ps;  // UART instance

    xil_printf("Starting simple data write and read test...\n");

    // Initialize UART
    XUartPs_Config *Config;
    Config = XUartPs_LookupConfig(XPAR_XUARTPS_0_DEVICE_ID);
    if (!Config) {
        xil_printf("Error: UART configuration not found.\n");
        return -1;
    }
    if (XUartPs_CfgInitialize(&Uart_Ps, Config, Config->BaseAddress) != XST_SUCCESS) {
        xil_printf("Error: UART initialization failed.\n");
        return -1;
    }
    XUartPs_SetBaudRate(&Uart_Ps, 9600);

    // Write data to memory
    xil_printf("Waiting for data from UART to write to memory...\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            // Wait for data to arrive
            while (!XUartPs_IsReceiveData(Uart_Ps.Config.BaseAddress));
            (*TxBufferPtr)[i][j] = XUartPs_RecvByte(Uart_Ps.Config.BaseAddress);
        }
    }
    xil_printf("Data written successfully. Now waiting for keyboard input...\n");

    // Wait for user input
    while (1) {
        // Wait for user input
        while (!XUartPs_IsReceiveData(Uart_Ps.Config.BaseAddress));
        RecvChar = XUartPs_RecvByte(Uart_Ps.Config.BaseAddress);
        xil_printf("You pressed: %c\r\n", RecvChar);

        // If '1' is received, print memory content
        if (RecvChar == '1') {
            xil_printf("Displaying memory content:\n");
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    read_data = (*TxBufferPtr)[i][j];
                    xil_printf("Address: [0x%08X], ", (u32)&((*TxBufferPtr)[i][j]));
                    xil_printf("Binary: ");
                    PrintBinary(read_data);
                    xil_printf(" (integer: %d)\n", read_data);

                }
            }
        }
        // Exit loop on 'q'
        if (RecvChar == 'q') {
            xil_printf("Exiting program...\n");
            break;
        }
    }

    xil_printf("Program finished successfully.\n");
    return 0;
}
