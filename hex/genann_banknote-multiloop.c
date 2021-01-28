#include <stdio.h>
#include <stdlib.h>
//#include "genann.h"



#ifdef __cplusplus
extern "C" {
#endif


#include <assert.h>
#include <errno.h>
#include <math.h>
#include <string.h>


#define LOOKUP_SIZE 1421





//const char *save_name = "example/xor.ann";

//static int init_array[4] = {2,1,2,1}; //input ,hidden(layer num), hidden2 (neurons in one layer), output


//changeable variables

#define inputs_g 4
#define hidden_layers_g 2
#define hidden_g  3
#define outputs_g  3



//non changable variables
#if hidden_layers_g
#define  hidden_weights ((inputs_g+1) * hidden_g + (hidden_layers_g-1) * (hidden_g+1) * hidden_g)//10+6
#define output_weights ((hidden_g+1) * outputs_g) //6
#else
#define  hidden_weights 0
#define output_weights ((inputs_g+1) * outputs_g)
#endif

#define total_weights_g ((hidden_weights + output_weights))//9
#define total_neurons ((inputs_g + hidden_g * hidden_layers_g + outputs_g))
#define size  (sizeof(genann) + sizeof(int) * (total_weights_g + total_neurons + (total_neurons - inputs_g)))
//<<8 https://www.rapidtables.com/convert/number/decimal-to-hex.html?x=-6.029
//https://www.rapidtables.com/convert/number/hex-to-decimal.html?x=1c6
// turn float to hex and shift left 8 bits (*256)
//#define print_en

volatile int __attribute__((section(".mySection_par"))) startPar =  0x00 ;
volatile int __attribute__((section(".mySection_par_num"))) par_num =  0x03 ; //constant
volatile int __attribute__((section(".mySection_arr_str"))) arr_str =  0x00 ;
volatile int __attribute__((section(".mySection_arr_end"))) arr_end =  0x00 ;


#if hidden_layers_g
static int input_weights_arr [hidden_g][inputs_g+1+1] = {
	{-1505	,0,	-8544	,	-6025	,	5346	,	477	,	 },
	{2810	,0,	-405	,	2997	,	4222	,	-2526	,},
	{2810	,0,	-405	,	2997	,	4222	,	-2526	,},
};
static int hidden_weights_arr [hidden_layers_g-1][hidden_g][hidden_g+1+2] = {
	{{1501	,0,0,	-433	,	-459	,-459	,},  
	{2997	,0,0,	4222	,	-2526	,-2526	,},	
	{2997	,0,0,	4222	,	-2526	,-2526	,},		},
};
static int output_weights_arr [outputs_g][hidden_g+1+1] = {
	{1501	,0,	-433,	-459,	-459	,},
	{407	,0,	804	,	1951,	1951		},
	{407	,0,	804	,	1951,	1951		},
};
#else
static int output_weights_arr [outputs_g][inputs_g+1] = {
	
};
#endif

#define INPUT_SIZE 1
    /* Stores input array and output of each neuron (total_neurons long). */
//first cols are inputs, others are hidden and output neurons 
static int input[INPUT_SIZE][total_neurons] = { //inputs_g + hidden_g * hidden_layers_g + outputs_g
{	7	,	49	,	60	,	56	,0,0 ,0,0 ,0,0}, 
 };

static const int sigmoid_dom_min = -1024;
static const int sigmoid_dom_max = 1024;

//static const int hidden_weights = hidden_layers ? (inputs+1) * hidden + (hidden_layers-1) * (hidden+1) * hidden : 0;//6
//static const int output_weights = (hidden_layers ? (hidden+1) : (inputs+1)) * outputs;//3
//static const int total_weights = (hidden_weights + output_weights);//9

//static const int total_neurons = (inputs + hidden * hidden_layers + outputs); 4+ 2*1+2 = 8

/* Allocate extra size for weights, outputs, and deltas. */
//static const int size = sizeof(genann) + sizeof(int) * (total_weights + total_neurons + (total_neurons - inputs));
//static const genann *ret = malloc(size);



// -5.5390625 -(0x5.8A)	low limit  altý 0.00390625     (0x0.01)    den küçük
//  5.5390625  (0x5.8A)   up limit  üstü 1-0.00390625 = 0.99609375  den büyük
//  0.0078125  (0x0.02) is step size
 
#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#define unused          __attribute__((unused))
#else
#define likely(x)       x
#define unlikely(x)     x
#define unused
#pragma warning(disable : 4996) /* For fscanf */
#endif

 /* How many inputs, outputs, and hidden neurons. */

const int ann_inputs = inputs_g;
const int ann_hidden_layers = hidden_layers_g;
const int ann_hidden = hidden_g;
const int ann_outputs = outputs_g;
/* Total number of weights, and size of weights buffer. */
const int ann_total_weights = total_weights_g;



 int genann_activation( int a) {
    //assert(!isnan(a));
    a = a >> 8;
 //uint8_t  r = (uint8_t ) ((a/1024+1) *   (128));
	
    return   (a/1024+1) *   (128);

    /* Because floating point... */
    //if (unlikely(j >= LOOKUP_SIZE)) return lookup[LOOKUP_SIZE - 1];

}

#pragma GCC optimize ("no-peel-loops")
void genann_run () __attribute__((optimize("no-unroll-loops")));

void genann_run(volatile int *inputs) {
    volatile int *i = inputs;
	if (!ann_hidden_layers) {//Input layer if no hidden layers
		for (int j = 0; j < ann_outputs; ++j) {
			output_weights_arr[j][1] = output_weights_arr[j][0]  * -1;
			output_weights_arr[j][1] = output_weights_arr[j][1] * 256;
			for (int k = 0; k < ann_inputs; ++k) {
				output_weights_arr[j][1] += output_weights_arr[j][k+1] * i[k];
			}
			output_weights_arr[j][1]= genann_activation( output_weights_arr[j][1]);
		}
		return ;
	}
	
	arr_str =  &input_weights_arr[0] ;
	arr_end =  &input_weights_arr[hidden_g-1] + (&input_weights_arr[hidden_g-1] - &input_weights_arr[hidden_g-2]);
	startPar = 0x1;
    /* Input layer */
	#pragma GCC unroll 0
    for (int j = 0; j < ann_hidden; ++j) {
        input_weights_arr[j][1] = input_weights_arr[j][0] * -1;
        input_weights_arr[j][1] = input_weights_arr[j][1] * 256;
        for (int k = 0; k < ann_inputs; ++k) {
            input_weights_arr[j][1] += input_weights_arr[j][k+1] * i[k];
        }
        input_weights_arr[j][1] = genann_activation( input_weights_arr[j][1]);
    }
	startPar = 0x0;	
	
    for (int j = 0; j < ann_hidden; ++j) {	
		hidden_weights_arr[0][j][1] = input_weights_arr[j][1];
	}
	
    /* Hidden layers, if any. */
    for (int h = 0; h < ann_hidden_layers-1; ++h) {	
		arr_str =  &hidden_weights_arr[h][0] ;
		arr_end =  &hidden_weights_arr[h][hidden_g-1] + (&hidden_weights_arr[h][hidden_g-1] - &hidden_weights_arr[h][hidden_g-2]);
		startPar = 0x1;
		#pragma GCC unroll 0
        for (int j = 0; j < ann_hidden; ++j) {
            hidden_weights_arr[h][j][2] = hidden_weights_arr[h][j][0] * -1;
            hidden_weights_arr[h][j][2] = hidden_weights_arr[h][j][2] * 256;
            for (int k = 0; k < ann_hidden; ++k) {
				hidden_weights_arr[h][j][2] += hidden_weights_arr[h][j][k+1] * hidden_weights_arr[h][k][1];
            }
           hidden_weights_arr[h][j][2] = genann_activation( hidden_weights_arr[h][j][2]);
        }
		startPar = 0x0;
    }

    /* Output layer. */
	arr_str =  &output_weights_arr[0] ;
	arr_end =  &output_weights_arr[outputs_g-1] + (&output_weights_arr[outputs_g-1] - &output_weights_arr[outputs_g-2]);
	startPar = 0x1;
	asm volatile ("nop");
	#pragma GCC unroll 0
    for (int j = 0; j < ann_outputs; ++j) {
        output_weights_arr[j][1] = output_weights_arr[j][0] * -1;
            output_weights_arr[j][1] = output_weights_arr[j][1] * 256;
			for (int k = 0; k < ann_hidden; ++k) {
            output_weights_arr[j][1] += output_weights_arr[j][k+2] * hidden_weights_arr[hidden_layers_g-2][k][1];
        }
        output_weights_arr[j][1] = genann_activation( output_weights_arr[j][1]);
    }
	startPar = 0x0;
	return ;
}


int main(int argc, char *argv[])
{
	for (int j = 0; j < INPUT_SIZE; j++) {
            genann_run(input[j]);//result is last columns of input     in this case 6&7 

	}
    return 0;
}
