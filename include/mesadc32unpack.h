// mesadc32unpack.h

#include <vector>

class mesadc32hit
{
public:
   int channel; // 5 bits
   int v; // 1 bit // overflow bit
   int adc_data; // 12 bits
};

class mesadc32event
{
public:
   bool error;

   // event header word
   int module_id; // 8 bits
   int nwords32; // 12 bits

   // end of event word
   int time_stamp; // 30 bits

   // data words (hits)
   std::vector<mesadc32hit> hits;

public:
   mesadc32event(); // ctor
   void Print() const;
};

mesadc32event* UnpackMesadc32(const char** data, int* datalen, bool verbose);

//end
/* emacs
 * Local Variables:
 * tab-width: 8
 * c-basic-offset: 3
 * indent-tabs-mode: nil
 * End:
 */
