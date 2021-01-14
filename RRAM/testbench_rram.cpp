//@Module : RRAM
//@Author : Ahmed MAHMOUDI
//@Project : KI-PRO
//@Date   : 04/01/2021

#include "systemc.h"

#include"rram.h"




void next_cycle (sc_signal<bool> &signal_clk);


int sc_main( int argc, char* argv[])
{
	  int i=0;

	  sc_signal<bool>   clk;						// Clock input of the design
	  sc_signal<bool>   rst; 						// Active high, synchronous reset input
	  sc_signal<bool>   Read_Write;						// Active high, enable write operation
	  sc_signal<bool>   mul_enable;					// Active high, enable multiplication operation

	  sc_signal<uint16_t>    data_in;	// 16 bit array, input of a single memory cell data
	  sc_signal<uint8_t>   addr;		// 8 bit array, input of the cell address
	  sc_signal<uint16_t>   data_out;	// 16 bit array, output of a single memory cell data


	  sc_trace_file* my_trace_file;
	  my_trace_file = sc_create_vcd_trace_file ("rram_trace");

	  sc_trace(my_trace_file, clk, "clk");
	  sc_trace(my_trace_file, rst, "reset");
	  sc_trace(my_trace_file, addr, "addr");
	  sc_trace(my_trace_file, data_in, "data_in");
	  sc_trace(my_trace_file, data_out, "data_out");
	  sc_trace(my_trace_file, Read_Write, "Read_Write");
	  sc_trace(my_trace_file, mul_enable, "mul_enable");

	  rram rram_test("RRAM");
	  rram_test.clk(clk);
	  rram_test.rst(rst);
	  rram_test.addr(addr);
	  rram_test.data_in(data_in);
	  rram_test.data_out(data_out);
	  rram_test.Read_Write(Read_Write);
	  rram_test.mul_enable(mul_enable);

	  rst=1;
	  next_cycle(clk);

	  Read_Write = 1; // write operation
	  rst=0;		  // reset not enabled

	  addr=0x00 ;  data_in=0x0001;
	  for (i=0;i<5;i++)
		  {
			  next_cycle(clk);
		  }

	  addr=0x01 ; data_in=0x0003;
	  for (i=0;i<5;i++)
		  {
			  next_cycle(clk);
		  }

	  addr=0x04 ; data_in=0x0005;
	  for (i=0;i<5;i++)
	 		  {
	 			  next_cycle(clk);
	 		  }

	  addr=0x05 ; data_in=0x0005;
	  for (i=0;i<5;i++)
	  	 	 {
	  	 		  next_cycle(clk);
	  	 	 }

	  Read_Write = 0; // Read operation
	  addr=0x01 ;
	  for (i=0;i<5;i++)
	 		  {
	 			  next_cycle(clk);
	 		  }

	  addr=0x11 ;
		  for (i=0;i<5;i++)
		 		  {
		 			  next_cycle(clk);
		 		  }
	 addr=0x05 ;
		  for (i=0;i<5;i++)
		  		 {
		  		 	 next_cycle(clk);
		  		 }
	 rst=1; // resetting the rram to zero
		 for (i=0;i<5;i++)
				 {
				  	next_cycle(clk);
				 }

	 Read_Write = 1; // write operation
	 rst=0;		  // reset not enabled

	addr=0x00 ;  data_in=0x1111;
		for (i=0;i<5;i++)
		 	{
		 		next_cycle(clk);
		 	}
	Read_Write = 0; next_cycle(clk); Read_Write = 1;
	addr=0x01 ; data_in=0x0033;
		for (i=0;i<5;i++)
		 	{
		 		next_cycle(clk);
		 	}
	Read_Write = 0; next_cycle(clk); Read_Write = 1;
	addr=0x08 ;  data_in=0xffff;
		for (i=0;i<5;i++)
			 {
			 	next_cycle(clk);
			 }

	addr=0x09 ; data_in=0x0004;
			for (i=0;i<5;i++)
			 {
			 	next_cycle(clk);
			 }


	mul_enable = 1;//enable multiplication
	Read_Write = 0;
	addr=0x00;
	for (i=0;i<5;i++)
	 {
	 	next_cycle(clk);
	 }
	addr=0x01;
	for (i=0;i<5;i++)
	 {
	 	next_cycle(clk);
	 }
	addr=0x08;
	for (i=0;i<5;i++)
	 {
	 	next_cycle(clk);
	 }
	addr=0x09;
	for (i=0;i<5;i++)
	 {
	 	next_cycle(clk);
	 }




	    sc_close_vcd_trace_file (my_trace_file);

	    return EXIT_SUCCESS;

}

void next_cycle (sc_signal<bool> &signal_clk)
{
    signal_clk = true;
    sc_start (1, SC_NS);
    signal_clk = false;
    sc_start (1, SC_NS);
}
