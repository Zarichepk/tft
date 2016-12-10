
#include <Arduino.h>


#define WHITE 0x0000
#define RED 0xFFC0
#define GREEN 0xF81F
#define BLUE 0x07FF
#define CYAN 0x003F
#define MAGENTA 0x07C0
#define YELLOW 0xF800
#define BLACK 0xFFFF


#define TFTWIDTH   240
#define TFTHEIGHT  320

#define LCD_RD   A0 
#define LCD_WR   A1 
#define LCD_RS   A2 
#define LCD_CS   A3 
#define LCD_RST  A4 

//-------------------------------------------------------------------------------------------
void Lcd_Writ_Bus(unsigned char d)
{
	
	PORTD = (PORTD & B00000011) | ((d) & B11111100);
	
	PORTB = (PORTB & B11111100) | ((d) & B00000011);

	
	*(portOutputRegister(digitalPinToPort(LCD_WR))) &=  ~digitalPinToBitMask(LCD_WR);
	*(portOutputRegister(digitalPinToPort(LCD_WR))) |=  digitalPinToBitMask(LCD_WR); 
}

//-------------------------------------------------------------------------------------------
void Lcd_Write_Com(unsigned char VH)
{
	*(portOutputRegister(digitalPinToPort(LCD_RS))) &=  ~digitalPinToBitMask(LCD_RS);
	Lcd_Writ_Bus(VH);
}

//-------------------------------------------------------------------------------------------
void Lcd_Write_Data(unsigned char VH)
{
	*(portOutputRegister(digitalPinToPort(LCD_RS))) |=  digitalPinToBitMask(LCD_RS); 
	Lcd_Writ_Bus(VH);
}
//-------------------------------------------------------------------------------------------
void Lcd_Write_Com_Data(unsigned char com, unsigned char dat)
{
	Lcd_Write_Com(com);
	Lcd_Write_Data(dat);
}
//-------------------------------------------------------------------------------------------
void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	Lcd_Write_Com(0x2a);
	Lcd_Write_Data(x1 >> 8); 
	Lcd_Write_Data(x1);
	Lcd_Write_Data(x2 >> 8);
	Lcd_Write_Data(x2);

	Lcd_Write_Com(0x2b);
	Lcd_Write_Data(y1 >> 8); 
	Lcd_Write_Data(y1);
	Lcd_Write_Data(y2 >> 8);
	Lcd_Write_Data(y2);

	Lcd_Write_Com(0x2c); 
}

//-------------------------------------------------------------------------------------------

void Lcd_Init(void)
{
	digitalWrite(LCD_RST, HIGH); 
	delay(5); 
	digitalWrite(LCD_RST, LOW); 
	delay(15);
	digitalWrite(LCD_RST, HIGH); 
	delay(15);

	digitalWrite(LCD_CS, HIGH); 
	digitalWrite(LCD_WR, HIGH); 
	digitalWrite(LCD_CS, LOW); 

	Lcd_Write_Com(0xC0);    
	Lcd_Write_Data(0x23);   

	Lcd_Write_Com(0xC1);    
	Lcd_Write_Data(0x10);   

	Lcd_Write_Com(0xC5);    
	Lcd_Write_Data(0x3e);   
	Lcd_Write_Data(0x28);  

	Lcd_Write_Com(0xC7);    
	Lcd_Write_Data(0x86);   

	Lcd_Write_Com(0x36);    
	Lcd_Write_Data(0x40);  

	Lcd_Write_Com(0x3A);   
	Lcd_Write_Data(0x55);   

	Lcd_Write_Com(0xB1);    
	Lcd_Write_Data(0x00);   
	Lcd_Write_Data(0x18);   

	Lcd_Write_Com(0xB6);    
	Lcd_Write_Data(0x08);  
	
	Lcd_Write_Data(0x82);   
	Lcd_Write_Data(0x27);   

	Lcd_Write_Com(0x11);    
	delay(120);             

	Lcd_Write_Com(0x29);  

	Lcd_Write_Com(0x2c);   
}
//-------------------------------------------------------------------------------------------
void invertColor()
{
	Lcd_Write_Com(0x21);
}
//-------------------------------------------------------------------------------------------
void normalColor()
{
	Lcd_Write_Com(0x20); 
}
//-------------------------------------------------------------------------------------------
void blink(int Times, int DL)
{
	for (int t = 1; t <= Times; ++t)
	{
		Lcd_Write_Com(0x21); 
		delay(DL);          
		Lcd_Write_Com(0x20); 
		delay(DL);           
	}
}
//-------------------------------------------------------------------------------------------
void drawCircle(int x, int y, int radius, unsigned color)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;

	digitalWrite(LCD_CS, LOW);
	Address_set(x, y + radius, x, y + radius);
	Lcd_Write_Com(0x2C);
	Lcd_Write_Data(color >> 8);
	Lcd_Write_Data(color);
	Address_set(x, y - radius, x, y - radius);
	Lcd_Write_Com(0x2C);
	Lcd_Write_Data(color >> 8);
	Lcd_Write_Data(color);
	Address_set(x + radius, y, x + radius, y);
	Lcd_Write_Com(0x2C);
	Lcd_Write_Data(color >> 8);
	Lcd_Write_Data(color);
	Address_set(x - radius, y, x - radius, y);
	Lcd_Write_Com(0x2C);
	Lcd_Write_Data(color >> 8);
	Lcd_Write_Data(color);

	while (x1 < y1)
	{
		if (f >= 0)
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;
		Address_set(x + x1, y + y1, x + x1, y + y1);
		Lcd_Write_Com(0x2C);
		Lcd_Write_Data(color >> 8);
		Lcd_Write_Data(color);
		Address_set(x - x1, y + y1, x - x1, y + y1);
		Lcd_Write_Com(0x2C);
		Lcd_Write_Data(color >> 8);
		Lcd_Write_Data(color);
		Address_set(x + x1, y - y1, x + x1, y - y1);
		Lcd_Write_Com(0x2C);
		Lcd_Write_Data(color >> 8);
		Lcd_Write_Data(color);
		Address_set(x - x1, y - y1, x - x1, y - y1);
		Lcd_Write_Com(0x2C);
		Lcd_Write_Data(color >> 8);
		Lcd_Write_Data(color);
		Address_set(x + y1, y + x1, x + y1, y + x1);
		Lcd_Write_Com(0x2C);
		Lcd_Write_Data(color >> 8);
		Lcd_Write_Data(color);
		Address_set(x - y1, y + x1, x - y1, y + x1);
		Lcd_Write_Com(0x2C);
		Lcd_Write_Data(color >> 8);
		Lcd_Write_Data(color);
		Address_set(x + y1, y - x1, x + y1, y - x1);
		Lcd_Write_Com(0x2C);
		Lcd_Write_Data(color >> 8);
		Lcd_Write_Data(color);
		Address_set(x - y1, y - x1, x - y1, y - x1);
		Lcd_Write_Com(0x2C);
		Lcd_Write_Data(color >> 8);
		Lcd_Write_Data(color);
	}
}
//-------------------------------------------------------------------------------------------
void drawPixel(unsigned int x, unsigned int y, unsigned int color)
{
	
	if ((x < 0) || (y < 0) || (x >= TFTWIDTH) || (y >= TFTHEIGHT)) return;

	digitalWrite(LCD_CS, LOW);
	Address_set(x, y, TFTWIDTH - 1, TFTHEIGHT - 1);
	digitalWrite(LCD_CS, LOW);
	Lcd_Write_Com(0x2C);
	Lcd_Write_Data(color >> 8);
	Lcd_Write_Data(color);
}
//-------------------------------------------------------------------------------------------
void drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color)
{
	unsigned int  dx = (x2 > x1 ? x2 - x1 : x1 - x2);
	short     xstep =  x2 > x1 ? 1 : -1;
	unsigned int  dy = (y2 > y1 ? y2 - y1 : y1 - y2);
	short     ystep =  y2 > y1 ? 1 : -1;
	int       col = x1, row = y1;

	digitalWrite(LCD_CS, LOW);
	if (dx < dy)
	{
		int t = - (dy >> 1);
		while (true)
		{
			Address_set (col, row, col, row);
			Lcd_Write_Com(0x2C);
			Lcd_Write_Data(color >> 8);
			Lcd_Write_Data(color);
			if (row == y2)
			return;
			row += ystep;
			t += dx;
			if (t >= 0)
			{
				col += xstep;
				t   -= dy;
			}
		}
	}
	else
	{
		int t = - (dx >> 1);
		while (true)
		{
			Address_set (col, row, col, row);
			Lcd_Write_Com(0x2C);
			Lcd_Write_Data(color >> 8);
			Lcd_Write_Data(color);
			if (col == x2)
			return;
			col += xstep;
			t += dy;
			if (t >= 0)
			{
				row += ystep;
				t   -= dx;
			}
		}
	}
}
//-------------------------------------------------------------------------------------------
void drawTriangle(unsigned x0, unsigned y0, unsigned x1, unsigned y1, unsigned x2, unsigned y2, uint16_t color)
{
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

//-------------------------------------------------------------------------------------------
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
	unsigned int i, j;
	Lcd_Write_Com(0x02c);
	digitalWrite(LCD_RS, HIGH);
	digitalWrite(LCD_CS, LOW);
	l = l + x;
	Address_set(x, y, l, y);
	j = l * 2;
	for (i = 1; i <= j; i++)
	{
		Lcd_Write_Data(c);
	}
}
//-------------------------------------------------------------------------------------------
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
	unsigned int i, j;
	Lcd_Write_Com(0x02c); 
	digitalWrite(LCD_RS, HIGH);
	digitalWrite(LCD_CS, LOW);
	l = l + y;
	Address_set(x, y, x, l);
	j = l * 2;
	for (i = 1; i <= j; i++)
	{
		Lcd_Write_Data(c);
	}
}
//-------------------------------------------------------------------------------------------

void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
{
	H_line(x  , y  , w, c);
	H_line(x  , y + h, w, c);
	V_line(x  , y  , h, c);
	V_line(x + w, y  , h, c);
}
//-------------------------------------------------------------------------------------------
void Rectf(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
{
	unsigned int i;
	for (i = 0; i < h; i++)
	{
		H_line(x  , y  , w, c);
		H_line(x  , y + i, w, c);
	}
}
//-------------------------------------------------------------------------------------------
int RGB(int r, int g, int b)
{
	return r << 16 | g << 8 | b;
}
//-------------------------------------------------------------------------------------------
void LCD_Clear(unsigned int j)
{
	unsigned int i, m;
	Address_set(0, 0, 239, 319);
	digitalWrite(LCD_CS, LOW);
	for (i = 0; i < 240; i++)
	for (m = 0; m < 320; m++)
	{
		Lcd_Write_Data(j >> 8);
		Lcd_Write_Data(j);
	}
}
//-------------------------------------------------------------------------------------------
void setup()
{
	for (int p = 2; p < 10; p++)
	{
		pinMode(p, OUTPUT); 
	pinMode(A0, OUTPUT); 
	pinMode(A1, OUTPUT); 
	pinMode(A2, OUTPUT); 
	pinMode(A3, OUTPUT); 
	pinMode(A4, OUTPUT); 
	digitalWrite(A0, HIGH);
	digitalWrite(A1, HIGH);
	digitalWrite(A2, HIGH);
	digitalWrite(A3, HIGH);
	digitalWrite(A4, HIGH);

	Lcd_Init();

	LCD_Clear(BLACK);
}
}
//-------------------------------------------------------------------------------------------
void loop()
{
	
	LCD_Clear(WHITE);
	delay(1000);
	invertColor();
	delay(1000);
	normalColor();
	LCD_Clear(WHITE);
	blink(1, 200);
	LCD_Clear(RED);
	blink(1, 200);
	LCD_Clear(GREEN);
	blink(1, 200);
	LCD_Clear(BLUE);
	blink(1, 200);
	LCD_Clear(YELLOW);
	blink(1, 200);
	LCD_Clear(MAGENTA);
	blink(1, 200);
	LCD_Clear(CYAN);
	blink(1, 200);
	LCD_Clear(BLACK);
	blink(1, 200);
	LCD_Clear(BLACK);
	
	for (int i = 0; i < 50; i++)
	{
		drawTriangle(random(239), random(319), random(239), random(319), random(239), random(319), random(65535)); 
	}
	delay(2000);
	blink(3, 500);
	
	LCD_Clear(BLACK);
	for (int i = 0; i < 200; i++)
	{
		drawLine(random(240), random(320), random(240), random(320), random(65535)); 
	}
	delay(2000);
	blink(3, 500); 
	LCD_Clear(BLACK);
	
	for (int i = 0; i < 10000; i++)
	{
		drawPixel(random(239), random(319), random(65535)); 
	}
	delay(2000);
	blink(3, 500); 
	LCD_Clear(BLACK);

	for (int i = 0; i < 100; i++)
	{
		Rect(random(120), random(160), random(120), random(160), random(65535)); 
	}
	delay(2000);
	blink(3, 500); 
	LCD_Clear(BLACK);
	
	for (int i = 0; i < 100; i++)
	{
		drawCircle(120, 160, random(120), random(65535)); 
	}
	delay(2000);
	blink(3, 500);
	LCD_Clear(BLACK);
	
	for (int i = 0; i < 30; i++)
	{
		Rectf(random(120), random(160), random(120), random(160), random(65535)); 
	}
	delay(2000);
	blink(3, 500); 
	LCD_Clear(BLACK);
	
}
//-------------------------------------------------------------------------------------------

