# Micro SD SPI
With this code, you can read, write, delete, change text files on a micro SD card. I will show you an example how to get started.

# How to use
- 1. Begin to implement fatfs_sd.c and fatfs_sd.h where you can access fatfs_sd.h 
- 2. Enable UART and SPI and FATFS. Notice that you cannot have so fast SPI here! Maximum 2.25 Mb/s for SD cards.
- 3. Copy over user_diskio.c from this GitHub repository to the user_diskio.c inside your STM32 IDE.
- 4. Copy over the program example to your main.c file

You can follow the pictures.
UART config:

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/Micro%20SD%20SPI/UART.png)

SPI config:

![a](https://github.com/DanielMartensson/STM32-Libraries/blob/master/Micro%20SD%20SPI/SPI.png)

FATFS config:

![a](https://github.com/DanielMartensson/STM32-Libraries/blob/master/Micro%20SD%20SPI/FATFS.png)

User_diskio.c and fatfs_sd.h and fatfs.c location:

![a](https://github.com/DanielMartensson/STM32-Libraries/blob/master/Micro%20SD%20SPI/User_diskio.png)

Program example:
```
// Include
#include "main.h"
#include "fatfs.h"
#include "Micro SD SPI/fatfs_sd.h"
#include "string.h"
#include "stdio.h"

// Structures
SPI_HandleTypeDef hspi2;
UART_HandleTypeDef huart2;

// Variables
FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];  // to store strings..

int i = 0;

int bufsize(char *buf) {
	int i = 0;
	while (*buf++ != '\0')
		i++;
	return i;
}

void clear_buffer(void) {
	for (int i = 0; i < BUFFER_SIZE; i++)
		buffer[i] = '\0';
}

void send_uart(char *string) {
	uint8_t len = strlen(string);
	HAL_UART_Transmit(&huart2, (uint8_t*) string, len, HAL_MAX_DELAY); // transmit in blocking mode
}

int main() {

	SD_init_handler(&hspi2, SD_CS_GPIO_Port, SD_CS_Pin);
	fresult = f_mount(&fs, "/", 1);
	if (fresult != FR_OK)
		send_uart("ERROR!!! in mounting SD CARD...\n\n");
	else
		send_uart("SD CARD mounted successfully...\n\n");

	/*************** Card capacity details ********************/

	/* Check free space */
	f_getfree("", &fre_clust, &pfs);

	total = (uint32_t) ((pfs->n_fatent - 2) * pfs->csize * 0.5);
	sprintf(buffer, "SD CARD Total Size: \t%lu\n", total);
	send_uart(buffer);
	clear_buffer();
	free_space = (uint32_t) (fre_clust * pfs->csize * 0.5);
	sprintf(buffer, "SD CARD Free Space: \t%lu\n\n", free_space);
	send_uart(buffer);
	clear_buffer();

	/************* The following operation is using PUTS and GETS *********************/

	/* Open file to write/ create a file if it doesn't exist */
	fresult = f_open(&fil, "file1.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

	/* Writing text */
	f_puts("This data is from the FILE1.txt. And it was written using ...f_puts... ", &fil);

	/* Close file */
	fresult = f_close(&fil);

	if (fresult == FR_OK)
		send_uart("File1.txt created and the data is written \n");

	/* Open file to read */
	fresult = f_open(&fil, "file1.txt", FA_READ);

	/* Read string from the file */
	f_gets(buffer, f_size(&fil), &fil);

	send_uart("File1.txt is opened and it contains the data as shown below\n");
	send_uart(buffer);
	send_uart("\n\n");

	/* Close file */
	f_close(&fil);

	clear_buffer();

	/**************** The following operation is using f_write and f_read **************************/

	/* Create second file with read write access and open it */
	fresult = f_open(&fil, "file2.txt", FA_CREATE_ALWAYS | FA_WRITE);

	/* Writing text */
	strcpy(buffer, "This is File2.txt, written using ...f_write... and it says Hello from Controllerstech\n");
	fresult = f_write(&fil, buffer, bufsize(buffer), &bw);
	send_uart("File2.txt created and data is written\n");

	/* Close file */
	f_close(&fil);

	// clearing buffer to show that result obtained is from the file
	clear_buffer();

	/* Open second file to read */
	fresult = f_open(&fil, "file2.txt", FA_READ);
	if (fresult == FR_OK)
		send_uart("file2.txt is open and the data is shown below\n");

	/* Read data from the file
	 * Please see the function details for the arguments */
	f_read(&fil, buffer, f_size(&fil), &br);
	send_uart(buffer);
	send_uart("\n\n");

	/* Close file */
	f_close(&fil);

	clear_buffer();

	/*********************UPDATING an existing file ***************************/

	/* Open the file with write access */
	fresult = f_open(&fil, "file2.txt", FA_OPEN_EXISTING | FA_READ | FA_WRITE);

	/* Move to offset to the end of the file */
	fresult = f_lseek(&fil, f_size(&fil));

	if (fresult == FR_OK)
		send_uart("About to update the file2.txt\n");

	/* write the string to the file */
	fresult = f_puts("This is updated data and it should be in the end", &fil);

	f_close(&fil);

	clear_buffer();

	/* Open to read the file */
	fresult = f_open(&fil, "file2.txt", FA_READ);

	/* Read string from the file */
	fresult = f_read(&fil, buffer, f_size(&fil), &br);
	if (fresult == FR_OK)
		send_uart("Below is the data from updated file2.txt\n");
	send_uart(buffer);
	send_uart("\n\n");

	/* Close file */
	f_close(&fil);

	clear_buffer();

	/*************************REMOVING FILES FROM THE DIRECTORY ****************************/

	fresult = f_unlink("/file1.txt");
	if (fresult == FR_OK) send_uart("file1.txt removed successfully...\n");

	fresult = f_unlink("/file2.txt");
	if (fresult == FR_OK) send_uart("file2.txt removed successfully...\n");

	/* Unmount SDCARD */
	fresult = f_mount(NULL, "/", 1);
	if (fresult == FR_OK) send_uart ("SD CARD UNMOUNTED successfully...\n");

}
```
