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
#define hidden_layers_g 1
#define hidden_g  2
#define outputs_g  2



//non changable variables
#if hidden_layers_g
#define  hidden_weights ((inputs_g+1) * hidden_g + (hidden_layers_g-1) * (hidden_g+1) * hidden_g)//10
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


static int weights [total_weights_g]  = //in_bias1+ in_weig1-n + in_bias2 + inweig2-n .... 
{	-1505	,
	-8544	,
	-6025	,
	5346	,
	477	,
	2810	,
	-405	,
	2997	,
	4222	,
	-2526	,
	1501	,
	-433	,
	-459	,
	407	,
	804	,
	1951	};

#define INPUT_SIZE 75
    /* Stores input array and output of each neuron (total_neurons long). */
//first cols are inputs, others are hidden and output neurons 
static int input[INPUT_SIZE][total_neurons] = { //inputs_g + hidden_g * hidden_layers_g + outputs_g
{	7	,	49	,	60	,	56	,0,0 ,0,0}, 
{	69	,	92	,	1	,	96	,0,0 ,0,0}, 
{	3	,	2	,	22	,	6	,0,0 ,0,0}, 
{	17	,	52	,	60	,	33	,0,0 ,0,0}, 
{	3	,	63	,	65	,	81	,0,0 ,0,0}, 
{	83	,	122	,	3	,	89	,0,0 ,0,0}, 
{	52	,	31	,	10	,	14	,0,0 ,0,0}, 
{	32	,	21	,	19	,	4	,0,0 ,0,0}, 
{	17	,	24	,	1	,	1	,0,0 ,0,0}, 
{	3	,	26	,	42	,	8	,0,0 ,0,0}, 
{	7	,	47	,	52	,	26	,0,0 ,0,0}, 
{	9	,	43	,	44	,	45	,0,0 ,0,0}, 
{	52	,	48	,	8	,	53	,0,0 ,0,0}, 
{	90	,	117	,	3	,	59	,0,0 ,0,0}, 
{	63	,	45	,	10	,	19	,0,0 ,0,0}, 
{	45	,	3	,	8	,	3	,0,0 ,0,0}, 
{	26	,	2	,	12	,	10	,0,0 ,0,0}, 
{	11	,	35	,	40	,	15	,0,0 ,0,0}, 
{	14	,	29	,	32	,	18	,0,0 ,0,0}, 
{	31	,	20	,	11	,	18	,0,0 ,0,0}, 
{	45	,	24	,	2	,	31	,0,0 ,0,0}, 
{	29	,	10	,	34	,	0	,0,0 ,0,0}, 
{	46	,	76	,	129	,	10	,0,0 ,0,0}, 
{	64	,	66	,	133	,	11	,0,0 ,0,0}, 
{	50	,	52	,	3	,	27	,0,0 ,0,0}, 
{	14	,	23	,	17	,	17	,0,0 ,0,0}, 
{	31	,	58	,	82	,	0	,0,0 ,0,0}, 
{	57	,	166	,	218	,	38	,0,0 ,0,0}, 
{	53	,	104	,	155	,	20	,0,0 ,0,0}, 
{	43	,	9	,	32	,	9	,0,0 ,0,0}, 
{	31	,	46	,	18	,	37	,0,0 ,0,0}, 
{	9	,	1	,	0	,	4	,0,0 ,0,0}, 
{	34	,	87	,	96	,	7	,0,0 ,0,0}, 
{	58	,	161	,	197	,	19	,0,0 ,0,0}, 
{	49	,	100	,	125	,	6	,0,0 ,0,0}, 
{	32	,	2	,	19	,	15	,0,0 ,0,0}, 
{	27	,	38	,	30	,	31	,0,0 ,0,0}, 
{	24	,	44	,	22	,	49	,0,0 ,0,0}, 
{	7	,	7	,	4	,	8	,0,0 ,0,0}, 
{	23	,	80	,	72	,	2	,0,0 ,0,0}, 
{	0	,	43	,	56	,	54	,0,0 ,0,0}, 
{	56	,	63	,	8	,	70	,0,0 ,0,0}, 
{	31	,	3	,	17	,	5	,0,0 ,0,0}, 
{	33	,	87	,	79	,	7	,0,0 ,0,0}, 
{	37	,	132	,	112	,	27	,0,0 ,0,0}, 
{	9	,	56	,	28	,	6	,0,0 ,0,0}, 
{	7	,	12	,	28	,	1	,0,0 ,0,0}, 
{	35	,	73	,	76	,	5	,0,0 ,0,0}, 
{	25	,	84	,	61	,	5	,0,0 ,0,0}, 
{	1	,	1	,	21	,	7	,0,0 ,0,0}, 
{	15	,	52	,	60	,	33	,0,0 ,0,0}, 
{	3	,	62	,	65	,	79	,0,0 ,0,0}, 
{	82	,	121	,	0	,	87	,0,0 ,0,0}, 
{	51	,	33	,	8	,	14	,0,0 ,0,0}, 
{	34	,	20	,	17	,	2	,0,0 ,0,0}, 
{	18	,	27	,	1	,	2	,0,0 ,0,0}, 
{	1	,	25	,	43	,	8	,0,0 ,0,0}, 
{	6	,	46	,	52	,	25	,0,0 ,0,0}, 
{	9	,	44	,	44	,	43	,0,0 ,0,0}, 
{	50	,	47	,	7	,	51	,0,0 ,0,0}, 
{	90	,	118	,	2	,	58	,0,0 ,0,0}, 
{	63	,	42	,	13	,	18	,0,0 ,0,0}, 
{	45	,	2	,	9	,	4	,0,0 ,0,0}, 
{	27	,	1	,	13	,	11	,0,0 ,0,0}, 
{	12	,	34	,	39	,	16	,0,0 ,0,0}, 
{	13	,	28	,	33	,	16	,0,0 ,0,0}, 
{	29	,	18	,	10	,	16	,0,0 ,0,0}, 
{	47	,	24	,	0	,	32	,0,0 ,0,0}, 
{	32	,	12	,	34	,	2	,0,0 ,0,0}, 
{	50	,	77	,	129	,	12	,0,0 ,0,0}, 
{	67	,	65	,	132	,	14	,0,0 ,0,0}, 
{	49	,	51	,	3	,	27	,0,0 ,0,0}, 
{	15	,	22	,	18	,	18	,0,0 ,0,0}, 
{	33	,	56	,	76	,	1	,0,0 ,0,0}, 
{	59	,	163	,	213	,	41	,0,0 ,0,0}
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

/*****
genann *genann_init( int * weights) {
    //if (hidden_layers < 0) return 0;
    //if (inputs < 1) return 0;
    //if (outputs < 1) return 0;
    //if (hidden_layers > 0 && hidden < 1) return 0;

    //2 1 2 1

    //if (!ret) return 0;

    ret->inputs = inputs_g;
    ret->hidden_layers = hidden_layers_g;
    ret->hidden = hidden_g;
    ret->outputs = outputs_g;

    ret->total_weights = total_weights_g;
    //ret->total_neurons = total_neurons;

    // Set pointers.
    //ret->weight = (int*)((char*)ret + sizeof(genann));
    ret->weight = weights;

    ret->output = ret->weight + ret->total_weights;
    //ret->delta = ret->output + ret->total_neurons;

    //genann_randomize(ret);

    //genann_init_sigmoid_lookup(ret);

    return ret;
}
**/



 int genann_activation( int a) {
    //assert(!isnan(a));
    a = a >> 8;
 //uint8_t  r = (uint8_t ) ((a/1024+1) *   (128));
	
    return   (a/1024+1) *   (128);

    /* Because floating point... */
    //if (unlikely(j >= LOOKUP_SIZE)) return lookup[LOOKUP_SIZE - 1];

}

/*
void genann_free(genann *ann) {
    // The weight, output, and delta pointers go to the same buffer.
    free(ann);
}*/


void genann_run(volatile int *inputs) {
    int const *w = weights;
    volatile int *o = inputs + ann_inputs;
    volatile int *i = inputs;

    /* Copy the inputs to the scratch area, where we also store each neuron's
     * output, for consistency. This way the first layer isn't a special case. */
    //memcpy(ann->output, inputs, sizeof(int) * ann->inputs);

    int h, j, k;

   if (!ann_hidden_layers) {//no hidden layers
        //int *ret = o;
        for (j = 0; j < ann_outputs; ++j) {
            int sum = *w++ * -1;
            sum = sum * 256;
            for (k = 0; k < ann_inputs; ++k) {
                sum += *w++ * i[k];
            }
            *o++ = genann_activation( sum);
        }
        return ;
   }

    /* Figure input layer */
    for (j = 0; j < ann_hidden; ++j) {
        int sum = *w++ * -1;
            sum = sum * 256;
        for (k = 0; k < ann_inputs; ++k) {
           //printf("in_pre_o w= %d i = %d\n", *w , i[k]);

            sum += *w++ * i[k];

            //printf("in_pre %d\n", sum);

        }
        *o++ = genann_activation( sum);
        //printf("in %d %d\n",  genann_activation(  sum), sum);

                        //printf("in %d\n",  genann_activation( sum));

    }

    i += ann_inputs;

    /* Figure hidden layers, if any. */
    for (h = 1; h < ann_hidden_layers; ++h) {
        for (j = 0; j < ann_hidden; ++j) {
            int sum = *w++ * -1;
            sum = sum * 256;
            for (k = 0; k < ann_hidden; ++k) {
                sum += *w++ * i[k];
            }
           *o++ = genann_activation( sum);
           // printf("hid %d %d\n",  genann_activation(  sum), sum);

        }

        i += ann_hidden;
    }
    //*o = *o << 20;

    //int const *ret = o;
    /* Figure output layer. */
    for (j = 0; j < ann_outputs; ++j) {
        int sum = *w++ * -1;
            sum = sum * 256;
        for (k = 0; k < ann_hidden; ++k) {
           //printf("out_pre_o w= %d i = %d\n", *w , i[k]);

            sum += *w++ * i[k];
       // printf("out_pre %d\n", sum);
        }
        *o++ = genann_activation( sum);

        //printf("out %d %d\n",  genann_activation(  sum), sum);

       // printf("out %d\n",  genann_activation(sum));
    }

    /* Sanity check that we used all weights and wrote all outputs. */
    //assert(w - ann->weight == ann->total_weights);
    //assert(o - ann->output == ann->total_neurons);
	return ;
}


int main(int argc, char *argv[])
{
//ret = malloc(size);
    /*printf("GENANN example 3.\n");
    printf("Load a saved ANN to solve the XOR function.\n");


    FILE *saved = fopen(save_name, "r");
    if (!saved) {
        printf("Couldn't open file: %s\n", save_name);
        exit(1);
    }

    genann *ann = genann_read(saved);
    fclose(saved);

    if (!ann) {
        printf("Error loading ANN from file: %s.", save_name);
        exit(1);
    } */
        //int inputs, int hidden_layers, int hidden, int outputs, int * weights)
    //genann *ann = genann_init(weights);
          // printf("abababa w= %d \n", ann->weight[4]) ;


    /* Input data for the XOR function. */

    /* Run the network and see what it predicts. */
    //printf("Output for [%d, %d] is %d.\n", input[0][0], input[0][1], *genann_run(ann, input[0]));
    //printf("Output for [%d, %d] is %d.\n", input[1][0], input[1][1], *genann_run(ann, input[1]));
    //printf("Output for [%d, %d] is %d.\n", input[2][0], input[2][1], *genann_run(ann, input[2]));
    //printf("Output for [%d, %d] is %d.\n", input[3][0], input[3][1], *genann_run(ann, input[3]));

 /*   volatile int results [50][2];
    int i;
	for (i = 0; i<20; i++){
		results[i] = *genann_run(ann, input[i]);
		printf("Output for the %d. result is %d\n", i+1,results[i]);
	}
*/
    //volatile static int results [300][2];
    //volatile static int real_results [300];

arr_str =  &input[0] ;
arr_end =  &input[INPUT_SIZE-1] + (&input[INPUT_SIZE-1] - &input[INPUT_SIZE-2]);

	startPar = 0x1;
	for (int j = 0; j < INPUT_SIZE; j++) {
            genann_run(input[j]);//result is last columns of input     in this case 6&7 

	}
	startPar = 0x0;

		//results[j][0] = *result++;
        //results[j][1] = *result;
        /*if (results[j][0] <128 && results[j][1] <128)
            real_results[j] = 1;
        else if (results[j][0] <128 && results[j][1] >= 128)
            real_results[j] = 2;
        else if (results[j][0] >= 128 && results[j][1] <128)
            real_results[j] = 3;
        else if (results[j][0] >= 128 && results[j][1] >=128)
            real_results[j] = 4;
        else real_results[j] = 0;
		#ifdef print_en
			printf("Output of input [%d,%d,%d,%d] output [%d %d] predicted [%d %d] is [%d %d] ",input[j][0],input[j][1],input[j][2],input[j][3],output[j][0], output[j][1], predicted[j][0],predicted[j][1], results[j][0],results[j][1]);
			printf("%d\n", real_results[j]);
		 #endif // print_en
		}

/*    results[0] = *genann_run(ann, input[0]);
    results[1] = *genann_run(ann, input[1]);
    results[2] =  *genann_run(ann, input[2]);
    results[3] = *genann_run(ann, input[3]);*/

    //printf("%d",results[1]);
/*
	*genann_run(ann, input[0]);//0
	*genann_run(ann, input[1]);//1
	*genann_run(ann, input[2]);//1
	*genann_run(ann, input[3]);//0 */

    //genann_free(ann);
    return 0;
}
