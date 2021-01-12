#ifndef RRAM_RRAM_H_
#define RRAM_RRAM_H_


#include "systemc.h"



#define DATA_WIDTH        16    // Data space in a cell
#define ADDR_WIDTH        8     // 2^8 = 256 --> rram can store 256  cells == 256*16 == 4096 bits
                                // data space in the chip = 4 kbits

SC_MODULE(rram)
{
  sc_in<bool>   clk;
  sc_in<bool>   rst;
  sc_in<bool>   write;
  sc_in<bool>   mul_enable;

  sc_in<sc_uint<DATA_WIDTH>>    data_in;
  sc_in<sc_uint<ADDR_WIDTH >>    addr;
  sc_out<sc_uint<DATA_WIDTH>>    data_out;


  //-----------Internal variables-------------------
  sc_bv<DATA_WIDTH> rram_data [256];

  //-----------Methods------------------------------
  void  Write_data ();
  void read_data();
  void  reset_data ();
  void  multiply_data();
  sc_uint<2*DATA_WIDTH> multiply_two_cells(sc_uint<DATA_WIDTH>,sc_uint<DATA_WIDTH> );

  //-----------Constructor--------------------------
  SC_CTOR(rram) {
      SC_METHOD (Write_data);
      sensitive << clk.pos();
  }




};


#endif /* RRAM_RRAM_H_ */
