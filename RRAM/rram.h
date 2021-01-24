//@Module : RRAM
//@Author : Ahmed MAHMOUDI
//@Project : KI-PRO
//@Date   : 04/01/2021

#ifndef RRAM_RRAM_H_
#define RRAM_RRAM_H_



#include <unistd.h>
#include <string.h> // for using memcpy

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

using namespace std;

#define SC_INCLUDE_DYNAMIC_PROCESSES			//The simple sockets spawn dynamic processes



#define DATA_WIDTH        16    				// Data space in a cell
#define ADDR_WIDTH        8     				// 2^8 = 256 --> rram can store 256  cells == 256*16 == 4096 bits
                                				// data space in the chip = 4 kbits


struct rram : public sc_core::sc_module
{

	tlm_utils::simple_target_socket<rram> tsock; // Declaring a socket/port for he rram



  //-----------Internal variables-------------------
  uint16_t (*rram_data)= new uint16_t[256];		// array[256] of 16 bits cells, inner data of the RRAM
  uint32_t m_size = 256 ;






  //-----------Methods------------------------------


  //The target (RRAM) should copy data FROM the data array in the bus
  //Writing data in the RRAM
  void  write_data (unsigned addr,const uint16_t *src, unsigned num_bytes)
  {
	  assert(addr + num_bytes <= m_size); // to test if data sent is not greater to rram size

	  memcpy(rram_data,src+addr,num_bytes);
	  //copying data from the memory array where the starting address of RRAM is assigned
  }

//the target (RRAM) should copy data TO the data array in the bus
// reading from RRAM and writing to the bus
  void  read_data(unsigned addr, uint16_t* dst, unsigned num_bytes)
  {

	  assert(addr + num_bytes <= m_size); // to test if data sent is not greater to rram size

	  multiply_data();

	  memcpy(dst+addr,rram_data,num_bytes);
  }


  // Reset the internal data of the RRAM
  void  reset_data ()
  {
	  uint16_t null_cell = 0x0000;

	  for(int i=0;i<256;i++)
	  {
		  rram_data+i = null_cell;
	  }

  }

  // Multiplication algorithm via shifting and addition
  // m bit multiplicand * n bit multiplier == (m+n) bit product
  // accomplished via shifting and addition
  uint32_t multiply_two_cells(uint16_t cell_1,uint16_t cell_2)
  {

  	uint32_t result= 0x00000000;
  	//result = cell_1*cell_2);

      while( cell_2)
      {
          if(cell_2 & 1)
          {
          	result = result + cell_1;
          }
          cell_1 = cell_1 << 1;
          cell_2 = cell_2 >> 1;
      }

      return result;
  }



  // Multiply operation of each adjacent two cells of the RRAM
  void  multiply_data()
  // we will use a temporary array to store the multiplication result
  //then slice it to store it in the  adjacent cells
  {

      for(int i=0;i<255;i=i+2)
      {
      	uint32_t tmp = 0x00000000 ;
        tmp = multiply_two_cells( *(rram_data + i) , *(rram_data + (i+1)) );
        rram_data + i = tmp & 0xFFFF;
        rram_data + (i+1) = tmp >> 16;
      }

  }

  // Transport function used to pass payload to other systemC modules with a delay
  void transport(tlm::tlm_generic_payload &trans , sc_core::sc_time &delay)
	{
		tlm::tlm_command cmd = trans.get_command();

		uint32_t addr = trans.get_address();					// the start address in the system memory map of
																// contiguous block of data being read or written
																// for RRAM must be 1 byte = 2bits and in VP declared as 4 bytes

		/*auto*/ uint16_t *ptr = trans.get_data_ptr();			//The data pointer attribute points to a data buffer within the initiator(Bus)

		/*auto*/ unsigned len = trans.get_data_length();		// gives the length of data in bytes



		if (cmd == tlm::TLM_WRITE_COMMAND) {
			write_data(addr,ptr,len);
		} else if (cmd == tlm::TLM_READ_COMMAND) {
			read_data(addr,ptr,len);
		} else {
			sc_assert(false && "unsupported tlm command");
		}

		delay += sc_core::sc_time(10, sc_core::SC_NS);
	}


  ~rram(void) {
		delete rram_data;
	}


  //-----------Constructor--------------------------
  rram(sc_core::sc_module_name)
  {
		tsock.register_b_transport(this, &rram::transport);


  }

};


//////////////////////////////////////////////////////////////////////
//-----------------------RISC-V VP memory map-------------------------

//addr_t mem_size = 1024 * 1024 * 32;  // 32 MB ram, to place it before the CLINT

//addr_t mem_start_addr = 0x00000000;
//addr_t mem_end_addr = mem_start_addr + mem_size - 1;
//addr_t clint_start_addr = 0x02000000;
//addr_t clint_end_addr = 0x0200ffff;
//addr_t sys_start_addr = 0x02010000;
//addr_t sys_end_addr = 0x020103ff;

//-----------RRAM address in the memory map of risc-v vp-------------

// OFFset = 0x00FF as mem_size = 4kbits = 256 * 16 = 2^8 * 16
//addr_t rram_start_addr = 0x03000000;
//addr_t rram_end_addr = mem_start_addr + OFFset = 0x30000FF;

///////////////////////////////////////////////////////////////////////

#endif /* RRAM_RRAM_H_ */
