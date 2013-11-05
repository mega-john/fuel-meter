/*
 * ili9341.c
 *
 * Created: 05.11.2013 11:01:37
 *  Author: estarcev
 */ 
#include "ili9341.h"

#define FONT_SPACE 6
#define FONT_X 8
#define FONT_Y 8

void TFT_init( void )
{
    SPI_begin();
    TFT_CS_HIGH;
    TFT_DC_HIGH;
    uint8_t i = 0;
	uint8_t TFTDriver = 0;

    TFT_RST_ON;
    _delay_ms(10);
    TFT_RST_OFF;

    for(i = 0; i < 3; i++)
    {
	    TFTDriver = TFT_readID();
    }

    TFT_sendCMD(0xCB);
    TFT_WRITE_DATA(0x39);
    TFT_WRITE_DATA(0x2C);
    TFT_WRITE_DATA(0x00);
    TFT_WRITE_DATA(0x34);
    TFT_WRITE_DATA(0x02);

    TFT_sendCMD(0xCF);
    TFT_WRITE_DATA(0x00);
    TFT_WRITE_DATA(0XC1);
    TFT_WRITE_DATA(0X30);

    TFT_sendCMD(0xE8);
    TFT_WRITE_DATA(0x85);
    TFT_WRITE_DATA(0x00);
    TFT_WRITE_DATA(0x78);

    TFT_sendCMD(0xEA);
    TFT_WRITE_DATA(0x00);
    TFT_WRITE_DATA(0x00);

    TFT_sendCMD(0xED);
    TFT_WRITE_DATA(0x64);
    TFT_WRITE_DATA(0x03);
    TFT_WRITE_DATA(0X12);
    TFT_WRITE_DATA(0X81);

    TFT_sendCMD(0xF7);
    TFT_WRITE_DATA(0x20);

    TFT_sendCMD(0xC0);         //Power control
    TFT_WRITE_DATA(0x23);         //VRH[5:0]

    TFT_sendCMD(0xC1);         //Power control
    TFT_WRITE_DATA(0x10);         //SAP[2:0];BT[3:0]

    TFT_sendCMD(0xC5);         //VCM control
    TFT_WRITE_DATA(0x3e);         //Contrast
    TFT_WRITE_DATA(0x28);

    TFT_sendCMD(0xC7);         //VCM control2
    TFT_WRITE_DATA(0x86);          //--

    TFT_sendCMD(0x36);         // Memory Access Control
    TFT_WRITE_DATA(0x48);         //C8         //48 68???//28 E8 ???

    TFT_sendCMD(0x3A);
    TFT_WRITE_DATA(0x55);

    TFT_sendCMD(0xB1);
    TFT_WRITE_DATA(0x00);
    TFT_WRITE_DATA(0x18);

    TFT_sendCMD(0xB6);         // Display Function Control
    TFT_WRITE_DATA(0x08);
    TFT_WRITE_DATA(0x82);
    TFT_WRITE_DATA(0x27);
    
    TFT_sendCMD(0xF2);         // 3Gamma Function Disable
    TFT_WRITE_DATA(0x00);

    TFT_sendCMD(0x26);         //Gamma curve selected
    TFT_WRITE_DATA(0x01);

    TFT_sendCMD(0xE0);         //Set Gamma
    TFT_WRITE_DATA(0x0F);
    TFT_WRITE_DATA(0x31);
    TFT_WRITE_DATA(0x2B);
    TFT_WRITE_DATA(0x0C);
    TFT_WRITE_DATA(0x0E);
    TFT_WRITE_DATA(0x08);
    TFT_WRITE_DATA(0x4E);
    TFT_WRITE_DATA(0xF1);
    TFT_WRITE_DATA(0x37);
    TFT_WRITE_DATA(0x07);
    TFT_WRITE_DATA(0x10);
    TFT_WRITE_DATA(0x03);
    TFT_WRITE_DATA(0x0E);
    TFT_WRITE_DATA(0x09);
    TFT_WRITE_DATA(0x00);

    TFT_sendCMD(0XE1);         //Set Gamma
    TFT_WRITE_DATA(0x00);
    TFT_WRITE_DATA(0x0E);
    TFT_WRITE_DATA(0x14);
    TFT_WRITE_DATA(0x03);
    TFT_WRITE_DATA(0x11);
    TFT_WRITE_DATA(0x07);
    TFT_WRITE_DATA(0x31);
    TFT_WRITE_DATA(0xC1);
    TFT_WRITE_DATA(0x48);
    TFT_WRITE_DATA(0x08);
    TFT_WRITE_DATA(0x0F);
    TFT_WRITE_DATA(0x0C);
    TFT_WRITE_DATA(0x31);
    TFT_WRITE_DATA(0x36);
    TFT_WRITE_DATA(0x0F);

    TFT_sendCMD(0x11);         //Exit Sleep
    _delay_ms(120);

    TFT_sendCMD(0x29); //Display on
    TFT_sendCMD(0x2c);
    TFT_fillScreen1();	
}

void TFT_setCol( uint16_t StartCol, uint16_t EndCol )
{
    TFT_sendCMD(0x2A); /* Column Command address */
    TFT_sendData(StartCol);
    TFT_sendData(EndCol);	
}

void TFT_setPage( uint16_t StartPage, uint16_t EndPage )
{
    TFT_sendCMD(0x2B); /* Column Command address */
    TFT_sendData(StartPage);
    TFT_sendData(EndPage);	
}

void TFT_setXY( uint16_t poX, uint16_t poY )
{
    TFT_setCol(poX, poX);
    TFT_setPage(poY, poY);
    TFT_sendCMD(0x2c);	
}

void TFT_setPixel( uint16_t poX, uint16_t poY, uint16_t color )
{
    TFT_setXY(poX, poY);
    TFT_sendData(color);	
}

void TFT_sendCMD( uint8_t index )
{
    TFT_DC_LOW;
    TFT_CS_LOW;
    SPI_transfer(index);
    TFT_CS_HIGH;
}

void TFT_WRITE_Package( uint16_t *data, uint8_t howmany )
{
    uint16_t data1 = 0;
    uint8_t data2 = 0;

    TFT_DC_HIGH;
    TFT_CS_LOW;
    uint8_t count = 0;
    for(count = 0; count < howmany; count++)
    {
	    data1 = data[count] >> 8;
	    data2 = data[count] & 0xff;
	    SPI_transfer(data1);
	    SPI_transfer(data2);
    }
    TFT_CS_HIGH;	
}

void TFT_WRITE_DATA( uint8_t data )
{
	TFT_DC_HIGH;
	TFT_CS_LOW;
	SPI_transfer(data);
	TFT_CS_HIGH;
}

void TFT_sendData( uint16_t data )
{
    uint8_t data1 = data >> 8;
    uint8_t data2 = data & 0xff;
    TFT_DC_HIGH;
    TFT_CS_LOW;
    SPI_transfer(data1);
    SPI_transfer(data2);
    TFT_CS_HIGH;	
}

uint8_t TFT_Read_Register( uint8_t Addr, uint8_t xParameter )
{
    uint8_t data = 0;
    TFT_sendCMD(0xd9); /* ext command */
    TFT_WRITE_DATA(0x10 + xParameter); /* 0x11 is the first Parameter */
    TFT_DC_LOW;
    TFT_CS_LOW;
    SPI_transfer(Addr);
    TFT_DC_HIGH;
    data = SPI_transfer(0);
    TFT_CS_HIGH;
    return data;	
}

void TFT_fillScreen( uint16_t XL, uint16_t XR, uint16_t YU, uint16_t YD, uint16_t color )
{
    unsigned long XY = 0;
    unsigned long i = 0;

    if(XL > XR)
    {
	    XL = XL ^ XR;
	    XR = XL ^ XR;
	    XL = XL ^ XR;
    }
    if(YU > YD)
    {
	    YU = YU ^ YD;
	    YD = YU ^ YD;
	    YU = YU ^ YD;
    }
    XL = constrain(XL, MIN_X, MAX_X);
    XR = constrain(XR, MIN_X, MAX_X);
    YU = constrain(YU, MIN_Y, MAX_Y);
    YD = constrain(YD, MIN_Y, MAX_Y);

    XY = (XR - XL + 1);
    XY = XY * (YD - YU + 1);

    TFT_setCol(XL, XR);
    TFT_setPage(YU, YD);
    TFT_sendCMD(0x2c); /* start to write to display ram */

    TFT_DC_HIGH;
    TFT_CS_LOW;

    uint8_t Hcolor = color >> 8;
    uint8_t Lcolor = color & 0xff;
    for(i = 0; i < XY; i++)
    {
	    SPI_transfer(Hcolor);
	    SPI_transfer(Lcolor);
    }
    TFT_CS_HIGH;	
}

void TFT_fillScreen1(void)
{
    TFT_setCol(0, 239);
    TFT_setPage(0, 319);
    TFT_sendCMD(0x2c); /* start to write to display ram */

    TFT_DC_HIGH;
    TFT_CS_LOW;
    for(uint16_t i = 0; i < 38400; i++)
    {
	    SPI_transfer(0);
	    SPI_transfer(0);
	    SPI_transfer(0);
	    SPI_transfer(0);
    }
    TFT_CS_HIGH;	
}

uint8_t TFT_readID( void )
{
    uint8_t i = 0;
    uint8_t data[3];
    uint8_t ID[3] = {0x00, 0x93, 0x41};
    uint8_t ToF = 1;
    for(i = 0;i < 3; i++)
    {
	    data[i] = TFT_Read_Register(0xd3, i + 1);
	    if(data[i] != ID[i])
	    {
		    ToF = 0;
	    }
    }
    //if(!ToF) /* data!=ID */
    //{
	    //Serial.print("Read TFT ID failed, ID should be 0x09341, but read ID = 0x");
	    //for(i=0;i<3;i++)
	    //{
		    //Serial.print(data[i],HEX);
	    //}
	    //Serial.println();
    //}
    return ToF;	
}

void TFT_drawChar( uint8_t ascii, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor )
{
    //if((ascii >= 32) && (ascii <= 127))
    //{
	    //;
    //}
    //else
    //{
	    //ascii = '?' - 32;
    //}
    //for (int i = 0; i < FONT_X; i++ ) 
	//{
	    //uint8_t temp = pgm_read_byte(&simpleFont[ascii - 0x20][i]);
	    //for(uint8_t f = 0; f < 8; f++)
	    //{
		    //if((temp >> f) & 0x01)
		    //{
			    //TFT_fillRectangle(poX + i * size, poY + f * size, size, size, fgcolor);
		    //}
	    //}
    //}	
}

void TFT_drawString( char *string, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor )
{
    while(*string)
    {
	    TFT_drawChar(*string, poX, poY, size, fgcolor);
	    *string++;

	    if(poX < MAX_X)
	    {
		    poX += FONT_SPACE * size; /* Move cursor right */
	    }
    }	
}

void TFT_fillRectangle( uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color )
{
    TFT_fillScreen(poX, poX + length, poY, poY + width, color);
}

void TFT_drawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color )
{
    int x = x1 - x0;
    int y = y1 - y0;
    int dx = abs(x), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */
    for (;;){ /* loop */
	    TFT_setPixel(x0, y0, color);
	    e2 = 2 * err;
	    if (e2 >= dy) 
		{ /* e_xy+e_x > 0 */
		    if (x0 == x1) break;
		    err += dy; 
			x0 += sx;
	    }
	    if (e2 <= dx) 
		{ /* e_xy+e_y < 0 */
		    if (y0 == y1) break;
		    err += dx; 
			y0 += sy;
	    }
    }	
}

void TFT_drawVerticalLine( uint16_t poX, uint16_t poY,uint16_t length, uint16_t color )
{
    TFT_setCol(poX, poX);
    TFT_setPage(poY, poY + length);
    TFT_sendCMD(0x2c);
    for(int i = 0; i < length; i++)
    TFT_sendData(color);	
}

void TFT_drawHorizontalLine( uint16_t poX, uint16_t poY, uint16_t length, uint16_t color )
{
    TFT_setCol(poX, poX + length);
    TFT_setPage(poY, poY);
    TFT_sendCMD(0x2c);
    for(int i = 0; i < length; i++)
    TFT_sendData(color);	
}

void TFT_drawRectangle( uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color )
{
    TFT_drawHorizontalLine(poX, poY, length, color);
    TFT_drawHorizontalLine(poX, poY + width, length, color);
    TFT_drawVerticalLine(poX, poY, width, color);
    TFT_drawVerticalLine(poX + length, poY, width, color);	
}

void TFT_drawCircle( int poX, int poY, int r, uint16_t color )
{
    int x = -r, y = 0, err = 2 - 2 * r, e2;
    do {
	    TFT_setPixel(poX - x, poY + y, color);
	    TFT_setPixel(poX + x, poY + y, color);
	    TFT_setPixel(poX + x, poY - y, color);
	    TFT_setPixel(poX - x, poY - y, color);
	    e2 = err;
	    if (e2 <= y) 
		{
		    err += ++y * 2 + 1;
		    if (-x == y && e2 <= x) e2 = 0;
	    }
	    if (e2 > x) err += ++x * 2 + 1;
    } while (x <= 0);	
}

void TFT_fillCircle( int poX, int poY, int r, uint16_t color )
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {

	    TFT_drawVerticalLine(poX - x, poY - y, 2 * y, color);
	    TFT_drawVerticalLine(poX + x, poY - y, 2 * y, color);

	    e2 = err;
	    if (e2 <= y) 
		{
		    err += ++y * 2 + 1;
		    if (-x == y && e2 <= x) e2 = 0;
	    }
	    if (e2 > x) err += ++x * 2 + 1;
    } while (x <= 0);	
}

void TFT_drawTriangle( int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, uint16_t color )
{
    TFT_drawLine(poX1, poY1, poX2, poY2, color);
    TFT_drawLine(poX1, poY1, poX3, poY3, color);
    TFT_drawLine(poX2, poY2, poX3, poY3, color);	
}

uint8_t TFT_drawNumber( long long_num, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor )
{
    uint8_t char_buffer[10] = "";
    uint8_t i = 0;
    uint8_t f = 0;

    if (long_num < 0)
    {
	    f = 1;
	    TFT_drawChar('-', poX, poY, size, fgcolor);
	    long_num = -long_num;
	    if(poX < MAX_X)
	    {
		    poX += FONT_SPACE * size; /* Move cursor right */
	    }
    }
    else if (long_num == 0)
    {
	    f = 1;
	    TFT_drawChar('0', poX, poY, size, fgcolor);
	    return f;
	    if(poX < MAX_X)
	    {
		    poX += FONT_SPACE * size; /* Move cursor right */
	    }
    }


    while (long_num > 0)
    {
	    char_buffer[i++] = long_num % 10;
	    long_num /= 10;
    }

    f = f + i;
    for(; i > 0; i--)
    {
	    TFT_drawChar('0' + char_buffer[i - 1], poX, poY, size, fgcolor);
	    if(poX < MAX_X)
	    {
		    poX += FONT_SPACE * size; /* Move cursor right */
	    }
    }
    return f;	
}

uint8_t TFT_drawFloat( float floatNumber, uint8_t decimal, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor )
{
    uint16_t temp = 0;
    float decy = 0.0;
    float rounding = 0.5;
    uint8_t f = 0;
    if(floatNumber < 0.0)
    {
	    TFT_drawChar('-', poX, poY, size, fgcolor);
	    floatNumber = -floatNumber;
	    if(poX < MAX_X)
	    {
		    poX += FONT_SPACE * size; /* Move cursor right */
	    }
	    f = 1;
    }
    for (uint8_t i = 0; i < decimal; ++i)
    {
	    rounding /= 10.0;
    }
    floatNumber += rounding;

    temp = (uint16_t)floatNumber;
    uint8_t howlong = TFT_drawNumber(temp, poX, poY, size, fgcolor);
    f += howlong;
    if((poX + 8 * size * howlong) < MAX_X)
    {
	    poX += FONT_SPACE * size * howlong; /* Move cursor right */
    }

    if(decimal > 0)
    {
	    TFT_drawChar('.', poX, poY, size, fgcolor);
	    if(poX < MAX_X)
	    {
		    poX += FONT_SPACE * size; /* Move cursor right */
	    }
	    f += 1;
    }
    decy = floatNumber-temp; /* decimal part, 4 */
    for(uint8_t i = 0; i < decimal; i++)
    {
	    decy *= 10; /* for the next decimal */
	    temp = decy; /* get the decimal */
	    TFT_drawNumber(temp, poX, poY, size, fgcolor);
	    floatNumber = -floatNumber;
	    if(poX < MAX_X)
	    {
		    poX += FONT_SPACE * size; /* Move cursor right */
	    }
	    decy -= temp;
    }
    f += decimal;
    return f;
	
}

uint8_t TFT_drawFloat1( float floatNumber, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor )
{
    uint8_t decimal = 2;
    uint16_t temp = 0;
    float decy = 0.0;
    float rounding = 0.5;
    uint8_t f = 0;
    if(floatNumber <0.0) /* floatNumber < 0 */
    {
	    TFT_drawChar('-', poX, poY, size, fgcolor); /* add a '-' */
	    floatNumber = -floatNumber;
	    if(poX < MAX_X)
	    {
		    poX += FONT_SPACE * size; /* Move cursor right */
	    }
	    f = 1;
    }
    for (uint8_t i = 0; i < decimal; ++i)
    {
	    rounding /= 10.0;
    }
    floatNumber += rounding;

    temp = (uint16_t)floatNumber;
    uint8_t howlong = TFT_drawNumber(temp, poX, poY, size, fgcolor);
    f += howlong;
    if((poX + 8 * size * howlong) < MAX_X)
    {
	    poX += FONT_SPACE * size * howlong; /* Move cursor right */
    }

    if(decimal > 0)
    {
	    TFT_drawChar('.', poX, poY, size, fgcolor);
	    if(poX < MAX_X)
	    {
		    poX += FONT_SPACE * size; /* Move cursor right */
	    }
	    f += 1;
    }
    decy = floatNumber - temp; /* decimal part, */
    for(uint8_t i = 0; i < decimal; i++)
    {
	    decy *= 10; /* for the next decimal */
	    temp = decy; /* get the decimal */
	    TFT_drawNumber(temp, poX, poY, size, fgcolor);
	    floatNumber = -floatNumber;
	    if(poX < MAX_X)
	    {
		    poX += FONT_SPACE * size; /* Move cursor right */
	    }
	    decy -= temp;
    }
    f += decimal;
    return f;	
}