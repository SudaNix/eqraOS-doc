#include <string.h>
#include <ctype.h>
#include <hal.h>

#include "keyboard.h"

enum KEYBOARD_ENCODER_IO {
	KEYBOARD_ENC_INPUT_BUF	=	0x60,
	KEYBOARD_ENC_CMD_REG	=	0x60
};

enum KEYBOARD_ENCODER_CMDS {
	KEYBOARD_ENC_CMD_SET_LED				=	0xED,
	KEYBOARD_ENC_CMD_ECHO					=	0xEE,
	KEYBOARD_ENC_CMD_SCAN_CODE_SET			=	0xF0,
	KEYBOARD_ENC_CMD_ID						=	0xF2,
	KEYBOARD_ENC_CMD_AUTODELAY				=	0xF3,
	KEYBOARD_ENC_CMD_ENABLE					=	0xF4,
	KEYBOARD_ENC_CMD_RESETWAIT				=	0xF5,
	KEYBOARD_ENC_CMD_RESETSCAN				=	0xF6,
	KEYBOARD_ENC_CMD_ALL_AUTO				=	0xF7,
	KEYBOARD_ENC_CMD_ALL_MAKEBREAK			=	0xF8,
	KEYBOARD_ENC_CMD_ALL_MAKEONLY			=	0xF9,
	KEYBOARD_ENC_CMD_ALL_MAKEBREAK_AUTO		=	0xFA,
	KEYBOARD_ENC_CMD_SINGLE_AUTOREPEAT		=	0xFB,
	KEYBOARD_ENC_CMD_SINGLE_MAKEBREAK		=	0xFC,
	KEYBOARD_ENC_CMD_SINGLE_BREAKONLY		=	0xFD,
	KEYBOARD_ENC_CMD_RESEND					=	0xFE,
	KEYBOARD_ENC_CMD_RESET					=	0xFF
};

enum KEYBOARD_CONTROLLER_IO {
	KEYBOARD_CTRL_STATS_REG	=	0x64,
	KEYBOARD_CTRL_CMD_REG	=	0x64
};

enum KEYBOARD_CONTROLLER_STATS_MASK {
	KEYBOARD_CTRL_STATS_MASK_OUT_BUF	=	1,		//00000001
	KEYBOARD_CTRL_STATS_MASK_IN_BUF		=	2,		//00000010
	KEYBOARD_CTRL_STATS_MASK_SYSTEM		=	4,		//00000100
	KEYBOARD_CTRL_STATS_MASK_CMD_DATA	=	8,		//00001000
	KEYBOARD_CTRL_STATS_MASK_LOCKED		=	0x10,	//00010000
	KEYBOARD_CTRL_STATS_MASK_AUX_BUF	=	0x20,	//00100000
	KEYBOARD_CTRL_STATS_MASK_TIMEOUT	=	0x40,	//01000000
	KEYBOARD_CTRL_STATS_MASK_PARITY		=	0x80	//10000000
};

enum KEYBOARD_CONTROLLER_CMDS {
	KEYBOARD_CTRL_CMD_READ				=	0x20,
	KEYBOARD_CTRL_CMD_WRITE				=	0x60,
	KEYBOARD_CTRL_CMD_SELF_TEST			=	0xAA,
	KEYBOARD_CTRL_CMD_INTERFACE_TEST	=	0xAB,
	KEYBOARD_CTRL_CMD_DISABLE			=	0xAD,
	KEYBOARD_CTRL_CMD_ENABLE			=	0xAE,
	KEYBOARD_CTRL_CMD_READ_IN_PORT		=	0xC0,
	KEYBOARD_CTRL_CMD_READ_OUT_PORT		=	0xD0,
	KEYBOARD_CTRL_CMD_WRITE_OUT_PORT	=	0xD1,
	KEYBOARD_CTRL_CMD_READ_TEST_INPUTS	=	0xE0,
	KEYBOARD_CTRL_CMD_SYSTEM_RESET		=	0xFE,
	KEYBOARD_CTRL_CMD_MOUSE_DISABLE		=	0xA7,
	KEYBOARD_CTRL_CMD_MOUSE_ENABLE		=	0xA8,
	KEYBOARD_CTRL_CMD_MOUSE_PORT_TEST	=	0xA9,
	KEYBOARD_CTRL_CMD_MOUSE_WRITE		=	0xD4
};

enum KEYBOARD_ERROR {
	KEYBOARD_ERR_BUF_OVERRUN			=	0,
	KEYBOARD_ERR_ID_RET					=	0x83AB,
	KEYBOARD_ERR_BAT					=	0xAA,	//note: can also be L. shift key make code
	KEYBOARD_ERR_ECHO_RET				=	0xEE,
	KEYBOARD_ERR_ACK					=	0xFA,
	KEYBOARD_ERR_BAT_FAILED				=	0xFC,
	KEYBOARD_ERR_DIAG_FAILED			=	0xFD,
	KEYBOARD_ERR_RESEND_CMD				=	0xFE,
	KEYBOARD_ERR_KEY					=	0xFF
};

static char _scancode;							// current scan code
static bool _numlock, _scrolllock, _capslock;	// lock keys
static bool _shift, _alt, _ctrl;			// shift, alt, and ctrl keys current state
static int _keyboard_error = 0;				// set if keyboard error
static bool _keyboard_bat_res = false;		// set if the Basic Assurance Test (BAT) failed
static bool _keyboard_diag_res = false;		// set if diagnostics failed
static bool _keyboard_resend_res = false;	// set if system should resend last command
static bool _keyboard_disable = false;		// set if keyboard is disabled

// original xt scan code set. Array index==make code
static int _keyboard_scancode_std [] = {

	//! key			scancode
	KEY_UNKNOWN,	//0
	KEY_ESCAPE,		//1
	KEY_1,			//2
	KEY_2,			//3
	KEY_3,			//4
	KEY_4,			//5
	KEY_5,			//6
	KEY_6,			//7
	KEY_7,			//8
	KEY_8,			//9
	KEY_9,			//0xa
	KEY_0,			//0xb
	KEY_MINUS,		//0xc
	KEY_EQUAL,		//0xd
	KEY_BACKSPACE,	//0xe
	KEY_TAB,		//0xf
	KEY_Q,			//0x10
	KEY_W,			//0x11
	KEY_E,			//0x12
	KEY_R,			//0x13
	KEY_T,			//0x14
	KEY_Y,			//0x15
	KEY_U,			//0x16
	KEY_I,			//0x17
	KEY_O,			//0x18
	KEY_P,			//0x19
	KEY_LEFTBRACKET,//0x1a
	KEY_RIGHTBRACKET,//0x1b
	KEY_RETURN,		//0x1c
	KEY_LCTRL,		//0x1d
	KEY_A,			//0x1e
	KEY_S,			//0x1f
	KEY_D,			//0x20
	KEY_F,			//0x21
	KEY_G,			//0x22
	KEY_H,			//0x23
	KEY_J,			//0x24
	KEY_K,			//0x25
	KEY_L,			//0x26
	KEY_SEMICOLON,	//0x27
	KEY_QUOTE,		//0x28
	KEY_GRAVE,		//0x29
	KEY_LSHIFT,		//0x2a
	KEY_BACKSLASH,	//0x2b
	KEY_Z,			//0x2c
	KEY_X,			//0x2d
	KEY_C,			//0x2e
	KEY_V,			//0x2f
	KEY_B,			//0x30
	KEY_N,			//0x31
	KEY_M,			//0x32
	KEY_COMMA,		//0x33
	KEY_DOT,		//0x34
	KEY_SLASH,		//0x35
	KEY_RSHIFT,		//0x36
	KEY_KP_ASTERISK,//0x37
	KEY_RALT,		//0x38
	KEY_SPACE,		//0x39
	KEY_CAPSLOCK,	//0x3a
	KEY_F1,			//0x3b
	KEY_F2,			//0x3c
	KEY_F3,			//0x3d
	KEY_F4,			//0x3e
	KEY_F5,			//0x3f
	KEY_F6,			//0x40
	KEY_F7,			//0x41
	KEY_F8,			//0x42
	KEY_F9,			//0x43
	KEY_F10,		//0x44
	KEY_KP_NUMLOCK,	//0x45
	KEY_SCROLLLOCK,	//0x46
	KEY_HOME,		//0x47
	KEY_KP_8,		//0x48	//keypad up arrow
	KEY_PAGEUP,		//0x49
	KEY_KP_2,		//0x50	//keypad down arrow
	KEY_KP_3,		//0x51	//keypad page down
	KEY_KP_0,		//0x52	//keypad insert key
	KEY_KP_DECIMAL,	//0x53	//keypad delete key
	KEY_UNKNOWN,	//0x54
	KEY_UNKNOWN,	//0x55
	KEY_UNKNOWN,	//0x56
	KEY_F11,		//0x57
	KEY_F12			//0x58
};

// invalid scan code. Used to indicate the last scan code is not to be reused
const int INVALID_SCANCODE = 0;


uint8_t	keyboard_ctrl_read_status ();
void keyboard_ctrl_send_cmd (uint8_t);
uint8_t	keyboard_enc_read_buf ();
void keyboard_enc_send_cmd (uint8_t);
void _cdecl i386_keyboard_irq ();


// read status from keyboard controller
uint8_t keyboard_ctrl_read_status () {

	return inportb (KEYBOARD_CTRL_STATS_REG);
}

// send command byte to keyboard controller
void keyboard_ctrl_send_cmd (uint8_t cmd) {

	// wait for keyboard controller input buffer to be clear
	while (1)
		if ( (keyboard_ctrl_read_status () & KEYBOARD_CTRL_STATS_MASK_IN_BUF) == 0)
			break;

	outportb (KEYBOARD_CTRL_CMD_REG, cmd);
}

// read keyboard encoder buffer
uint8_t keyboard_enc_read_buf () {

	return inportb (KEYBOARD_ENC_INPUT_BUF);
}

// send command byte to keyboard encoder
void keyboard_enc_send_cmd (uint8_t cmd) {

	// wait for keyboard controller input buffer to be clear
	while (1)
		if ( (keyboard_ctrl_read_status () & KEYBOARD_CTRL_STATS_MASK_IN_BUF) == 0)
			break;

	// send command byte to keyboard encoder
	outportb (KEYBOARD_ENC_CMD_REG, cmd);
}

//	keyboard interrupt handler
void _cdecl i386_keyboard_irq () {

	_asm add esp, 12
	_asm pushad
	_asm cli

	static bool _extended = false;

	int code = 0;

	// read scan code only if the keyboard controller output buffer is full (scan code is in it)
	if (keyboard_ctrl_read_status () & KEYBOARD_CTRL_STATS_MASK_OUT_BUF) {

		// read the scan code
		code = keyboard_enc_read_buf ();

		// is this an extended code? If so, set it and return
		if (code == 0xE0 || code == 0xE1)
			_extended = true;
		else {

			// either the second byte of an extended scan code or a single byte scan code
			_extended = false;

			// test if this is a break code (Original XT Scan Code Set specific)
			if (code & 0x80) {	//test bit 7

				// covert the break code into its make code equivelant
				code -= 0x80;

				// grab the key
				int key = _keyboard_scancode_std [code];

				// test if a special key has been released & set it
				switch (key) {

					case KEY_LCTRL:
					case KEY_RCTRL:
						_ctrl = false;
						break;

					case KEY_LSHIFT:
					case KEY_RSHIFT:
						_shift = false;
						break;

					case KEY_LALT:
					case KEY_RALT:
						_alt = false;
						break;
				}
			}
			else {

				// this is a make code - set the scan code
				_scancode = code;

				// grab the key
				int key = _keyboard_scancode_std [code];

				// test if user is holding down any special keys & set it
				switch (key) {

					case KEY_LCTRL:
					case KEY_RCTRL:
						_ctrl = true;
						break;

					case KEY_LSHIFT:
					case KEY_RSHIFT:
						_shift = true;
						break;

					case KEY_LALT:
					case KEY_RALT:
						_alt = true;
						break;

					case KEY_CAPSLOCK:
						_capslock = (_capslock) ? false : true;
						keyboard_set_leds (_numlock, _capslock, _scrolllock);
						break;

					case KEY_KP_NUMLOCK:
						_numlock = (_numlock) ? false : true;
						keyboard_set_leds (_numlock, _capslock, _scrolllock);
						break;

					case KEY_SCROLLLOCK:
						_scrolllock = (_scrolllock) ? false : true;
						keyboard_set_leds (_numlock, _capslock, _scrolllock);
						break;
				}
			}
		}

		// watch for errors
		switch (code) {

			case KEYBOARD_ERR_BAT_FAILED:
				_keyboard_bat_res = false;
				break;

			case KEYBOARD_ERR_DIAG_FAILED:
				_keyboard_diag_res = false;
				break;

			case KEYBOARD_ERR_RESEND_CMD:
				_keyboard_resend_res = true;
				break;
		}
	}

	// tell hal we are done
	int_done(0);

	// return from interrupt handler
	_asm sti
	_asm popad
	_asm iretd
}

bool keyboard_get_scroll_lock() {
	return _scrolllock;
}

bool keyboard_get_num_lock() {
	return _numlock;
}

bool keyboard_get_caps_lock() {
	return _capslock;
}

bool keyboard_get_ctrl() {
	return _ctrl;
}

bool keyboard_get_alt() {
	return _alt;
}

bool keyboard_get_shift() {
	return _shift;
}

void keyboard_ignore_resend() {
	_keyboard_resend_res = false;
}

bool keyboard_check_resend() {
	return _keyboard_resend_res;
}

bool keyboard_get_diagnostic_res() {
	return _keyboard_diag_res;
}

bool keyboard_get_bat_res() {
	return _keyboard_bat_res;
}

uint8_t keyboard_get_last_scan() {
	return _scancode;
}


bool keyboard_self_test() {
	keyboard_ctrl_send_cmd (KEYBOARD_CTRL_CMD_SELF_TEST);

	// wait for output buffer to be full
	while (1)
		if (keyboard_ctrl_read_status () & KEYBOARD_CTRL_STATS_MASK_OUT_BUF)
			break;

	// if output buffer == 0x55, test passed
	return (keyboard_enc_read_buf () == 0x55) ? true : false;
}

KEY_CODE keyboard_get_last_key() {
	return (_scancode != INVALID_SCANCODE)?((KEY_CODE)_keyboard_scancode_std[_scancode]):KEY_UNKNOWN;
}

void keyboard_discard_last_key() {
	_scancode = INVALID_SCANCODE;
}

void keyboard_set_leds(bool num,bool caps,bool scroll) {
	uint8_t data = 0;
	data = (scroll) ? (data | 1) : (data & 1);
	data = (num) ? (num | 2) : (num & 2);
	data = (caps) ? (num | 4) : (num & 4);

	keyboard_enc_send_cmd (KEYBOARD_ENC_CMD_SET_LED);
	keyboard_enc_send_cmd (data);
}

char keyboard_key_to_ascii(KEY_CODE code) {
	uint8_t key = code;
	
	if (isascii(key)) {
		// if shift key is down or caps lock is on, make the key uppercase
		if (_shift || _capslock)
			if (key >= 'a' && key <= 'z')
				key -= 32;
			
		if (_shift && !_capslock) {
			if (key >= '0' && key <= '9')
				switch (key) {
					case '0':
						key = KEY_RIGHTPARENTHESIS;
						break;
					case '1':
						key = KEY_EXCLAMATION;
						break;
					case '2':
						key = KEY_AT;
						break;
					case '3':
						key = KEY_EXCLAMATION;
						break;
					case '4':
						key = KEY_HASH;
						break;
					case '5':
						key = KEY_PERCENT;
						break;
					case '6':
						key = KEY_CARRET;
						break;
					case '7':
						key = KEY_AMPERSAND;
						break;
					case '8':
						key = KEY_ASTERISK;
						break;
					case '9':
						key = KEY_LEFTPARENTHESIS;
						break;
				}
				
			else {
				switch (key) {
					case KEY_COMMA:
						key = KEY_LESS;
						break;

					case KEY_DOT:
						key = KEY_GREATER;
						break;

					case KEY_SLASH:
						key = KEY_QUESTION;
						break;

					case KEY_SEMICOLON:
						key = KEY_COLON;
						break;

					case KEY_QUOTE:
						key = KEY_QUOTEDOUBLE;
						break;

					case KEY_LEFTBRACKET :
						key = KEY_LEFTCURL;
						break;

					case KEY_RIGHTBRACKET :
						key = KEY_RIGHTCURL;
						break;

					case KEY_GRAVE:
						key = KEY_TILDE;
						break;

					case KEY_MINUS:
						key = KEY_UNDERSCORE;
						break;

					case KEY_PLUS:
						key = KEY_EQUAL;
						break;

					case KEY_BACKSLASH:
						key = KEY_BAR;
						break;
				}
			}
		}
		
		return key;
	}
	
	return 0;
}

void keyboard_enable() {
	keyboard_ctrl_send_cmd(KEYBOARD_CTRL_CMD_ENABLE);
	_keyboard_disable = false;
}

void keyboard_disable() {
	keyboard_ctrl_send_cmd(KEYBOARD_CTRL_CMD_DISABLE);
	_keyboard_disable = true;
}

bool keyboard_is_disabled() {
	return _keyboard_disable;
}

void keyboard_reset_system() {
	keyboard_ctrl_send_cmd (KEYBOARD_CTRL_CMD_WRITE_OUT_PORT);
	keyboard_enc_send_cmd (0xfe);
}

void keyboard_install(int irq) {
	// Install  interrupt handler (irq 1 uses interrupt 33)
	set_vector(irq, i386_keyboard_irq);

	// assume BAT test is good. If there is a problem, the IRQ handler where catch the error
	_keyboard_bat_res = true;
	_scancode = 0;

	// set lock keys and led lights
	_numlock = _scrolllock = _capslock = false;
	keyboard_set_leds (false, false, false);

	// shift, ctrl, and alt keys
	_shift = _alt = _ctrl = false;
}