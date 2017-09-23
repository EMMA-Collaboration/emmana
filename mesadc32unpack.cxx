// mesadc32unpack.cxx

#include <stdio.h>
#include <stdint.h>

#include "mesadc32unpack.h"

mesadc32event::mesadc32event() // ctor
{
   error = false;
}

void mesadc32event::Print() const
{
   printf("mesadc32event: error %d, module_id %d, nwords32 %d, timestamp 0x%08x\n",
          error,
          module_id,
          nwords32,
          time_stamp);
   for (unsigned i=0; i<hits.size(); i++) {
      printf("mesadc32hit[%2d], channel %2d, overflow %d, adc_data %5d\n", i, hits[i].channel, hits[i].v, hits[i].adc_data);
   }
};

mesadc32event* UnpackMesadc32(const char** data8, int* datalen, bool verbose)
{
   const uint32_t *data = (const uint32_t*)(*data8);
   int count = (*datalen)/4;

   mesadc32event* e = new mesadc32event();

   // ADC data is: event header, optional data words and an end of event word

   if (verbose) {
      printf("datalen %d, count %d, header 0x%08x\n", *datalen, count, data[0]);
   }

   // less than 2 words
   if (count < 2) {
      e->error = true;
      // consume all words
      *data8 += *datalen;
      *datalen -= *datalen;
      return e;
   }

   if ((data[0]>>30) != 0x1) { // header marker
      e->error = true;
      // first word is not a header, consume it
      *data8 += 4;
      *datalen -= 4;
      return e;
   }

   e->module_id = (data[0]>>16)&0xFF; // 8 bits
   e->nwords32 = (data[0]>>0)&0xFFF; // 12 bits

   data++;
   count--;
   *data8 += 4;
   *datalen -= 4;

   int nw32 = e->nwords32;

   if (count < e->nwords32) {
      e->error = true;
      // too few data words
      nw32 = count;
   }

   if (verbose) {
      printf("datalen %d, count %d, nw32 %d\n", *datalen, count, nw32);
   }

   for (int i=0; i<nw32; i++) {

      if (verbose) {
         printf("data[%d] is 0x%08x\n", i, data[0]);
      }

      if ((data[0]>>30) != 0x0) {
         // not a data word
         break;
      }

      mesadc32hit h;
      h.channel = (data[0]>>16)&0x1F; // 5 bits
      h.v = (data[0]>>15)&0x1; // 1 bit
      h.adc_data = (data[0]>>0)&0xFFF; // 12 bits
      e->hits.push_back(h);

      data++;
      count--;
      *data8 += 4;
      *datalen -= 4;
   }

   if (verbose) {
      printf("datalen %d, count %d, footer 0x%08x\n", *datalen, count, data[0]);
   }

   if (count < 1) {
      e->error = true;
      // too few data words, end of event word is missing?
      return e;
   }

   if ((data[0]>>30) != 0x3) { // end of event marker
      e->error = true;
      // last word is not a footer
      return e;
   }

   e->time_stamp = data[0]&0x3FFFFFFF;

   data++;
   count--;
   *data8 += 4;
   *datalen -= 4;

   if (verbose) {
      printf("datalen %d, count %d, done\n", *datalen, count);
   }

   return e;
}

//end
/* emacs
 * Local Variables:
 * tab-width: 8
 * c-basic-offset: 3
 * indent-tabs-mode: nil
 * End:
 */
