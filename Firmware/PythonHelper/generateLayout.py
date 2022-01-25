
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

#COLEMAK
# KC_NC is interpreted as a not connected key (is skipped)
Layer_0 = [["KC_TAB",  "KC_Q",  "KC_W",  "KC_F",  "KC_P",  "KC_B",  "KC_J",  "KC_L",  "KC_U",  "KC_Y",  "KC_SCLN",  "KC_ENT"],
  ["KC_ESC",  "KC_A",  "KC_R",  "KC_S",  "KC_T",  "KC_G",  "KC_M",  "KC_N",  "KC_E",  "KC_I",  "KC_O",  "KC_QUOT"],
  ["KC_LSFT",  "KC_Z",  "KC_X",  "KC_C",  "KC_D",  "KC_V",  "KC_K",  "KC_H",  "KC_COMM",  "KC_DOT",  "KC_SLSH",  "KC_RSFT"],
  ["KC_NC",  "KC_NC",  "KC_DOWN",  "KC_UP",  "KC_BSPC", "KC_NO", "KC_NO", "KC_SPC",  "KC_LEFT",  "KC_RGHT", "KC_NC",  "KC_NC"]]



modSet = ["KC_LCTL","KC_LSFT","KC_LALT","KC_LGUI","KC_RCTL","KC_RSFT","KC_RALT","KC_RGUI"]



def main():

    n_row = len(Layer_0)
    n_col = len(Layer_0[0])

    kDict = keycodes.keyCodeDict()

    kc_code_left = "uint8_t kc_left_l0[22]={"
    kc_code_right = "uint8_t kc_right_l0[22]={"

    mod_set_left = []
    mod_set_right = []

    firstWrite = False
    vect_ind = 0

    for row in Layer_0:
        for ii in range(0,int(n_col/2)):

            k_left = row[int(n_col/2)-1-ii]
            k_right = row[int(n_col/2)+ii]

            # kr is not checked since it is the same board (if KC_NC is in one and not the other then it is a mistake in the keymap)
            if k_left != "KC_NC":

                kl = kDict[k_left]
                kr = kDict[k_right]

                if firstWrite:
                    kc_code_left = kc_code_left + " ," + kl
                    kc_code_right = kc_code_right + " ," + kr
                else:
                    kc_code_left = kc_code_left + kl
                    kc_code_right = kc_code_right + kr
                    firstWrite = True

                if k_left in modSet:
                    mod_set_left.append(vect_ind)
                if k_right in modSet:
                    mod_set_right.append(vect_ind)

                vect_ind+=1


    kc_code_left = kc_code_left + "};"
    kc_code_right = kc_code_right + "};"

    print(kc_code_left)
    print(kc_code_right)

    print("\n\nVector indices to filter for modifier: Left")
    for ii in mod_set_left: print(f"{ii}")

    print("\n\nVector indices to filter for modifier: Right")
    for ii in mod_set_right: print(f"{ii}")



    return 0


if __name__ == '__main__':
    main()
