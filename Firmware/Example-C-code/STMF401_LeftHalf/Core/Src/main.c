/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_hid.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

extern USBD_HandleTypeDef hUsbDeviceFS;





//Storage for the encoder values. Counter 1 is the previous value
//Counter 2 is the present value. The enc1 count is incremented by the
// difference between counter 2 and counter 1 (can be negative).
uint32_t enc1_counter1 = 0x0;
uint32_t enc1_counter2 = 0x0;
int32_t enc1_count = 0x0;
//The encoder count will define how many key presses to send to the computer
//The MCU loop happens faster than the computer polling so there must be a delay.
//The key press is sent until the count increments up to the delay. Then the
//key release is also sent for enough times for the count to increment to 2x the
//delay
const uint16_t enc1_delay = 200;
const uint8_t enc_scaling = 2;
uint16_t enc_hold_count=0x0;


const uint16_t debounce = 10;


// This index is to be passed during the column and row scanning to keep track
// of which key is being checked
uint8_t vect_ind = 0x0;

// Initialize a vector for the keyboard
// This is the storage for how many cycles a key is logged as connected.
// This will be checked against the debounce count in order to determine that
// a key should be considered to be pressed.
uint16_t kb_count_vect[22]={0x0};

//This is the variable to keep track of the layer state
uint8_t layer_state = 0;


// These are the keycode and modifier vectors. There is a keycode and modifier for each
// key on each half. There are two more codes and modifiers at the end in order to
// account a single encoder. One code for clockwise and one for counter-clockwise
// encoder rotation (increasing or decreasing).

//Layer 0:
const uint8_t kc_left_l0[24]={0x05 ,0x13 ,0x09 ,0x1a ,0x14 ,0x2b ,0x0a ,0x17 ,0x16 ,0x15 ,0x04 ,0x00 ,0x19 ,0x07 ,0x06 ,0x1b ,0x1d ,0x00 ,0x00 ,0x2a ,0x00 ,0x29, 0x51, 0x52};
const uint8_t kc_right_l0[24]={0x0d ,0x0f ,0x18 ,0x1c ,0x33 ,0x28 ,0x10 ,0x11 ,0x08 ,0x0c ,0x12 ,0x34 ,0x0e ,0x0b ,0x36 ,0x37 ,0x38 ,0x00 ,0x00 ,0x2c ,0x00 ,0x00 ,0x50, 0x4f};
const uint8_t mod_left_l0[24]={0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x02 ,0x00 ,0x00 ,0x00 ,0x00, 0x00 ,0x00};
const uint8_t mod_right_l0[24]={0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x20 ,0x00 ,0x00 ,0x00 ,0x40, 0x00 ,0x00};
//Layer 1:
const uint8_t kc_left_l1[24]={0x00 ,0x4b ,0x52 ,0x4a ,0x00 ,0x2b ,0x46 ,0x4f ,0x51 ,0x50 ,0x00 ,0x00 ,0x4c ,0x4e ,0x51 ,0x4d ,0x00 ,0x00 ,0x00 ,0x2a ,0x00 ,0x29, 0x51, 0x52};
const uint8_t kc_right_l1[24]={0x54 ,0x5f ,0x60 ,0x61 ,0x55 ,0x28 ,0x2e ,0x5c ,0x5d ,0x5e ,0x56 ,0x34 ,0x00 ,0x59 ,0x5a ,0x5b ,0x57 ,0x00 ,0x00 ,0x2c ,0x62 ,0x63 ,0x50, 0x4f};
const uint8_t mod_left_l1[24]={0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x02 ,0x00 ,0x00 ,0x00 ,0x00, 0x00 ,0x00};
const uint8_t mod_right_l1[24]={0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x20 ,0x00 ,0x00 ,0x00 ,0x00, 0x00 ,0x00};
//Layer 2:
const uint8_t kc_left_l2[24]={0x22 ,0x21 ,0x20 ,0x1f ,0x1e ,0x35 ,0x22 ,0x21 ,0x20 ,0x1f ,0x1e ,0x35 ,0x00 ,0x00 ,0x00 ,0x10 ,0x00 ,0x00 ,0x00 ,0x2a ,0x00 ,0x29, 0x51, 0x52};
const uint8_t kc_right_l2[24]={0x23 ,0x24 ,0x25 ,0x26 ,0x27 ,0x2d ,0x23 ,0x24 ,0x25 ,0x26 ,0x27 ,0x2d ,0x2f ,0x30 ,0x2f ,0x30 ,0x31 ,0x31 ,0x00 ,0x2c ,0x00 ,0x00 ,0x50, 0x4f};
const uint8_t mod_left_l2[24]={0x02 ,0x02 ,0x02 ,0x02 ,0x02 ,0x02 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x08 ,0x08 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, 0x00 ,0x00};
const uint8_t mod_right_l2[24]={0x02 ,0x02 ,0x02 ,0x02 ,0x02 ,0x02 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x02 ,0x02 ,0x00 ,0x00 ,0x00 ,0x02 ,0x00 ,0x00 ,0x00 ,0x40, 0x00 ,0x00};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */




void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){

	//Update the counter2 value.
	//this value will be compared against counter1 and if it is
	//different then the delta will be used to send keycodes for the
	//encoder and then counter1 will be sent to counter2 after the
	//the codes are sent

	// This callback function is triggered whenever there
	// is a change in the encoder value. There is no need to
	// poll the encoder.
	enc1_counter2 = __HAL_TIM_GET_COUNTER(htim);

}



void check_layer_state(uint8_t *kbSPI_tx, uint8_t *kbSPI_rx, uint8_t n_kbSPI){

    uint8_t ii;

    // Check for certain keys that are pressed. There are only keycode vectors
    // for layers 0, 1, and 2 so layers 3 and 4 are not used right now.

	// Layer 0: default
	// Layer 1: kbSPI_tx has key 21
	// Layer 2: kbSPI_rx has key 21
	// Layer 3: kbSPI_tx has key 22
    // Layer 4: kbSPI_rx has key 22


    if(layer_state==0){
        // If layer state is 0 check if a layer key is pressed
        for(ii=0; ii<n_kbSPI; ++ii){
    		//if(kbSPI_rx[ii]==22){
    		//	layer_state=4;
    		//}
    		//else if(kbSPI_tx[ii]==22){
    		//	layer_state=3;
    		//}
    		if(kbSPI_rx[ii]==21){
    			layer_state=2;
    		}
    		else if(kbSPI_tx[ii]==21){
    			layer_state=1;
    		}
    	}
    }
    else{
        // If a layer key was previously pressed (layer_state!=0) then only set the layer state
        // back to zero if that key is released
    	// This allows a second layer key to be pressed without overiding the first layer state
    	// For example, my numpad setup uses the key 21 as a "0" for the right half.
    	// when I press that key I want to be sure it doesn't cause layer state 2 to become
    	// active instead of staying in layer state 1 and being interpreted as a key press.
        if(layer_state==4){
            layer_state=0;
            for(ii=0; ii<n_kbSPI; ++ii){
        		if(kbSPI_rx[ii]==22){
        			layer_state=4;
        		}
            }
        }
        else if(layer_state==3){
            layer_state=0;
            for(ii=0; ii<n_kbSPI; ++ii){
        		if(kbSPI_tx[ii]==22){
        			layer_state=3;
        		}
            }
        }
        else if(layer_state==2){
            layer_state=0;
            for(ii=0; ii<n_kbSPI; ++ii){
        		if(kbSPI_rx[ii]==21){
        			layer_state=2;
        		}
            }
        }
        else if(layer_state==1){
            layer_state=0;
            for(ii=0; ii<n_kbSPI; ++ii){
        		if(kbSPI_tx[ii]==21){
        			layer_state=1;
        		}
            }
        }
    }

}




void send_keycodes(uint8_t *kbSPI_tx, uint8_t *kbSPI_rx, uint8_t n_kbSPI){

	// In this function the vectors kbSPI_tx and kbSPI_rx are parsed
	// Both of these vectors contain the indices of keys that have been
	// held down for long enough to be considered pressed (exceeded the
	// debounce count)
	//
	// These vectors are a vector of indices. eg. if the number 19 is
	// in the vector it means that the key 19 is pressed. The kbSPI_tx
	// is for this half, the kbSPI_rx is for the other half.

	// USB HID data. Most significant byte is the modifiers (data[0])
	// Modifiers are combined using a bitwise or
	// data[1] is reserved
	// data[2] to data[7] are the keycodes

	uint8_t kbUSB_data[8]={0x00};

	// code keeps track of the number of keycodes that have been added to the
	// keycode data. This must be limited to 6.
	uint8_t code=0;
	uint8_t ii;

	// These are temporary values to hold the keycode and modifier that correspond to
	// a given key that is interpreted.
	uint8_t kc = 0x0;
	uint8_t mod = 0x0;


	code=0;

	// Before keys are interpreted the vectors are checked to see if any layer
	// keys are pressed. If a layer key is pressed the layer state is changed and
	// this then changes the interpretation of the the keycode and modifier that
	// are associated with a key press.

	// Layer keycodes and modifiers are stored in separate vectors for each layer.
	check_layer_state(kbSPI_tx,kbSPI_rx,n_kbSPI);


	// Interpret kbSPI_tx (this half)
	for(ii=0; ii<n_kbSPI; ++ii){

		if(kbSPI_tx[ii]>0){
			// Look up each keycode and modifier from the keymap vectors
			// based on the layer state
			if(layer_state==0){
				kc = kc_left_l0[kbSPI_tx[ii]-1];
				mod = mod_left_l0[kbSPI_tx[ii]-1];
			}
			else if(layer_state==1){
				kc = kc_left_l1[kbSPI_tx[ii]-1];
				mod = mod_left_l1[kbSPI_tx[ii]-1];

			}
			else if(layer_state==2){
				kc = kc_left_l2[kbSPI_tx[ii]-1];
				mod = mod_left_l2[kbSPI_tx[ii]-1];
			}
			// Additional layers can be added as needed with more vectors
			//
			//else if(layer_state==3){
			//	kc = kc_left_l3[kbSPI_tx[ii]-1];
			//	mod = mod_left_l3[kbSPI_tx[ii]-1];
			//}
			//else{
			//	kc = kc_left_l4[kbSPI_tx[ii]-1];
			//	mod = mod_left_l4[kbSPI_tx[ii]-1];
			//}

			// If the keycode is not empty and the number of keycodes
			// already added is less than 6 add the keycode to the data

			if(kc>0){
				if(code<6){
					kbUSB_data[code+2]=kc;
					++code;
				}
			}

			kbUSB_data[0] = kbUSB_data[0]|mod;

		}


	}




	// Now add in kbSPI_rx (the other board)
	for(ii=0; ii<n_kbSPI; ++ii){

		if(kbSPI_rx[ii]>0){
			// Look up each keycode and modifier from the keymap vectors
			// based on the layer state
			if(layer_state==0){
				kc = kc_right_l0[kbSPI_rx[ii]-1];
				mod = mod_right_l0[kbSPI_rx[ii]-1];
			}
			else if(layer_state==1){
				kc = kc_right_l1[kbSPI_rx[ii]-1];
				mod = mod_right_l1[kbSPI_rx[ii]-1];

			}
			else if(layer_state==2){
				kc = kc_right_l2[kbSPI_rx[ii]-1];
				mod = mod_right_l2[kbSPI_rx[ii]-1];
			}
			//else if(layer_state==3){
			//	kc = kc_right_l3[kbSPI_rx[ii]-1];
			//	mod = mod_right_l3[kbSPI_rx[ii]-1];
			//}
			//else{
			//	kc = kc_right_l4[kbSPI_rx[ii]-1];
			//	mod = mod_right_l4[kbSPI_rx[ii]-1];
			//}


			// If the keycode is not empty and the number of keycodes
			// already added is less than 6 add the keycode to the data
			if(kc>0){
				if(code<6){
					kbUSB_data[code+2]=kc;
					++code;
				}
			}

			kbUSB_data[0] = kbUSB_data[0]|mod;

		}
	}

	// Update the data sent over USB using USBD_HID_SendReport
	// Note that this function only updates the data that can
	// be sent. The computer polls over USB triggering the data
	// transmission. USBD_HID_SendReport only updates the
	// available data. It can be called repeatedly and will not
	// cause any buffer over-run. It is non-blocking so it will
	// not wait to be polled by the computer.
	USBD_HID_SendReport(&hUsbDeviceFS,kbUSB_data,8);



}



void scan_cols(uint8_t row)
{

	// Row specified by rowInd is toggled. Scan through columns to check it any column is pulled low
	// diodes go from column to row. Rows are set low to toggle. Columns have a pull-up on to set them
	// high when there is no connection.

	//Check column 1
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)!=GPIO_PIN_SET){
		kb_count_vect[vect_ind]+=1;
	}
	else{
		kb_count_vect[vect_ind]=0;
	}
	vect_ind+=1;

	//Check column 2
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)!=GPIO_PIN_SET){
		kb_count_vect[vect_ind]+=1;
	}
	else{
		kb_count_vect[vect_ind]=0;
	}
	vect_ind+=1;

	//Check column 3
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)!=GPIO_PIN_SET){
		kb_count_vect[vect_ind]+=1;
	}
	else{
		kb_count_vect[vect_ind]=0;
	}
	vect_ind+=1;

	//Check column 4
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)!=GPIO_PIN_SET){
		kb_count_vect[vect_ind]+=1;
	}
	else{
		kb_count_vect[vect_ind]=0;
	}
	vect_ind+=1;

	// Row 3 only has 4 key spots so no need to check pin B12 or in B13 (Col 5 or Col 6)
	if(row<3){
		//Check column 5
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)!=GPIO_PIN_SET){
			kb_count_vect[vect_ind]+=1;
		}
		else{
			kb_count_vect[vect_ind]=0;
		}
		vect_ind+=1;

		//Check column 6
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)!=GPIO_PIN_SET){
			kb_count_vect[vect_ind]+=1;
		}
		else{
			kb_count_vect[vect_ind]=0;
		}
		vect_ind+=1;
	}


}


void scan_matrix(void)
{
	vect_ind = 0x0;
	// Key switch detection...
	// Trigger row 1 and scan columns
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,0);
	scan_cols(0);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,1);

	// Trigger row 2 and scan columns
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,0);
	scan_cols(1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,1);

	// Trigger row 3 and scan columns
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,0);
	scan_cols(2);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,1);

	// Trigger row 4 and scan columns
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,0);
	scan_cols(3);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,1);

}



void update_keys(uint8_t *kbSPI_tx, uint8_t n_kbSPI){

	// The indices of the vector are sent instead of the keycodes.
	// This will allow the half that is connected to the computer
	// to interpret the keys (use layers etc.)
	//
	// In this function any key that has a count greater than
	// the debounce count will be considered to be a valid key press.
	// It will be added to the vector of indices that is keeping track
	// of which keys are pressed. This vector is limited in size to
	// 8 keys at once (8 keys per half maximum).
	// Even though this is larger than the number of key codes that can
	// be sent it seemed okay to have a little more space for keys
	// and then down select when the vectors are interpreted into
	// keycodes.
	// There is no rollover of keys (keys are not stored for future transmission
	// if there are too many keys pressed).

	uint8_t kb_ind;
	uint8_t ii;

	int32_t enc_delta;

	for( kb_ind=0; kb_ind<n_kbSPI; ++kb_ind){
		kbSPI_tx[kb_ind] = 0x0;
	}

	kb_ind=0;

	for( ii = 0; ii < 22; ++ii ){
		if(kb_count_vect[ii]>debounce){
			kb_count_vect[ii]=debounce+1; //prevent overrun of uint16 values

			// if there is space in the vector append the key
			if(kb_ind<n_kbSPI){
				kbSPI_tx[kb_ind]=ii+1; //increment by one since zeros mean no keys are pressed
				++kb_ind;
			}
		}
	}


	// Interpret the encoder signal if the value has changed.
	// The previous encoder value is stored in counter1 and the updated
	// values is in counter2. The difference is how many positions the
	// encoder has moved. The encoder wraps around at 65535 (only has values
	// from 0 to 65535) so if a value that is greater or less than a reasonable
	// delta is detected it means the encoder has gone lower than zero or
	// higher than 65535. So that number should be added or subtracted from
	// the delta.

	// In order to dampen the response the delta is divided by a scaling value.
	// This dampened delta is added to an encoder count that can be negative
	// or positive. This count is then used to sequentially add key indices
	// (either 23 or 24) to the vector of indices until the count returns back to zero.
	if(enc1_counter2!=enc1_counter1){
		enc_delta=(enc1_counter2-enc1_counter1);
		if(enc_delta>65000){
			enc_delta = enc_delta-65535;
		}
		else if(enc_delta<-65000){
			enc_delta = enc_delta+65536;
		}
		enc1_counter1=enc1_counter2;
		enc1_count = enc1_count+enc_delta/enc_scaling;
	}


	// If there is space in the vector of keycodes and the encoder count
	// is not zero then add the indice 23 or 24 to the set of key indices
	// This indice is held on for the number of cycles in enc1_delay and
	// then held off for the same number of cycles. If the code is turned
	// on and off too fast the computer will not interpret it so this
	// delay is necessary. Too long of a value for enc1_delay and the
	// encoder values will back up and the response will seem sluggish.
	// Too small of a value and the encoder key presses will not be
	// registered by the computer.
	if(kb_ind<n_kbSPI){
		if(enc1_count!=0){
			if(enc_hold_count<enc1_delay){
				++enc_hold_count;
				if(enc1_count<0){
					kbSPI_tx[kb_ind]=23;
					++kb_ind;
				}
				else{
					kbSPI_tx[kb_ind]=24;
					++kb_ind;
				}
			}
			else if(enc_hold_count<(enc1_delay+enc1_delay)){
				++enc_hold_count;

			}
			else{
				if(enc1_count<0){
					++enc1_count;
				}
				else{
					--enc1_count;
				}
				enc_hold_count=0x0;
			}

		}

	}


}




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  // Storage for the number of key indices for each half (tx and rx) and
  // the indices themselves.
  uint8_t n_kbSPI = 8;
  uint8_t kbSPI_tx[8]={0x0};
  uint8_t kbSPI_rx[8]={0x0};



  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  // Start the encoder timer interrupt
  HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);

  // Initialize all GPIO Output pins (rows) to 1
  // keys are interpreted as being pressed when
  // a row is set to low and the corresponding column
  // is detected as low. All columns have pullup
  // resistors enabled in the MCU so that they normally
  // detect high and are only pulled low by a key press
  // when the corresponding row is low.
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,1);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,1);
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,1);
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,1);

  // USB polling interval is not necessary
  //USB_polling = USBD_HID_GetPollingInterval(&hUsbDeviceFS);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	// Scan the matrix and increment the vector of keys for any
	// key that has been pressed. If a column is not pulled low
	// reset the count to zero for the corresponding key.
	scan_matrix();

	//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);  // this just turns on the blue LED.

	// Update the vector of keys that are considered to be pressed
	// each number in the vector corresponds to the switch that is pressed
	update_keys(kbSPI_tx,n_kbSPI);

	// Receive the data of the keys that are pressed on the other half.
	// This is a blocking function so it will wait for data to arrive.
	// This will also serve to regulate the other half making it have
	// the same number cycles (makes it easier for the crude debounce count
	// and encoder count values since the same numbers can be used on the other
	// half. If the other half was looping faster (because it doesn't need to
	// interpret layer states or keycodes) then those values would need to
	// be higher for the same effective amounts of time.
	HAL_SPI_Receive(&hspi1,kbSPI_rx,n_kbSPI,10000);

	// Turn off the blue LED light on the development board
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,1);

	// Update the keycode data that is ready to be sent to the computer
	// when it polls for it.
	send_keycodes(kbSPI_tx,kbSPI_rx,n_kbSPI);



  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 15;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 15;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB12 PB13 PB14
                           PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
