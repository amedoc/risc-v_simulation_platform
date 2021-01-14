//@Module : RRAM
//@Author : Ahmed MAHMOUDI
//@Project : KI-PRO
//@Date   : 04/01/2021

#ifndef RRAM_RRAM_H_
#define RRAM_RRAM_H_


#include "systemc.h"
#include <unistd.h>

using namespace std;




#define DATA_WIDTH        16    // Data space in a cell
#define ADDR_WIDTH        8     // 2^8 = 256 --> rram can store 256  cells == 256*16 == 4096 bits
                                // data space in the chip = 4 kbits

//SC_MODULE(rram)
struct rram : public sc_core::sc_module
{
  sc_in<bool>   clk;						// Clock input of the design
  sc_in<bool>   rst; 						// Active high, synchronous reset input
  sc_in<bool>   Read_Write;					// enable Read if high, enable write if low
  sc_in<bool>   mul_enable;					// Active high, enable multiplication operation

  sc_in<uint16_t>    data_in;	// 16 bit array, input of a single memory cell data
  sc_in<uint8_t>     addr;		// 8 bit array, input of the cell address
  sc_out<uint16_t>   data_out;	// 16 bit array, output of a single memory cell data


  //-----------Internal variables-------------------
  uint16_t rram_data [256];		// matrix[256][16], inner data of the RRAM

  //-----------Methods------------------------------
  void  Write_data ();						// Write/Enter operation of data in one cell
  void  read_data();						// Read/Extract operation of data from one cell
  void  reset_data ();						// Reset the internal data of the RRAM

  uint32_t multiply_two_cells(uint16_t,uint16_t );
    	  	  	  	  	  	  	  	  	  	// Multiplication algorithm via shifting and addition

  void  multiply_data();					// Multiply operation of each adjacent two cells of the RRAM



  //-----------Constructor--------------------------
  SC_CTOR(rram) {
      SC_METHOD (Write_data);  		// register the Write_data as a process with the simulation kernel
      sensitive << clk.pos();
      sensitive << Read_Write << rst;

      SC_METHOD (read_data);		// register the read_data as a process with the simulation kernel
      sensitive << Read_Write;
      sensitive << clk.pos();

      SC_METHOD (multiply_data);	// register the multiply_data as a process with the simulation kernel
      sensitive << mul_enable;

      SC_METHOD (reset_data);	// register the multiply_data as a process with the simulation kernel
      sensitive << rst;


  }




};


#endif /* RRAM_RRAM_H_ */
