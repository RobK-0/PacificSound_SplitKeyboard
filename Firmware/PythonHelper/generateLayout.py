
#File for generating the C vectors of keycodes for "Pacific Sound" keyboard

'''
The keyboard does column scanning. Each row is triggered low (every row is normally high). The columns are set
in the STM32F4XX as inputs with a pull-up resistor enabled. This means that when a key is pressed and the row
is pulled down then that column will be pulled low. So that is what is detected as a key press.

So when a row is triggered each column is read in. In the STM32F4XX the data is stored as a vector of
length 22 (22 possible switches). So since the columns are sequentially read this means that the
vector represents the rows with the starting position of each row being the switch closest to the
microcontroller (this is just what was chosen as the seqence when reading the GPIO pins).

To interpret the keys a vector of keycodes is made corresponding to the keymap below.

'''


import keycodes




##########################################################################
# My keymaps
##########################################################################

n_row = 4
n_col = 12
length_check = 22

Layers = {}


#COLEMAK
# KC_NC is interpreted as a not connected key (is skipped)
Layers["0"] = {}

Layers["0"]["keys"] = [["KC_TAB",  "KC_Q",  "KC_W",  "KC_F",  "KC_P",  "KC_B",  "KC_J",  "KC_L",  "KC_U",  "KC_Y",  "KC_SCLN",  "KC_ENT"],
  ["KC_ESC",  "KC_A",  "KC_R",  "KC_S",  "KC_T",  "KC_G",  "KC_M",  "KC_N",  "KC_E",  "KC_I",  "KC_O",  "KC_QUOT"],
  ["KC_NO",  "KC_Z",  "KC_X",  "KC_C",  "KC_D",  "KC_V",  "KC_K",  "KC_H",  "KC_COMM",  "KC_DOT",  "KC_SLSH", "KC_NO" ],
  ["KC_NC",  "KC_NC",  "KC_NO", "KC_NO",  "KC_BSPC", "KC_NO", "KC_NO", "KC_SPC",  "KC_NO", "KC_NO", "KC_NC",  "KC_NC"]]

# Modifiers are kept in a separate vector. So things like ctl, shift, etc are not in the vector of keys. They are in the vector of modifiers
# In the keyboard code for each key pressed the keycode will be added to the USB data and the modifier will be added in with "OR"
Layers["0"]["mod1"] =  [["KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO"],
            ["KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO"],
            ["KC_LSFT", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_RSFT"],
            ["KC_NC", "KC_NC", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NC", "KC_NC"]]




Layers["1"] = {}
# The layer 1 keys have some keys that are meant to be combined with a shift modifier (eg. curly bracket). So instead
# of using the QMK setup for these keys, I'll just use a modifier vector in addition to the layer vector
Layers["1"]["keys"] =  [["KC_NO", "KC_F1", "KC_F2", "KC_F3", "KC_F4", "KC_NO", "KC_NO", "KC_MINS", "KC_EQL", "KC_9", "KC_0", "KC_BSLS"],
                       ["KC_NO", "KC_F5", "KC_F6", "KC_F7", "KC_F8", "KC_NO", "KC_NO", "KC_MINS", "KC_EQL", "KC_LBRC", "KC_RBRC", "KC_BSLS"],
                       ["KC_NO", "KC_F9", "KC_F10", "KC_F11", "KC_F12", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_LBRC", "KC_RBRC", "KC_NO"],
                       ["KC_NC",  "KC_NC",  "KC_NO", "KC_NO",  "KC_BSPC", "KC_NO", "KC_NO", "KC_SPC",  "KC_NO", "KC_NO", "KC_NC",  "KC_NC"]]

Layers["1"]["mod1"] =  [["KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_LSFT", "KC_LSFT", "KC_LSFT", "KC_LSFT", "KC_NO"],
                       ["KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_LSFT"],
                       ["KC_LSFT", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_LSFT", "KC_LSFT", "KC_NO"],
                       ["KC_NC", "KC_NC", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NC", "KC_NC"]]



Layers["2"] = {}

Layers["2"]["keys"] =  [["KC_NO", "KC_NO", "KC_HOME", "KC_UP", "KC_PGUP", "KC_NO", "KC_NO", "KC_HOME", "KC_UP", "KC_PGUP", "KC_NO", "KC_NO"],
                        ["KC_NO", "KC_NO", "KC_LEFT", "KC_DOWN", "KC_RGHT", "KC_PSCR", "KC_NO", "KC_LEFT", "KC_DOWN", "KC_RGHT", "KC_NO", "KC_NO"],
                        ["KC_NO", "KC_NO", "KC_END", "KC_DOWN", "KC_PGDN", "KC_INS", "KC_NO", "KC_END", "KC_DOWN", "KC_PGDN", "KC_NO", "KC_NO"],
                        ["KC_NC",  "KC_NC",  "KC_NO", "KC_NO",  "KC_BSPC", "KC_NO", "KC_NO", "KC_SPC",  "KC_NO", "KC_NO", "KC_NC",  "KC_NC"]]

Layers["2"]["mod1"] =  [["KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO"],
                        ["KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO"],
                        ["KC_LSFT", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_RSFT"],
                        ["KC_NC", "KC_NC", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NC", "KC_NC"]]



Layers["3"] = {}

Layers["3"]["keys"] =  [["KC_TAB", "KC_NO", "KC_HOME", "KC_UP", "KC_PGUP", "KC_NO", "KC_NO", "KC_P7", "KC_P8", "KC_P9", "KC_PAST", "KC_ENT"],
                    ["KC_ESC", "KC_NO", "KC_LEFT", "KC_DOWN", "KC_RGHT", "KC_NO", "KC_EQL", "KC_P4", "KC_P5", "KC_P6", "KC_PMNS", "KC_QUOT"],
                    ["KC_NO", "KC_NO", "KC_END", "KC_DOWN", "KC_PGDN", "KC_NO", "KC_NO", "KC_P1", "KC_P2", "KC_P3", "KC_PPLS", "KC_NO"],
                    ["KC_NC",  "KC_NC",  "KC_NO", "KC_NO",  "KC_BSPC", "KC_NO", "KC_NO", "KC_SPC",  "KC_NO", "KC_NO", "KC_NC",  "KC_NC"]]

Layers["3"]["mod1"] =  [["KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO"],
                        ["KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO"],
                        ["KC_LSFT", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_RSFT"],
                        ["KC_NC", "KC_NC", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NC", "KC_NC"]]


Layers["4"] = {}

Layers["4"]["keys"] =  [["KC_GRV", "KC_1", "KC_2", "KC_3", "KC_4", "KC_5", "KC_6", "KC_7", "KC_8", "KC_9", "KC_0", "KC_MINS"],
                    ["KC_GRV", "KC_1", "KC_2", "KC_3", "KC_4", "KC_5", "KC_6", "KC_7", "KC_8", "KC_9", "KC_0", "KC_MINS"],
                    ["KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO"],
                    ["KC_NC",  "KC_NC",  "KC_NO", "KC_NO",  "KC_BSPC", "KC_NO", "KC_NO", "KC_SPC",  "KC_NO", "KC_NO", "KC_NC",  "KC_NC"]]

Layers["4"]["mod1"] =  [["KC_LSFT", "KC_LSFT", "KC_LSFT", "KC_LSFT", "KC_LSFT", "KC_LSFT", "KC_LSFT", "KC_LSFT", "KC_LSFT", "KC_LSFT", "KC_LSFT", "KC_LSFT"],
                        ["KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO"],
                        ["KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO"],
                        ["KC_NC", "KC_NC", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NO", "KC_NC", "KC_NC"]]






#modSet = ["KC_LCTL","KC_LSFT","KC_LALT","KC_LGUI","KC_RCTL","KC_RSFT","KC_RALT","KC_RGUI"]



def main():

    kDict = keycodes.keyCodeDict()

    for key in Layers.keys():
        print(f"//Layer {key}:")
        kc_code_left = "uint8_t kc_left_l"+key+"[22]={"
        kc_code_right = "uint8_t kc_right_l"+key+"[22]={"

        mod_code_left = "uint8_t mod_left_l"+key+"[22]={"
        mod_code_right = "uint8_t mod_right_l"+key+"[22]={"

        firstWrite = True
        kc_l_ind = 0
        kc_r_ind = 0
        mod_l_ind = 0
        mod_r_ind = 0

        for jj in range(0,n_row):
            for ii in range(0,int(n_col/2)):

                k_left = Layers[key]["keys"][jj][int(n_col/2)-1-ii]
                k_right = Layers[key]["keys"][jj][int(n_col/2)+ii]

                m1_left = Layers[key]["mod1"][jj][int(n_col/2)-1-ii]
                m1_right = Layers[key]["mod1"][jj][int(n_col/2)+ii]



                # kr is not checked since it is the same board (if KC_NC is in one and not the other then it is a mistake in the keymap)

                if not firstWrite:
                    if k_left != "KC_NC":
                        kc_code_left = kc_code_left + " ," + kDict[k_left]
                        kc_l_ind+=1
                    if k_right != "KC_NC":
                        kc_code_right = kc_code_right + " ," + kDict[k_right]
                        kc_r_ind+=1
                    if m1_left != "KC_NC":
                        mod_code_left = mod_code_left + " ," + kDict[m1_left]
                        mod_l_ind+=1
                    if m1_right != "KC_NC":
                        mod_code_right = mod_code_right + " ," + kDict[m1_right]
                        mod_r_ind+=1


                else:
                    if k_left != "KC_NC":
                        kc_code_left = kc_code_left + kDict[k_left]
                        kc_l_ind+=1
                    if k_right != "KC_NC":
                        kc_code_right = kc_code_right + kDict[k_right]
                        kc_r_ind+=1
                    if m1_left != "KC_NC":
                        mod_code_left = mod_code_left + kDict[m1_left]
                        mod_l_ind+=1
                    if m1_right != "KC_NC":
                        mod_code_right = mod_code_right + kDict[m1_right]
                        mod_r_ind+=1


                    firstWrite = False


        kc_code_left = kc_code_left + "};"
        kc_code_right = kc_code_right + "};"

        mod_code_left = mod_code_left + "};"
        mod_code_right = mod_code_right + "};"

        print(f"//Code length: {kc_l_ind}")
        print(kc_code_left)
        print(f"//Code length: {kc_r_ind}")
        print(kc_code_right)
        print(f"//Code length: {mod_l_ind}")
        print(mod_code_left)
        print(f"//Code length: {mod_r_ind}")
        print(mod_code_right)


    return 0


if __name__ == '__main__':
    main()
