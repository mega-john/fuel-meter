/*
 * ili9341.c
 *
 * Created: 05.11.2013 11:01:37
 *  Author: estarcev
 */ 
#include "ili9341.h"

//#define FONT_SPACE 20
//#define FONT_X 5
//#define FONT_Y 8

uint8_t DisplayDirect;
FontCallback	FontRead = TFT_ReadFontData;
const uint8_t*	Font = 0x00;

void TFT_init( void )
{
	//init TFT pins
	TFT_DDR |= TFT_RST;
	TFT_DDR |= TFT_CS;
	TFT_DDR |= TFT_DC;
	
	//start TFT initialization
    SPI_begin();
    TFT_CS_HIGH;
    TFT_DC_HIGH;

    TFT_RST_ON;
    _delay_ms(10);
    TFT_RST_OFF;
    _delay_ms(10);
    
    TFT_sendCMD(0xCB);//power control A
    TFT_WRITE_DATA(0x39);
    TFT_WRITE_DATA(0x2C);
    TFT_WRITE_DATA(0x00);
    TFT_WRITE_DATA(0x34);//Vcore = 1.6
    TFT_WRITE_DATA(0x02);//DDVDH = 5.6

    TFT_sendCMD(0xCF);//power control B
    TFT_WRITE_DATA(0x00);
    TFT_WRITE_DATA(0XC1);
    TFT_WRITE_DATA(0X30);

    TFT_sendCMD(0xE8);//driver timing control A
    TFT_WRITE_DATA(0x85);//gate driver non-overlap timing control: 1:default + 1unit 
    TFT_WRITE_DATA(0x00);//EQ timing control: 0 - default – 1unit,  CR timing control: 0 - default – 1unit
    TFT_WRITE_DATA(0x78);//pre-charge timing control: 10 - default pre-charge timing 

    TFT_sendCMD(0xEA);//driver timing control B
    TFT_WRITE_DATA(0x00);
    TFT_WRITE_DATA(0x00);

    TFT_sendCMD(0xED);//Power on sequence control
    TFT_WRITE_DATA(0x64);
    TFT_WRITE_DATA(0x03);
    TFT_WRITE_DATA(0X12);
    TFT_WRITE_DATA(0X81);//DDVDH enhance mode(only for 8 external capacitors): enable

    TFT_sendCMD(0xF7);//Pump ratio control
    TFT_WRITE_DATA(0x20);//ratio control = 10:DDVDH=2xVCI 

    TFT_sendCMD(0xC0);//Power Control 1
    TFT_WRITE_DATA(0x23);//VRH[5:0] = 4.6V

    TFT_sendCMD(0xC1);//Power control 2
    TFT_WRITE_DATA(0x10);//SAP[2:0];BT[3:0]

    TFT_sendCMD(0xC5);//VCM control 1
    TFT_WRITE_DATA(0x3e);//Contrast, VCOMH(V) = 4.275
    TFT_WRITE_DATA(0x28);//VCOML(V)  = -1.500

    TFT_sendCMD(0xC7);//VCM control 2
    TFT_WRITE_DATA(0x86);//Set the VCOM offset voltage: VMH – 58  VML – 58

    TFT_sendCMD(0x36);// Memory Access Control
    TFT_WRITE_DATA(0x48);//C8         //48 68???//28 E8 ???

    TFT_sendCMD(0x3A);//Pixel Format Set
    TFT_WRITE_DATA(0x55);//16 bits / pixel 

    TFT_sendCMD(0xB1);//Frame Rate Control (In Normal Mode/Full Colors) 
    TFT_WRITE_DATA(0x00);
    TFT_WRITE_DATA(0x18);//24 clocks
    //TFT_WRITE_DATA(0x10);//16 clocks
    //TFT_WRITE_DATA(0x1F);//31 clocks

    TFT_sendCMD(0xB6);// Display Function Control
    TFT_WRITE_DATA(0x08);
    TFT_WRITE_DATA(0x82);
    TFT_WRITE_DATA(0x27);

    TFT_sendCMD(0xF2);// 3Gamma Function
    TFT_WRITE_DATA(0x01);//Disable

    TFT_sendCMD(0x26);//Gamma curve selected
    TFT_WRITE_DATA(0x00);

    TFT_sendCMD(0xE0);//Positive Gamma Correction
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

    TFT_sendCMD(0XE1);//Negative Gamma Correction
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

    TFT_sendCMD(0x11);//Exit Sleep
    _delay_ms(10);

    TFT_sendCMD(0x29);//Display on
    TFT_sendCMD(0x2c);//Memory Write
    TFT_fillScreen1();	
	TFT_setDisplayDirect(LEFT2RIGHT);
}

void TFT_setCol( uint16_t StartCol, uint16_t EndCol )
{
    TFT_sendCMD(0x2A);//Column Address Set
    TFT_sendData(StartCol);
    TFT_sendData(EndCol);	
}

void TFT_setPage( uint16_t StartPage, uint16_t EndPage )
{
    TFT_sendCMD(0x2B);//Page Address Set
    TFT_sendData(StartPage);
    TFT_sendData(EndPage);	
}

void TFT_setXY( uint16_t poX, uint16_t poY )
{
    TFT_setCol(poX, poX);
    TFT_setPage(poY, poY);
    TFT_sendCMD(0x2c);//Memory Write	
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
    TFT_DC_HIGH;
    TFT_CS_LOW;
    for(uint8_t count = 0; count < howmany; count++)
    {
	    SPI_transfer(data[count] >> 8);
	    SPI_transfer(data[count] & 0xff);
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
    TFT_DC_HIGH;
    TFT_CS_LOW;
    SPI_transfer(data >> 8);
    SPI_transfer(data & 0xff);
    TFT_CS_HIGH;
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
  
    for(i = 0; i < XY; i++)
    {
	    SPI_transfer(color >> 8);
	    SPI_transfer(color & 0xff);
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
    //for(uint16_t i = 0; i < 38400; i++)
    //{
	    //SPI_transfer(0);
	    //SPI_transfer(0);
	    //SPI_transfer(0);
	    //SPI_transfer(0);
    //}
    for(uint16_t i = 0; i < 19200; i++)
    {
	    SPI_transfer(0);
	    SPI_transfer(0);
	    SPI_transfer(0);
	    SPI_transfer(0);
	    SPI_transfer(0);
	    SPI_transfer(0);
	    SPI_transfer(0);
	    SPI_transfer(0);
    }
    TFT_CS_HIGH;	
}

void TFT_setDisplayDirect( uint8_t Direction)
{
	DisplayDirect = Direction;
}

static uint8_t firstChar = 0;
static uint8_t charCount = 0;
static uint8_t font_height = 0;
static uint8_t bytes_in_font_height = 0;

void TFT_SelectFont(const uint8_t* font)
{
	Font = font;
	firstChar = FontRead(Font + FONT_FIRST_CHAR);
	charCount = FontRead(Font + FONT_CHAR_COUNT);	
	font_height = FontRead(Font + FONT_HEIGHT);
	bytes_in_font_height = (font_height + 7) >> 3;
}

uint8_t CharWidth(char c)
{
	//uint8_t firstChar = FontRead(Font + FONT_FIRST_CHAR);
	//uint8_t charCount = FontRead(Font + FONT_CHAR_COUNT);
	//
	// read width data
	if (TFT_isFixedWidthFont(Font))
	{
		return FontRead(Font + FONT_FIXED_WIDTH);
	}
	if(c >= firstChar && c < (firstChar + charCount))
	{
		c -= firstChar;
		return FontRead(Font + FONT_WIDTH_TABLE + c) + 1;
	}
}

uint16_t TFT_StringWidth(const char* str)
{
	uint8_t width = 0;
	
	while(*str != 0)
	{
		width += CharWidth(*str++);
	}
	
	return width;
}

uint8_t TFT_drawChar(uint8_t c, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor, uint16_t bgcolor)
{
	uint8_t width = 0;
	//uint8_t height = FontRead(Font + FONT_HEIGHT);
	//uint8_t bytes_in_height = (height + 7) >> 3;// / 8;
	
	//uint8_t firstChar = FontRead(Font + FONT_FIRST_CHAR);
	//uint8_t charCount = FontRead(Font + FONT_CHAR_COUNT);
	//
	uint16_t index = 0;

	if(c < firstChar || c >= (firstChar + charCount))
	{
		return 0;
	}
	else if(c > 127)
	{
		c -= 0x40;
	}
	c -= firstChar;

	if(TFT_isFixedWidthFont(Font))
	{
		width = FontRead(Font + FONT_FIXED_WIDTH);
		index = c * bytes_in_font_height * width + FONT_WIDTH_TABLE;
	}
	else
	{
		for(uint8_t i = 0; i < c; i++)
		{
			index += FontRead(Font + FONT_WIDTH_TABLE + i);
		}
		index = index * bytes_in_font_height + charCount + FONT_WIDTH_TABLE;
		width = FontRead(Font + FONT_WIDTH_TABLE + c);
	}

	uint8_t f = 0;
	uint8_t j = 0;
	uint8_t page = 0;
	uint8_t data = 0;
	for(uint8_t i = 0; i < bytes_in_font_height; i++)
	{
		page = i * width;
		for(j = 0; j < width; j++)
		{
			data = FontRead(Font + index + page + j);
			
			//if(data == 0)
			//{
				//continue;
			//}
						
			if((font_height > 8) && (font_height < (i + 1) << 3))
			{
				data >>= (((i + 1) << 3) - font_height);
			}

			
			//TFT_fillRectangle(poX + j * size, poY /*+ size*/ + i * size * 8, size, size * 8, TFT_BLACK);
			
			//TFT_fillRectangle(poX + j * size, poY + 0 * size + i * size * 8, size, size, (data >> 0) & 0x01 ? fgcolor : TFT_BLACK);
			//TFT_fillRectangle(poX + j * size, poY + 1 * size + i * size * 8, size, size, (data >> 1) & 0x01 ? fgcolor : TFT_BLACK);
			//TFT_fillRectangle(poX + j * size, poY + 2 * size + i * size * 8, size, size, (data >> 2) & 0x01 ? fgcolor : TFT_BLACK);
			//TFT_fillRectangle(poX + j * size, poY + 3 * size + i * size * 8, size, size, (data >> 3) & 0x01 ? fgcolor : TFT_BLACK);
			//TFT_fillRectangle(poX + j * size, poY + 4 * size + i * size * 8, size, size, (data >> 4) & 0x01 ? fgcolor : TFT_BLACK);
			//TFT_fillRectangle(poX + j * size, poY + 5 * size + i * size * 8, size, size, (data >> 5) & 0x01 ? fgcolor : TFT_BLACK);
			//TFT_fillRectangle(poX + j * size, poY + 6 * size + i * size * 8, size, size, (data >> 6) & 0x01 ? fgcolor : TFT_BLACK);
			//TFT_fillRectangle(poX + j * size, poY + 7 * size + i * size * 8, size, size, (data >> 7) & 0x01 ? fgcolor : TFT_BLACK);
			
			
			//work good only for fixed width fonts!
			for(f = 0; f < 8; f++)
			{
#ifndef DEBUG_IN_STUDIO
				TFT_fillRectangle(poX + j * size, poY + f * size + i * size * 8, size, size, (data >> f) & 0x01 ? fgcolor : bgcolor);
#endif
			}
		}
	}
	return width + size;
}

void TFT_drawString(const char *string, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor, uint16_t bgcolor)
{
	uint8_t width = 0;
	
    while(*string)
    {
		width = TFT_drawChar(*string++, poX, poY, size, fgcolor, bgcolor);
	    
	    //*string++;
		
	    if(DisplayDirect == LEFT2RIGHT)
	    {
		    if(poX < MAX_X)
		    {
			    poX += width * size; // Move cursor right
		    }
	    }
	    else if(DisplayDirect == DOWN2UP)
	    {
		    if(poY > 0)
		    {
			    poY -= width * size; // Move cursor right
		    }
	    }
	    else if(DisplayDirect == RIGHT2LEFT)
	    {
		    if(poX > 0)
		    {
			    poX -= width * size; // Move cursor right
		    }
	    }
	    else if(DisplayDirect == UP2DOWN)
	    {
		    if(poY < MAX_Y)
		    {
			    poY += width * size; // Move cursor right
		    }
	    }
    }
}

void TFT_fillRectangle( uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color )
{
	for(uint16_t i = 0; i < width; i++)
	{
		if(DisplayDirect == LEFT2RIGHT || DisplayDirect == UP2DOWN)
		{
			TFT_drawHorizontalLine(poX, poY + i, length, color);
		}
		else 
		{
			TFT_drawHorizontalLine(poX, poY - i, length, color);
		}
	}
}

void TFT_drawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color )
{
    int x = x1 - x0;
    int y = y1 - y0;
    int dx = abs(x), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */
    for (;;)
	{
	    TFT_setPixel(x0, y0, color);
	    e2 = 2 * err;
	    if (e2 >= dy) 
		{ /* e_xy+e_x > 0 */
		    if (x0 == x1)
			{
				break;
			}
		    err += dy; 
			x0 += sx;
	    }
	    if (e2 <= dx) 
		{ /* e_xy+e_y < 0 */
		    if (y0 == y1)
			{
				break;
			}
		    err += dx; 
			y0 += sy;
	    }
    }	
}

void TFT_drawVerticalLine( uint16_t poX, uint16_t poY,uint16_t length, uint16_t color )
{
    TFT_setCol(poX, poX);
    TFT_setPage(poY, poY + length);
    TFT_sendCMD(0x2c);//Memory Write
  	TFT_DC_HIGH;
  	TFT_CS_LOW;

	for(uint16_t i = 0; i < length; i++)
    {
		SPI_transfer(color >> 8);
		SPI_transfer(color & 0xff);
		//TFT_sendData(color);	
	}
	
	TFT_CS_HIGH;
}

void TFT_drawHorizontalLine( uint16_t poX, uint16_t poY, uint16_t length, uint16_t color )
{
    TFT_setCol(poX, poX + length);
    TFT_setPage(poY, poY);
    TFT_sendCMD(0x2c);//Memory Write
	
	TFT_DC_HIGH;
	TFT_CS_LOW;
	
    for(uint16_t i = 0; i < length; i++)
    {
		SPI_transfer(color >> 8);
		SPI_transfer(color & 0xff);
		//TFT_sendData(color);	
	}
	
	TFT_CS_HIGH;
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
	    if (e2 > x)
		{
			err += ++x * 2 + 1;
		}
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
		    if (-x == y && e2 <= x)
			{
				e2 = 0;
			}
	    }
	    if (e2 > x)
		{
			err += ++x * 2 + 1;
		}
    } while (x <= 0);	
}

void TFT_drawTriangle( int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, uint16_t color )
{
    TFT_drawLine(poX1, poY1, poX2, poY2, color);
    TFT_drawLine(poX1, poY1, poX3, poY3, color);
    TFT_drawLine(poX2, poY2, poX3, poY3, color);	
}

//////////////////////////////////////////////////////////////////////////
//return count of symbols in long_num
//////////////////////////////////////////////////////////////////////////
uint8_t TFT_drawNumber( long long_num, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor, uint16_t bgcolor, uint16_t* out_width)
{
    uint8_t char_buffer[10] = "";
    uint8_t i = 0;
    uint8_t f = 0;
	uint8_t width = 0;

    if (long_num < 0)
    {
	    f = 1;
	    width = TFT_drawChar('-', poX, poY, size, fgcolor, bgcolor);
		//out_width += width;
	    long_num = -long_num;
	    if(poX < MAX_X)
	    {
		    poX += /*FONT_SPACE*/width * size; /* Move cursor right */
			*out_width = width /** size*/;
	    }
    }
    else if (long_num == 0)
    {
	    width = TFT_drawChar('0', poX, poY, size, fgcolor, bgcolor);
		*out_width = width /** size*/;
	    return 1;
	    //if(poX < MAX_X)
	    //{
		    //poX += /*FONT_SPACE*/width * size; /* Move cursor right */
	    //}
    }

    while (long_num > 0)
    {
	    char_buffer[i++] = long_num % 10;
	    long_num /= 10;
    }

    f = f + i;
    for(; i > 0; i--)
    {
	    width = TFT_drawChar('0' + char_buffer[i - 1], poX, poY, size, fgcolor, bgcolor);
		//out_width += width;
	    if(poX < MAX_X)
	    {
		    poX += /*FONT_SPACE*/width * size; /* Move cursor right */
			*out_width = width /** size*/;
	    }
    }
    return f;	
}

uint8_t TFT_drawFloat( float floatNumber, uint8_t decimal, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor, uint16_t bgcolor)
{
    uint16_t temp = 0;
    float decy = 0.0;
    float rounding = 0.5;
    uint8_t f = 0;
	uint16_t width = 0;
	uint16_t w = 0;
    if(floatNumber < 0.0)
    {
	    width = TFT_drawChar('-', poX, poY, size, fgcolor, bgcolor);
	    floatNumber = -floatNumber;
	    if(poX < MAX_X)
	    {
		    poX += /*FONT_SPACE*/width * size; /* Move cursor right */
	    }
	    f = 1;
    }
    for (uint8_t i = 0; i < decimal; ++i)
    {
	    rounding /= 10.0;
    }
    floatNumber += rounding;

    temp = (uint16_t)floatNumber;
    uint8_t howlong = TFT_drawNumber(temp, poX, poY, size, fgcolor, bgcolor, &w);
    f += howlong;
    if((poX + 8 * size * howlong) < MAX_X)
    {
	    poX += /*FONT_SPACE*/w * size * howlong; /* Move cursor right */
    }

    if(decimal > 0)
    {
	    width = TFT_drawChar('.', poX, poY, size, fgcolor, bgcolor);
	    if(poX < MAX_X)
	    {
		    poX += /*FONT_SPACE*/width * size; /* Move cursor right */
	    }
	    f += 1;
    }
    decy = floatNumber - temp; /* decimal part, 4 */
    for(uint8_t i = 0; i < decimal; i++)
    {
	    decy *= 10; /* for the next decimal */
	    temp = decy; /* get the decimal */
		width =  0;
	    TFT_drawNumber(temp, poX, poY, size, fgcolor, bgcolor, &w);
	    floatNumber = -floatNumber;
	    if(poX < MAX_X)
	    {
		    poX += /*FONT_SPACE*/w * size; /* Move cursor right */
	    }
	    decy -= temp;
    }
    f += decimal;
    return f;	
}

uint8_t TFT_drawFloatRound( float floatNumber, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor, uint16_t bgcolor)
{
    uint8_t decimal = 2;
    uint16_t temp = 0;
    float decy = 0.0;
    float rounding = 0.5;
    uint8_t f = 0;
	uint16_t width = 0;
    if(floatNumber < 0.0) /* floatNumber < 0 */
    {
	    width = TFT_drawChar('-', poX, poY, size, fgcolor, bgcolor); /* add a '-' */
	    floatNumber = -floatNumber;
	    if(poX < MAX_X)
	    {
		    poX += /*FONT_SPACE*/width * size; /* Move cursor right */
	    }
	    f = 1;
    }
    for (uint8_t i = 0; i < decimal; ++i)
    {
	    rounding /= 10.0;
    }
    floatNumber += rounding;

    temp = (uint16_t)floatNumber;
    uint8_t howlong = TFT_drawNumber(temp, poX, poY, size, fgcolor, bgcolor, &width);
    f += howlong;
    if((poX + 8 * size * howlong) < MAX_X)
    {
	    poX += /*FONT_SPACE*/width * size * howlong; /* Move cursor right */
    }

    if(decimal > 0)
    {
	    width = TFT_drawChar('.', poX, poY, size, fgcolor, bgcolor);
	    if(poX < MAX_X)
	    {
		    poX += /*FONT_SPACE*/width * size; /* Move cursor right */
	    }
	    f += 1;
    }
    decy = floatNumber - temp; /* decimal part, */
    for(uint8_t i = 0; i < decimal; i++)
    {
	    decy *= 10; /* for the next decimal */
	    temp = decy; /* get the decimal */
	    TFT_drawNumber(temp, poX, poY, size, fgcolor, bgcolor, &width);
	    floatNumber = -floatNumber;
	    if(poX < MAX_X)
	    {
		    poX += /*FONT_SPACE*/width * size; /* Move cursor right */
	    }
	    decy -= temp;
    }
    f += decimal;
    return f;	
}

uint8_t TFT_ReadFontData(const uint8_t* ptr)
{
	return pgm_read_byte(ptr);
}

void TFT_set_orientation(uint8_t orientation)
{
	TFT_sendCMD(0x36);//wr_cmd(0x36);                     // MEMORY_ACCESS_CONTROL
	switch (orientation) 
	{
		case 0:
			TFT_WRITE_DATA(0x48);//_spi.write(0x48);
		break;
		case 1:
			TFT_WRITE_DATA(0x28);//_spi.write(0x28);
		break;
		case 2:
			TFT_WRITE_DATA(0x88);//_spi.write(0x88);
		break;
		case 3:
			TFT_WRITE_DATA(0xE8);//_spi.write(0xE8);
		break;
	}
    //TFT_CS_HIGH;//_cs = 1;
}