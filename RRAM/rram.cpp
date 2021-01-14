//@Module : RRAM
//@Author : Ahmed MAHMOUDI
//@Project : KI-PRO
//@Date   : 04/01/2021

#include "rram.h"


void rram::Write_data()
{
  if(Read_Write && !rst)
      rram_data[addr.read()]=data_in;
}


void rram::reset_data()
{
	uint16_t null_array = 0x0000;
  if(rst){
      for(int i=0;i<256;i++)
      {
          rram_data[i] = null_array;
      }
  }
}


// m bit multiplicand * n bit multiplier == (m+n) bit product
// accomplished via shifting and addition
uint32_t rram::multiply_two_cells(uint16_t cell_1 ,uint16_t cell_2)
{

	uint32_t result= 0x00000000;
	result = cell_1*cell_2;
/*
    while( cell_2)
    {
        if(cell_2 & 1)
        {
        	result = result + cell_1;
        }
        cell_1 = cell_1 << 1;
        cell_2 = cell_2 >> 1;
    }*/

    return result;
}

void  rram::multiply_data()
{
    // we will use a temporary array to store the multiplication result then slice it to store it in the  adjacent cells
    if (mul_enable)
    {
        for(int i=0;i<255;i=i+2)
        {
        	uint32_t tmp = 0x00000000 ;
            tmp = multiply_two_cells(rram_data[i],rram_data[i+1]);
            rram_data[i] = tmp & 0xFFFF;
            rram_data[i+1] = tmp >> 16;
        }
    }


}


void rram::read_data()
{
	if(Read_Write == false)
    data_out = rram_data[addr.read()];
}

