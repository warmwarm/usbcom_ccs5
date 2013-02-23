
#include "inc/hw_types.h"
#include "usbcom.h"
#include "setlut.h"
/* this file is used to manually set the LUT table.
 */

#if 0
int LUTTable[256] =
{
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1, 
 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1 
};
#endif

#ifdef 0
int LUTTable[256] =
{
   2,   78,  109,  126,  141,  153,  164,  175, /*  1*8 */
 184,  191,  199,  206,  212,  219,  225,  231, /*  2*8 */
 236,  243,  248,  255,  261,  267,  273,  280, /*  3*8 */
 287,  294,  300,  306,  312,  319,  325,  331, /*  4*8 */
 338,  343,  349,  355,  361,  366,  372,  377, /*  5*8 */
 383,  388,  394,  399,  405,  410,  416,  422, /*  6*8 */
 427,  432,  438,  443,  449,  455,  460,  465, /*  7*8 */
 470,  476,  481,  487,  492,  497,  502,  508, /*  8*8 */
 512,  517,  522,  526,  531,  536,  540,  545, /*  9*8 */
 550,  554,  559,  564,  568,  573,  578,  582, /* 10*8 */
 587,  591,  596,  600,  604,  608,  613,  618, /* 11*8 */
 622,  626,  631,  636,  641,  645,  649,  654, /* 12*8 */
 658,  663,  668,  673,  678,  682,  687,  692, /* 13*8 */
 697,  702,  707,  712,  717,  722,  728,  733, /* 14*8 */
 738,  743,  748,  754,  759,  765,  770,  776, /* 15*8 */
 781,  787,  792,  798,  804,  809,  815,  821, /* 16*8 */
 827,  833,  839,  845,  851,  857,  863,  870, /* 17*8 */
 876,  883,  889,  896,  903,  910,  916,  924, /* 18*8 */
 931,  938,  945,  953,  960,  968,  975,  984, /* 19*8 */
 991,  999, 1006, 1014, 1021, 1028, 1035, 1042, /* 20*8 */
1049, 1056, 1064, 1071, 1078, 1085, 1092, 1100, /* 21*8 */
1107, 1114, 1122, 1129, 1136, 1144, 1151, 1159, /* 22*8 */
1166, 1174, 1182, 1190, 1197, 1205, 1212, 1220, /* 23*8 */
1228, 1236, 1244, 1252, 1260, 1268, 1276, 1284, /* 24*8 */
1292, 1301, 1309, 1318, 1327, 1335, 1344, 1353, /* 25*8 */
1362, 1371, 1380, 1389, 1398, 1408, 1417, 1427, /* 26*8 */
1437, 1447, 1457, 1467, 1477, 1487, 1498, 1509, /* 27*8 */
1520, 1531, 1542, 1553, 1564, 1575, 1586, 1597, /* 28*8 */
1608, 1619, 1631, 1642, 1654, 1666, 1677, 1689, /* 29*8 */
1702, 1714, 1726, 1738, 1751, 1764, 1776, 1790, /* 30*8 */
1804, 1819, 1833, 1847, 1861, 1875, 1890, 1905, /* 31*8 */
1921, 1938, 1956, 1972, 1990, 2012, 2027, 2047  /* 32*8 */
};
#endif

/* Following LUT table generated in 2012-07-25. Fully compliant with the DICOM. */
int LUTTable[256] =
{
0,    3,    7,   11,   13,   15,   20,   22,
25,   28,   31,   33,   35,   38,   41,   44,
47,   50,   53,   56,   59,   62,   65,   68,
71,   74,   77,   81,   84,   87,   91,   94,
98,  102,  106,  110,  114,  118,  123,  127,
131,  135,  139,  144,  148,  153,  158,  162,
166,  170,  175,  179,  183,  187,  190,  194,
198,  202,  206,  209,  213,  216,  220,  223,
227,  230,  234,  238,  241,  245,  249,  253,
257,  261,  265,  269,  273,  278,  282,  286,
290,  294,  298,  303,  307,  311,  316,  320,
325,  330,  335,  340,  345,  350,  355,  362,
367,  372,  377,  383,  388,  394,  399,  404,
410,  416,  423,  428,  434,  440,  446,  451,
457,  463,  469,  475,  482,  488,  494,  500,
506,  512,  519,  525,  532,  538,  545,  551,
558,  565,  572,  578,  585,  592,  598,  605,
612,  620,  628,  637,  646,  653,  661,  670,
678,  687,  695,  704,  712,  721,  730,  739,
748,  757,  766,  774,  784,  793,  802,  812,
822,  830,  839,  848,  859,  869,  880,  890,
900,  911,  921,  931,  941,  952,  963,  973,
984,  994, 1005, 1016, 1028, 1038, 1049, 1061,
1072, 1084, 1095, 1107, 1119, 1131, 1143, 1155,
1167, 1179, 1191, 1204, 1216, 1228, 1241, 1253,
1266, 1279, 1292, 1306, 1319, 1332, 1346, 1360,
1374, 1388, 1401, 1414, 1429, 1443, 1458, 1472,
1486, 1501, 1515, 1529, 1544, 1560, 1575, 1590,
1604, 1619, 1635, 1649, 1665, 1680, 1695, 1709,
1725, 1742, 1756, 1770, 1786, 1802, 1816, 1831,
1846, 1862, 1877, 1892, 1905, 1920, 1933, 1945,
1956, 1966, 1976, 1985, 1993, 2002, 2013, 2047
};

#ifdef 0
int LUTTable[256] =
{
 0, 0, 0, 0, 0, 2047, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0
};
#endif

unsigned char LUTTableRef[512];  //reference table stores the 16bit values.

tBoolean writePageToEEPROM (unsigned char* rev_buf)
{

    unsigned char pageIndex;
    unsigned char i;
    unsigned long Addr16;
    unsigned long LoAdd;
    unsigned long HiAdd;
    unsigned char value;

    InformFPGAReleaseEEPROM();
    EnableTristate();
        
    pageIndex = rev_buf[1];
    for (i=0; i<32; i++)      /* index inside the page */
    {
       Addr16= pageIndex*32 + i;
       HiAdd = HIGH(Addr16);
       LoAdd = LOW(Addr16);

       value = rev_buf[i + 2]; /*recv_buf[0] ingored here.*/

       u1printf("%d\n\r",value);

       if(!SPIWriteByte(HiAdd, LoAdd, value))
       {
         u1printf("write page fail\n\r");  /* check if we have printf routines...*/
       
         DisableTristate();
         InformFPGATakeEEPROM();
         return false;
       }
    }

    DisableTristate();
    InformFPGATakeEEPROM();
    return true;
}

tBoolean writeLUTToEEPROM (tBoolean set_default)
{
    int i;
    unsigned char index; /*page number */
    tBoolean write_rc;

    unsigned short int entry_value;

    u1printf("write LUT begin:\n\r");

    for (i=0; i<256; i++)
    {
      entry_value = LUTTable[i];

      LUTTableRef[i*2]  = LOW(entry_value);
      LUTTableRef[i*2+1]= HIGH(entry_value);
    }

    unsigned char page[34];

    for (index=0; index<16; index++)  /* index is page number */
    {
      if(set_default)   /* no use here */
        page[0]=2;
      else
        page[0]=0;

      page[1]=index;

      for(i=0; i<32; i++)
        page[i+2] = LUTTableRef[32*index+i];

      write_rc = writePageToEEPROM(page);  /* page index + content. */

      if (write_rc==false)
      {
    	  u1printf("write page fail.\n\r");
    	  return false;
      }
    }

    u1printf("write LUT done.\n\r");
    return true;
}

tBoolean readLUTFromEEPROM()
{
	 int i;
	 Uchar value;
	 unsigned char valuechar[10];
	 unsigned long LoAdd, HiAdd;

	 InformFPGAReleaseEEPROM();
	 EnableTristate();

	 print("read LUT begin:");
	 for(i=0;i<512;i++)
	 {
		HiAdd = HIGH(i);
	    LoAdd = LOW(i);
	    if (!SPIReadByte(HiAdd, LoAdd, &value))
	    {
	        print("read LUT fail.");  /* check if we have printf routines...*/

	        DisableTristate();
	        InformFPGATakeEEPROM();
	        return false;
	    }
	    itoa(valuechar, value);
	    print(valuechar);
	}

	 print("read LUT done.");
	DisableTristate();
	InformFPGATakeEEPROM();
	return true;
}

