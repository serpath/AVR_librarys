/*
 * LCD.h
 *
 * Created: 04.07.2014 19:47:23
 *  Author: serpath
 */ 


#ifndef LCD_H_
#define LCD_H_


void LCD_LCD();
void lcd_write_4(uint8_t);
void lcd_write_instruction_4d(uint8_t);
void lcd_write_character_4d(uint8_t);
void lcd_write_string_4d(uint8_t *);
void lcd_init_4d(void);

#endif /* LCD_H_ */