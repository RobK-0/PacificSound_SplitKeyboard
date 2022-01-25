


def keyCodeDict():
    keyDict = {}

    '''
     * Modifier masks - used for the first byte in the HID report.
     * The modifiers are combined in the first byte:
     * LCTL | LSFT | LALT | RCTL | RSFT | RALT
     * NOTE: The second byte in the report is reserved, 0x00
    '''
    keyDict["KC_LCTL"]="0x01"
    keyDict["KC_LSFT"]="0x02"
    keyDict["KC_LALT"]="0x04"
    keyDict["KC_LGUI"]="0x08"
    keyDict["KC_RCTL"]="0x10"
    keyDict["KC_RSFT"]="0x20"
    keyDict["KC_RALT"]="0x40"
    keyDict["KC_RGUI"]="0x80"


    '''
     * Scan codes - last N slots in the HID report (usually 6).
     * 0x00 if no key pressed.
     *
     * If more than N keys are pressed, the HID reports
     * KEY_ERR_OVF in all slots to indicate this condition.
    '''

    keyDict["KC_NO"]="0x00" # No key pressed
    keyDict["KCERR_OVF"]="0x01" #  Keyboard Error Roll Over - used for all slots if too many keys are pressed ("Phantom key")
    # 0x02 #  Keyboard POST Fail
    # 0x03 #  Keyboard Error Undefined
    keyDict["KC_A"]="0x04" # Keyboard a and A
    keyDict["KC_B"]="0x05" # Keyboard b and B
    keyDict["KC_C"]="0x06" # Keyboard c and C
    keyDict["KC_D"]="0x07" # Keyboard d and D
    keyDict["KC_E"]="0x08" # Keyboard e and E
    keyDict["KC_F"]="0x09" # Keyboard f and F
    keyDict["KC_G"]="0x0a" # Keyboard g and G
    keyDict["KC_H"]="0x0b" # Keyboard h and H
    keyDict["KC_I"]="0x0c" # Keyboard i and I
    keyDict["KC_J"]="0x0d" # Keyboard j and J
    keyDict["KC_K"]="0x0e" # Keyboard k and K
    keyDict["KC_L"]="0x0f" # Keyboard l and L
    keyDict["KC_M"]="0x10" # Keyboard m and M
    keyDict["KC_N"]="0x11" # Keyboard n and N
    keyDict["KC_O"]="0x12" # Keyboard o and O
    keyDict["KC_P"]="0x13" # Keyboard p and P
    keyDict["KC_Q"]="0x14" # Keyboard q and Q
    keyDict["KC_R"]="0x15" # Keyboard r and R
    keyDict["KC_S"]="0x16" # Keyboard s and S
    keyDict["KC_T"]="0x17" # Keyboard t and T
    keyDict["KC_U"]="0x18" # Keyboard u and U
    keyDict["KC_V"]="0x19" # Keyboard v and V
    keyDict["KC_W"]="0x1a" # Keyboard w and W
    keyDict["KC_X"]="0x1b" # Keyboard x and X
    keyDict["KC_Y"]="0x1c" # Keyboard y and Y
    keyDict["KC_Z"]="0x1d" # Keyboard z and Z

    keyDict["KC_1"]="0x1e" # Keyboard 1 and !
    keyDict["KC_2"]="0x1f" # Keyboard 2 and @
    keyDict["KC_3"]="0x20" # Keyboard 3 and #
    keyDict["KC_4"]="0x21" # Keyboard 4 and $
    keyDict["KC_5"]="0x22" # Keyboard 5 and %
    keyDict["KC_6"]="0x23" # Keyboard 6 and ^
    keyDict["KC_7"]="0x24" # Keyboard 7 and &
    keyDict["KC_8"]="0x25" # Keyboard 8 and *
    keyDict["KC_9"]="0x26" # Keyboard 9 and (
    keyDict["KC_0"]="0x27" # Keyboard 0 and )

    keyDict["KC_ENT"]="0x28" # Keyboard Return (ENTER)
    keyDict["KC_ESC"]="0x29" # Keyboard ESCAPE
    keyDict["KC_BSPC"]="0x2a" # Keyboard DELETE (Backspace)
    keyDict["KC_TAB"]="0x2b" # Keyboard Tab
    keyDict["KC_SPC"]="0x2c" # Keyboard Spacebar
    keyDict["KC_MINS"]="0x2d" # Keyboard - and _
    keyDict["KC_EQL"]="0x2e" # Keyboard = and +
    keyDict["KC_LBRC"]="0x2f" # Keyboard [ and {
    keyDict["KC_RBRC"]="0x30" # Keyboard ] and }
    keyDict["KC_BSLS"]="0x31" # Keyboard \ and |
    keyDict["KC_NUHS"]="0x32" # Keyboard Non-US # and ~
    keyDict["KC_SCLN"]="0x33" # Keyboard ; and :
    keyDict["KC_QUOT"]="0x34" # Keyboard ' and "
    keyDict["KC_GRV"]="0x35" # Keyboard ` and ~
    keyDict["KC_COMM"]="0x36" # Keyboard , and <
    keyDict["KC_DOT"]="0x37" # Keyboard . and >
    keyDict["KC_SLSH"]="0x38" # Keyboard / and ?
    keyDict["KC_CAPS"]="0x39" # Keyboard Caps Lock

    keyDict["KC_F1"]="0x3a" # Keyboard F1
    keyDict["KC_F2"]="0x3b" # Keyboard F2
    keyDict["KC_F3"]="0x3c" # Keyboard F3
    keyDict["KC_F4"]="0x3d" # Keyboard F4
    keyDict["KC_F5"]="0x3e" # Keyboard F5
    keyDict["KC_F6"]="0x3f" # Keyboard F6
    keyDict["KC_F7"]="0x40" # Keyboard F7
    keyDict["KC_F8"]="0x41" # Keyboard F8
    keyDict["KC_F9"]="0x42" # Keyboard F9
    keyDict["KC_F10"]="0x43" # Keyboard F10
    keyDict["KC_F11"]="0x44" # Keyboard F11
    keyDict["KC_F12"]="0x45" # Keyboard F12

    keyDict["KC_PSCR"]="0x46" # Keyboard Print Screen
    keyDict["KC_SLCK"]="0x47" # Keyboard Scroll Lock
    keyDict["KC_PAUS"]="0x48" # Keyboard Pause
    keyDict["KC_INS"]="0x49" # Keyboard Insert
    keyDict["KC_HOME"]="0x4a" # Keyboard Home
    keyDict["KC_PGUP"]="0x4b" # Keyboard Page Up
    keyDict["KC_DEL"]="0x4c" # Keyboard Delete Forward
    keyDict["KC_END"]="0x4d" # Keyboard End
    keyDict["KC_PGDN"]="0x4e" # Keyboard Page Down
    keyDict["KC_RGHT"]="0x4f" # Keyboard Right Arrow
    keyDict["KC_LEFT"]="0x50" # Keyboard Left Arrow
    keyDict["KC_DOWN"]="0x51" # Keyboard Down Arrow
    keyDict["KC_UP"]="0x52" # Keyboard Up Arrow

    keyDict["KC_NLCK"]="0x53" # Keyboard Num Lock and Clear
    keyDict["KC_PSLS"]="0x54" # Keypad /
    keyDict["KC_PAST"]="0x55" # Keypad *
    keyDict["KC_PMNS"]="0x56" # Keypad -
    keyDict["KC_PPLS"]="0x57" # Keypad +
    keyDict["KC_PENT"]="0x58" # Keypad ENTER
    keyDict["KC_P1"]="0x59" # Keypad 1 and End
    keyDict["KC_P2"]="0x5a" # Keypad 2 and Down Arrow
    keyDict["KC_P3"]="0x5b" # Keypad 3 and PageDn
    keyDict["KC_P4"]="0x5c" # Keypad 4 and Left Arrow
    keyDict["KC_P5"]="0x5d" # Keypad 5
    keyDict["KC_P6"]="0x5e" # Keypad 6 and Right Arrow
    keyDict["KC_P7"]="0x5f" # Keypad 7 and Home
    keyDict["KC_P8"]="0x60" # Keypad 8 and Up Arrow
    keyDict["KC_P9"]="0x61" # Keypad 9 and Page Up
    keyDict["KC_P0"]="0x62" # Keypad 0 and Insert
    keyDict["KC_PDOT"]="0x63" # Keypad . and Delete

    keyDict["KC_NUBS"]="0x64" # Keyboard Non-US \ and |
    #keyDict["KC_COMPOSE"]="0x65" # Keyboard Application
    #keyDict["KC_POWER"]="0x66" # Keyboard Power
    keyDict["KC_PEQL"]="0x67" # Keypad =

    keyDict["KC_F13"]="0x68" # Keyboard F13
    keyDict["KC_F14"]="0x69" # Keyboard F14
    keyDict["KC_F15"]="0x6a" # Keyboard F15
    keyDict["KC_F16"]="0x6b" # Keyboard F16
    keyDict["KC_F17"]="0x6c" # Keyboard F17
    keyDict["KC_F18"]="0x6d" # Keyboard F18
    keyDict["KC_F19"]="0x6e" # Keyboard F19
    keyDict["KC_F20"]="0x6f" # Keyboard F20
    keyDict["KC_F21"]="0x70" # Keyboard F21
    keyDict["KC_F22"]="0x71" # Keyboard F22
    keyDict["KC_F23"]="0x72" # Keyboard F23
    keyDict["KC_F24"]="0x73" # Keyboard F24

    keyDict["KC_EXEC"]="0x74" # Keyboard Execute
    keyDict["KC_HELP"]="0x75" # Keyboard Help
    keyDict["KC_MENU"]="0x76" # Keyboard Menu
    keyDict["KC_SLCT"]="0x77" # Keyboard Select
    keyDict["KC_STOP"]="0x78" # Keyboard Stop
    keyDict["KC_AGIN"]="0x79" # Keyboard Again
    keyDict["KC_UNDO"]="0x7a" # Keyboard Undo
    keyDict["KC_CUT"]="0x7b" # Keyboard Cut
    keyDict["KC_COPY"]="0x7c" # Keyboard Copy
    keyDict["KC_PSTE"]="0x7d" # Keyboard Paste
    keyDict["KC_FIND"]="0x7e" # Keyboard Find
    keyDict["KC_MUTE"]="0x7f" # Keyboard Mute
    keyDict["KC_VOLU"]="0x80" # Keyboard Volume Up
    keyDict["KC_VOLD"]="0x81" # Keyboard Volume Down
    # 0x82  Keyboard Locking Caps Lock
    # 0x83  Keyboard Locking Num Lock
    # 0x84  Keyboard Locking Scroll Lock
    keyDict["KC_PCMM"]="0x85" # Keypad Comma



    #define KEY_MEDIA_PLAYPAUSE 0xe8
    #define KEY_MEDIA_STOPCD 0xe9
    #define KEY_MEDIA_PREVIOUSSONG 0xea
    #define KEY_MEDIA_NEXTSONG 0xeb
    #define KEY_MEDIA_EJECTCD 0xec
    #define KEY_MEDIA_VOLUMEUP 0xed
    #define KEY_MEDIA_VOLUMEDOWN 0xee
    #define KEY_MEDIA_MUTE 0xef
    #define KEY_MEDIA_WWW 0xf0
    #define KEY_MEDIA_BACK 0xf1
    #define KEY_MEDIA_FORWARD 0xf2
    #define KEY_MEDIA_STOP 0xf3
    #define KEY_MEDIA_FIND 0xf4
    #define KEY_MEDIA_SCROLLUP 0xf5
    #define KEY_MEDIA_SCROLLDOWN 0xf6
    #define KEY_MEDIA_EDIT 0xf7
    #define KEY_MEDIA_SLEEP 0xf8
    #define KEY_MEDIA_COFFEE 0xf9
    #define KEY_MEDIA_REFRESH 0xfa
    #define KEY_MEDIA_CALC 0xfb


    return keyDict
