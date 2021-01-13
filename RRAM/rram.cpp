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
  sc_uint<DATA_WIDTH> null_array = 0x0000;
  if(rst){
      for(int i=0;i<256;i++)
      {
          rram_data[i] = null_array;
      }
  }
}


// m bit multiplicand * n bit multiplier == (m+n) bit product
// accomplished via shifting and addition
sc_uint<2*DATA_WIDTH> rram::multiply_two_cells(sc_uint<DATA_WIDTH> cell_1 ,sc_uint<DATA_WIDTH> cell_2)
{

    sc_uint<DATA_WIDTH*2> result= 0x00000000;

    while( cell_2 != 0)
    {
        if(cell_2 & 1)
        {
        	result = result + cell_1;
        }
        cell_1 <<= 1;
        cell_2 >>= 1;
    }

    return result;
}

void  rram::multiply_data()
{
    // we will use a temporary array to store the multiplication result then slice it to store it in the  adjacent cells
    if (mul_enable)
    {
        for(int i=0;i<255;i=i+2)
        {
            sc_uint<DATA_WIDTH*2> tmp = 0x00000000 ;
            tmp = multiply_two_cells(rram_data[i],rram_data[i+1]);
            rram_data[i] = tmp.range(15,0);
            rram_data[i+1] = tmp.range(31,16);
        }
    }


}


void rram::read_data()
{
	if(Read_Write == false)
    data_out = rram_data[addr.read()];
}

