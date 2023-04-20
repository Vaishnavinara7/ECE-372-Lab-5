#ifndef SPI_H
#define SPI_H

void SPI_MASTER_Init();
void read_execute(unsigned char CMD1, unsigned char data1);
void write_execute(unsigned char CMD0, unsigned char data0);

#endif
