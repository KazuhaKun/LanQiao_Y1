#include <REG52.H>

void InitHC138(unsigned char channel)
{
    P2 = (P2 & 0x1f) | (channel << 5);
}

// void InitHC138(unsigned char n)
// {
// 	switch(n)
// 	{
// 		case 4:
// 			P2 = (P2 & 0X1f) | 0x80;
// 		break;
// 		case 5:
// 			P2 = (P2 & 0X1f) | 0xa0;
// 		break;
// 		case 6:
// 			P2 = (P2 & 0X1f) | 0xc0;
// 		break;
// 		case 7:
// 			P2 = (P2 & 0X1f) | 0xe0;
// 		break;
// 	}
// }