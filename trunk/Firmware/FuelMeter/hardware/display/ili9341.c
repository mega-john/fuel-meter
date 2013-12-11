/*
 * ili9341.c
 *
 * Created: 05.11.2013 11:01:37
 *  Author: estarcev
 */ 
#include "ili9341.h"

#define FONT_SPACE 6
#define FONT_X 5
#define FONT_Y 8

uint8_t DisplayDirect;
FontCallback	FontRead = 0x00;
uint8_t			FontColor = 0x00;
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
    //uint8_t i = 0;
	//uint8_t TFTDriver = 0;

    TFT_RST_ON;
    _delay_ms(10);
    TFT_RST_OFF;
    _delay_ms(10);

    //for(i = 0; i < 3; i++)
    //{
	    //TFTDriver = TFT_readID();
    //}
    
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
    //uint16_t data1 = 0;
    //uint8_t data2 = 0;
//
    //TFT_DC_HIGH;
    //TFT_CS_LOW;
    //uint8_t count = 0;
    //for(count = 0; count < howmany; count++)
    //{
	    //data1 = data[count] >> 8;
	    //data2 = data[count] & 0xff;
	    //SPI_transfer(data1);
	    //SPI_transfer(data2);
    //}
    //TFT_CS_HIGH;    
	//uint16_t data1 = 0;
    //uint8_t data2 = 0;

    TFT_DC_HIGH;
    TFT_CS_LOW;
    uint8_t count = 0;
    for(count = 0; count < howmany; count++)
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
    //uint8_t data1 = data >> 8;
    //uint8_t data2 = data & 0xff;
    //TFT_DC_HIGH;
    //TFT_CS_LOW;
    //SPI_transfer(data1);
    //SPI_transfer(data2);
    //TFT_CS_HIGH;    
	//uint8_t data1 = data >> 8;
    //uint8_t data2 = data & 0xff;
    TFT_DC_HIGH;
    TFT_CS_LOW;
    SPI_transfer(data >> 8);
    SPI_transfer(data & 0xff);
    TFT_CS_HIGH;
}

//uint8_t TFT_Read_Register( uint8_t Addr, uint8_t xParameter )
//{
    //uint8_t data = 0;
    //TFT_sendCMD(0xd9); /* ext command */
    //TFT_WRITE_DATA(0x10 + xParameter); /* 0x11 is the first Parameter */
    //TFT_DC_LOW;
    //TFT_CS_LOW;
    //SPI_transfer(Addr);
    //TFT_DC_HIGH;
    //data = SPI_transfer(0);
    //TFT_CS_HIGH;
    //return data;	
//}

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

    //uint8_t Hcolor = color >> 8;
    //uint8_t Lcolor = color & 0xff;
    //for(i = 0; i < XY; i++)
    //{
	    //SPI_transfer(Hcolor);
	    //SPI_transfer(Lcolor);
    //}    
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
    for(uint16_t i = 0; i < 38400; i++)
    {
	    SPI_transfer(0);
	    SPI_transfer(0);
	    SPI_transfer(0);
	    SPI_transfer(0);
    }
    TFT_CS_HIGH;	
}

//uint8_t TFT_readID( void )
//{
    //uint8_t i = 0;
    //uint8_t data[3];
    //uint8_t ID[3] = {0x00, 0x93, 0x41};
    //uint8_t ToF = 1;
    //for(i = 0;i < 3; i++)
    //{
	    //data[i] = TFT_Read_Register(0xd3, i + 1);
	    //if(data[i] != ID[i])
	    //{
		    //ToF = 0;
	    //}
    //}
    ////if(!ToF) /* data!=ID */
    ////{
	    ////Serial.print("Read TFT ID failed, ID should be 0x09341, but read ID = 0x");
	    ////for(i=0;i<3;i++)
	    ////{
		    ////Serial.print(data[i],HEX);
	    ////}
	    ////Serial.println();
    ////}
    //return ToF;	
//}

//void TFT_setOrientation(uint8_t HV)//horizontal or vertical
//{
	//TFT_sendCMD(0x03);
	//if(HV == 1)//vertical
	//{
		//TFT_sendData(0x5038);
	//}
	//else//horizontal
	//{
		//TFT_sendData(0x5030);
	//}
	//TFT_sendCMD(0x2c); //Start to write to display RAM
//}

void TFT_setDisplayDirect( uint8_t Direction)
{
	DisplayDirect = Direction;
}

void TFT_drawChar( uint8_t c, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor )
{
	uint8_t width = 0;
	uint8_t height = FontRead(Font + FONT_HEIGHT);
	uint8_t bytes = (height + 7) >> 3;// / 8;
	
	uint8_t firstChar = FontRead(Font + FONT_FIRST_CHAR);
	uint8_t charCount = FontRead(Font + FONT_CHAR_COUNT);
	
	uint16_t index = 0;

	if(c < firstChar || c >= (firstChar + charCount))
	{
		return 1;
	}
	else if(c > 127)
	{
		c -= 0x40;
	}
	c -= firstChar;

	if(isFixedWidthFont(Font))
	{
		//thielefont = 0;
		width = FontRead(Font + FONT_FIXED_WIDTH);
		index = c * bytes * width + FONT_WIDTH_TABLE;
	}
	else
	{
		//thielefont = 1;
		for(uint8_t i = 0; i < c; i++)
		{
			index += FontRead(Font + FONT_WIDTH_TABLE + i);
		}
		index = index * bytes + charCount + FONT_WIDTH_TABLE;
		width = FontRead(Font + FONT_WIDTH_TABLE + c);
	}

	for(uint8_t i = 0; i < bytes; i++)
	{
		uint8_t page = i * width;
		for(uint8_t j = 0; j < width; j++)
		{
			uint8_t data = FontRead(Font + index + page + j);
			
			if((height > 8) && (height < (i + 1) << 3))
			{
				data >>= (((i + 1) << 3) - height);
			}
			if(data == 0)
			{
				continue;
			}
			
			for(uint8_t f = 0; f < 8; f++)
			{
				if((data >> f) & 0x01)
				{
					TFT_fillRectangle(poX + j * size, poY + f * size + i * 8 * size, size, size, fgcolor);
				}
			}
		}
	}
	//ks0108GotoXY(x + width + 1, y);


    //for (int i = 0; i < FONT_X; i++ ) {
	    //uint8_t temp = pgm_read_byte(&simpleFont[ascii-0x20][i]);
	    //for(uint8_t f = 0; f < 8; f++)
	    //{
		    //if((temp >> f) & 0x01)
		    //{
			    //TFT_fillRectangle(poX + i * size, poY + f * size, size, size, fgcolor);
		    //}
	    //}
    //}
	//
    //for (int i = 0; i < FONT_X; i++ ) 
	//{
	    //uint8_t temp = pgm_read_byte(&simpleFont[c - 0x20][i]);
	    //for(uint8_t f = 0; f < FONT_Y; f++)
	    //{
		    //if((temp >> f) & 0x01)
		    //{
			    //if(DisplayDirect == LEFT2RIGHT)
			    //{
					//TFT_fillRectangle(poX + i * size, poY + f * size, size, size, fgcolor);
				//}
			    //else if(DisplayDirect == DOWN2UP)
			    //{
					//TFT_fillRectangle(poX + f * size, poY - i * size, size, size, fgcolor);
				//}
			    //else if(DisplayDirect == RIGHT2LEFT)
			    //{
					//TFT_fillRectangle(poX - i * size, poY - f * size, size, size, fgcolor);
				//}
			    //else if(DisplayDirect == UP2DOWN)
			    //{
					//TFT_fillRectangle(poX - f * size, poY + i * size, size, size, fgcolor);
				//}
		    //}
	    //}
    //}
}

void TFT_drawString( char *string, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor )
{
    while(*string)
    {
	    {
		    TFT_drawChar(*string, poX, poY, size, fgcolor);
	    }
	    
	    *string++;
	    if(DisplayDirect == LEFT2RIGHT)
	    {
		    if(poX < MAX_X)
		    {
			    poX += 20 * size; // Move cursor right
		    }
	    }
	    else if(DisplayDirect == DOWN2UP)
	    {
		    if(poY > 0)
		    {
			    poY -= 8 * size; // Move cursor right
		    }
	    }
	    else if(DisplayDirect == RIGHT2LEFT)
	    {
		    if(poX > 0)
		    {
			    poX -= 8 * size; // Move cursor right
		    }
	    }
	    else if(DisplayDirect == UP2DOWN)
	    {
		    if(poY < MAX_Y)
		    {
			    poY += 8 * size; // Move cursor right
		    }
	    }
    }
}

void TFT_fillRectangle( uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color )
{
	//TFT_fillScreen(poX, poX + length, poY, poY + width, color);
	for(int i = 0; i < width; i++)
	{
		//if(DisplayDirect == LEFT2RIGHT)
		//{
			//TFT_drawHorizontalLine(poX, poY + i, length, color);
		//}
		//else if (DisplayDirect ==  DOWN2UP)
		//{
			//TFT_drawHorizontalLine(poX, poY - i, length, color);
		//}
		//else if(DisplayDirect == RIGHT2LEFT)
		//{
			//TFT_drawHorizontalLine(poX, poY - i, length, color);
		//}
		//else if(DisplayDirect == UP2DOWN)
		//{
			//TFT_drawHorizontalLine(poX, poY + i, length, color);
		//}
		
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
	{ /* loop */
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
    for(int i = 0; i < length; i++)
    {
		TFT_sendData(color);	
	}
}

void TFT_drawHorizontalLine( uint16_t poX, uint16_t poY, uint16_t length, uint16_t color )
{
    TFT_setCol(poX, poX + length);
    TFT_setPage(poY, poY);
    TFT_sendCMD(0x2c);//Memory Write
    for(int i = 0; i < length; i++)
    {
		TFT_sendData(color);	
	}
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
    if(floatNumber < 0.0) /* floatNumber < 0 */
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

uint8_t ReadFontData(const uint8_t* ptr)
{
	return pgm_read_byte(ptr);
}

void SelectFont(const uint8_t* font, FontCallback callback, uint8_t color)
{
	Font = font;
	FontRead = callback;
	FontColor = color;
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

uint8_t CharWidth(char c)
{
	uint8_t width = 0;
	uint8_t firstChar = FontRead(Font + FONT_FIRST_CHAR);
	uint8_t charCount = FontRead(Font + FONT_CHAR_COUNT);
	
	// read width data
	if(c >= firstChar && c < (firstChar + charCount))
	{
		c -= firstChar;
		width = FontRead(Font + FONT_WIDTH_TABLE + c) + 1;
	}
	
	return width;
}

uint8_t StringWidth(char* str)
{
	uint8_t width = 0;
	
	while(*str != 0)
	{
		width += CharWidth(*str++);
	}
	
	return width;
}
