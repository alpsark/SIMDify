/*****
 ** kmeans.c
 ** - a simple k-means clustering routine
 ** - returns the cluster labels of the data points in an array
 ** - here's an example
 **   extern int *k_means(double**, int, int, int, double, double**);
 **   ...
 **   int *c = k_means(data_points, num_points, dim, 20, 1e-4, 0);
 **   for (i = 0; i < num_points; i++) {
 **      printf("data point %d is in cluster %d\n", i, c[i]);
 **   }
 **   ...
 **   free(c);
 ** Parameters
 ** - array of data points (double **data)
 ** - number of data points (int n)
 ** - dimension (int m)
 ** - desired number of clusters (int k)
 ** - error tolerance (double t)
 **   - used as the stopping criterion, i.e. when the sum of
 **     squared euclidean distance (standard error for k-means)
 **     of an iteration is within the tolerable range from that
 **     of the previous iteration, the clusters are considered
 **     "stable", and the function returns
 **   - a suggested value would be 0.0001
 ** - output address for the final centroids (double **centroids)
 **   - user must make sure the memory is properly allocated, or
 **     pass the null pointer if not interested in the centroids
 ** References
 ** - J. MacQueen, "Some methods for classification and analysis
 **   of multivariate observations", Fifth Berkeley Symposium on
 **   Math Statistics and Probability, 281-297, 1967.
 ** - I.S. Dhillon and D.S. Modha, "A data-clustering algorithm
 **   on distributed memory multiprocessors",
 **   Large-Scale Parallel Data Mining, 245-260, 1999.
 ** Notes
 ** - this function is provided as is with no warranty.
 ** - the author is not responsible for any damage caused
 **   either directly or indirectly by using this function.
 ** - anybody is free to do whatever he/she wants with this
 **   function as long as this header section is preserved.
 ** Created on 2005-04-12 by
 ** - Roger Zhang (rogerz@cs.dal.ca)
 ** Modifications
 ** -
 ** Last compiled under Linux with gcc-3
 */
 /*
 ** src: http://cs.smu.ca/~r_zhang/code/kmeans.c
 */

static int IMAX = 0x7FFFFFFF ;
#include <stdio.h>
#include <stdlib.h>
 #include <assert.h>
 #include <float.h>
 #include <math.h>

 volatile int __attribute__((section(".mySection_par"))) startPar =  0x00 ;
volatile int __attribute__((section(".mySection_par_num"))) par_num =  0x03 ; //constant
volatile int __attribute__((section(".mySection_arr_str"))) arr_str =  0x00 ;
volatile int __attribute__((section(".mySection_arr_end"))) arr_end =  0x00 ;


 #define num_points  150
 #define feature_number  4
 #define number_of_clusters  2
 #define distance_offset 4
 //features / distances for each cluster 
static int arr [150][feature_number+number_of_clusters] = {
{	616	,	3894	,	4727	,	4394		,0,0},
{	5441	,	7236	,	109	,	7564		,0,0},
{	300	,	173	,	1754	,	489				,0,0},
{	1340	,	4132	,	4701	,	2598	,0,0},
{	268	,	4987	,	5150	,	6391		,0,0},
{	6523	,	9601	,	253	,	6964		,0,0},
{	4067	,	2495	,	795	,	1103		,0,0},
{	2564	,	1705	,	1502	,	327		,0,0},
{	1341	,	1916	,	155	,	119			,0,0},
{	238	,	2087	,	3352	,	665			,0,0},
{	621	,	3677	,	4077	,	2071		,0,0},
{	778	,	3401	,	3485	,	3556		,0,0},
{	4124	,	3790	,	653	,	4180		,0,0},
{	7042	,	9200	,	259	,	4683		,0,0},
{	4946	,	3571	,	827	,	1495		,0,0},
{	3535	,	304	,	656	,	295				,0,0},
{	2066	,	169	,	1005	,	829			,0,0},
{	887	,	2808	,	3143	,	1203		,0,0},
{	1094	,	2307	,	2523	,	1445	,0,0},
{	2445	,	1628	,	885	,	1480		,0,0},
{	3551	,	1895	,	186	,	2440		,0,0},
{	2281	,	856	,	2718	,	44			,0,0},
{	3605	,	5974	,	10091	,	828		,0,0},
{	5067	,	5187	,	10426	,	867		,0,0},
{	3920	,	4072	,	236	,	2115		,0,0},
{	1130	,	1845	,	1357	,	1380	,0,0},
{	2456	,	4556	,	6453	,	56		,0,0},
{	4477	,	13030	,	17083	,	3034	,0,0},
{	4195	,	8181	,	12129	,	1601	,0,0},
{	3380	,	707	,	2532	,	718			,0,0},
{	2436	,	3602	,	1416	,	2894	,0,0},
{	776	,	130	,	31	,	353					,0,0},
{	2708	,	6826	,	7533	,	590		,0,0},
{	4553	,	12585	,	15441	,	1498	,0,0},
{	3889	,	7832	,	9820	,	474		,0,0},
{	2508	,	227	,	1488	,	1206		,0,0},
{	2165	,	3021	,	2413	,	2424	,0,0},
{	1897	,	3507	,	1784	,	3849	,0,0},
{	620	,	558	,	385	,	664					,0,0},
{	1838	,	6301	,	5650	,	195		,0,0},
{	25	,	3399	,	4432	,	4265		,0,0},
{	4396	,	4960	,	648	,	5471		,0,0},
{	2456	,	244	,	1404	,	458			,0,0},
{	2620	,	6855	,	6216	,	622		,0,0},
{	2966	,	10325	,	8784	,	2113	,0,0},
{	714	,	4444	,	2224	,	498			,0,0},
{	600	,	999	,	2212	,	97				,0,0},
{	2790	,	5713	,	5953	,	459		,0,0},
{	1998	,	6607	,	4825	,	419		,0,0},
{	154	,	117	,	1682	,	595				,0,0},
{	1208	,	4074	,	4763	,	2612	,0,0},
{	295	,	4885	,	5149	,	6232		,0,0},
{	6424	,	9531	,	22	,	6851		,0,0},
{	3993	,	2621	,	628	,	1159		,0,0},
{	2659	,	1605	,	1364	,	164		,0,0},
{	1409	,	2125	,	103	,	192			,0,0},
{	110	,	1974	,	3366	,	652			,0,0},
{	523	,	3644	,	4074	,	1990		,0,0},
{	767	,	3459	,	3440	,	3427		,0,0},
{	3969	,	3681	,	600	,	4013		,0,0},
{	7036	,	9293	,	165	,	4539		,0,0},
{	4944	,	3300	,	1063	,	1444	,0,0},
{	3593	,	229	,	712	,	333				,0,0},
{	2167	,	124	,	1046	,	862			,0,0},
{	960	,	2696	,	3122	,	1312		,0,0},
{	1080	,	2199	,	2586	,	1275	,0,0},
{	2327	,	1438	,	821	,	1286		,0,0},
{	3724	,	1903	,	35	,	2509		,0,0},
{	2572	,	956	,	2707	,	166			,0,0},
{	3929	,	6081	,	10095	,	1014	,0,0},
{	5294	,	5146	,	10333	,	1118	,0,0},
{	3895	,	4039	,	301	,	2183		,0,0},
{	1224	,	1748	,	1480	,	1418	,0,0},
{	2640	,	4415	,	5983	,	139		,0,0},
{	4633	,	12750	,	16716	,	3216	,0,0},
{	4288	,	7863	,	11838	,	1897	,0,0},
{	3345	,	504	,	2632	,	537			,0,0},
{	1118	,	3335	,	1345	,	1957	,0,0},
{	559	,	310	,	183	,	446					,0,0},
{	1507	,	7319	,	7898	,	1228	,0,0},
{	3506	,	12566	,	15160	,	752		,0,0},
{	2949	,	8273	,	10264	,	1162	,0,0},
{	1602	,	389	,	1620	,	1910		,0,0},
{	1266	,	2818	,	2426	,	1886	,0,0},
{	492	,	3060	,	1835	,	2834		,0,0},
{	663	,	45	,	187	,	234					,0,0},
{	720	,	6758	,	5840	,	623			,0,0},
{	1996	,	9500	,	9682	,	128		,0,0},
{	973	,	6416	,	5602	,	1032		,0,0},
{	25	,	173	,	113	,	1219				,0,0},
{	62	,	2930	,	3546	,	2673		,0,0},
{	5525	,	6325	,	897	,	6624		,0,0},
{	1294	,	2673	,	840	,	2032		,0,0},
{	240	,	1783	,	2135	,	1241		,0,0},
{	1396	,	9669	,	9465	,	348		,0,0},
{	2967	,	13286	,	13472	,	2627	,0,0},
{	1100	,	7250	,	6013	,	368		,0,0},
{	224	,	521	,	403	,	1201				,0,0},
{	904	,	3370	,	4498	,	3696		,0,0},
{	2861	,	4519	,	581	,	4262		,0,0},
{	1083	,	312	,	1281	,	412			,0,0},
{	1568	,	7244	,	6553	,	127		,0,0},
{	2054	,	10867	,	9492	,	1411	,0,0},
{	234	,	4515	,	2119	,	1444		,0,0},
{	1581	,	869	,	2313	,	824			,0,0},
{	1551	,	3801	,	4914	,	3748	,0,0},
{	4147	,	7122	,	83	,	6417		,0,0},
{	2262	,	99	,	2812	,	486			,0,0},
{	1747	,	5823	,	5869	,	1212	,0,0},
{	959	,	6712	,	4985	,	328			,0,0},
{	1345	,	235	,	1878	,	1325		,0,0},
{	2227	,	4095	,	4803	,	2111	,0,0},
{	1257	,	4873	,	5286	,	5874	,0,0},
{	5385	,	9121	,	419	,	5918		,0,0},
{	2978	,	2344	,	526	,	401			,0,0},
{	1585	,	2156	,	1708	,	901		,0,0},
{	218	,	2203	,	95	,	564				,0,0},
{	1318	,	1901	,	3311	,	65		,0,0},
{	1489	,	3428	,	4030	,	1425	,0,0},
{	115	,	3221	,	3430	,	2845		,0,0},
{	3392	,	3356	,	720	,	3523		,0,0},
{	6163	,	8709	,	216	,	3634		,0,0},
{	4078	,	2923	,	870	,	653			,0,0},
{	2589	,	391	,	934	,	429				,0,0},
{	1011	,	190	,	905	,	3				,0,0},
{	66	,	2491	,	2940	,	621			,0,0},
{	247	,	1936	,	2469	,	805			,0,0},
{	1573	,	1063	,	712	,	838			,0,0},
{	2166	,	1593	,	45	,	1678		,0,0},
{	1166	,	1423	,	2924	,	661		,0,0},
{	2839	,	6630	,	10484	,	421		,0,0},
{	4504	,	5812	,	10886	,	528		,0,0},
{	2410	,	3743	,	402	,	1295		,0,0},
{	406	,	1349	,	1450	,	559			,0,0},
{	1388	,	4877	,	6477	,	341		,0,0},
{	3750	,	13458	,	17593	,	2777	,0,0},
{	3563	,	8382	,	12393	,	1282	,0,0},
{	2541	,	658	,	2684	,	1195		,0,0},
{	1666	,	300	,	1423	,	24			,0,0},
{	2096	,	7105	,	6618	,	337		,0,0},
{	2668	,	10451	,	9113	,	1732	,0,0},
{	474	,	4349	,	1990	,	751			,0,0},
{	1055	,	1185	,	2641	,	110		,0,0},
{	1164	,	3809	,	4940	,	4090	,0,0},
{	4477	,	7370	,	312	,	6775		,0,0},
{	2733	,	455	,	2439	,	217			,0,0},
{	2286	,	5448	,	5803	,	882		,0,0},
{	1624	,	6344	,	4657	,	169		,0,0},
{	508	,	477	,	1980	,	577				,0,0},
{	1640	,	4250	,	4902	,	2662	,0,0},
};

static     int  labels[num_points];
static     int  c[number_of_clusters][feature_number];
static     int  c1[number_of_clusters][feature_number];
static int counts[number_of_clusters];
//k_means( num_points, feature_number,arr, number_of_clusters, 1e-4 );
  volatile  int * k_means( int t ) {
        /* output cluster label for each data point */
		
       volatile  int old_error = IMAX; /* sum of squared euclidean distance */
		volatile int error = IMAX-1;
       volatile int cluster[num_points];
       // assert(data && k > 0 && k <= n && m > 0 && t >= 0); /* for debugging */

        /****
         ** initialization */
		int count = 0;
        for (int i = 0; i < number_of_clusters;  i++) {
            /* pick k points as initial centroids */
            //printf("data point %d is in cluster \n", c[i][0]);
            for (int j = feature_number; j-- > 0; c[i][j] = arr[count][j]);
			count += num_points / number_of_clusters;
        }

        /****
         ** main loop */

        while (error < old_error  &&  error > t){
			
            /* save error from last step */
            old_error = error, error = 0;

            /* clear old counts and temp centroids */
            for (int i = 0; i < number_of_clusters; counts[i++] = 0) {
                for (int j = 0; j < feature_number; j++ ){
					c1[i][j] = 0;
				}
			}		

			startPar = 0x1;		
            for (int h = 0; h < num_points; h++) {
                /* identify the closest cluster */				
                for (int i = 0; i < number_of_clusters; i++) {
				   for (int j = 0; j < feature_number; j++){
					   arr[h][distance_offset+i] = arr[h][distance_offset+i] + (arr[h][j] - c[i][j])*(arr[h][j] - c[i][j]);
					   }
                }
			}	
			startPar = 0x0;

			for (int h = 0; h < num_points; h++) {
				//find minimum distance
				unsigned int min_distance = IMAX;
                for (int i = 0; i < number_of_clusters; i++) {					
					if ( arr[h][distance_offset+i] < min_distance) {
							labels[h] = i;
						   // printf("data point %d is in cluster %d\n", h, labels[h]);
							min_distance = arr[h][distance_offset+i];
						}
				}
				
                /* update size and temp centroid of the destination cluster */
                for (int j = feature_number; j > 0; j--){
						c1[labels[h]][j] += arr[h][j];
					}
                counts[labels[h]]++;
                /* update standard error */
                error += min_distance;

            }

            for (int i = 0; i < number_of_clusters; i++) { /* update all centroids */
                for (int j = 0; j < feature_number; j++) {
					unsigned int num2 = counts[i];
					if (num2 == 0 ){
						c[i][j] = c1[i][j];
					}else {
					//spoof division 
						unsigned int num1 =  c1[i][j];
						unsigned int temp = 1, quotient = 0;
						while (num2 <= num1){
							int num2shft = num2 << 1;
							if (num2shft == 0 ){
							num1 >>= 1;	
							break;							
							}
							num2 <<= 1;
							temp <<= 1;
						}
						while (temp > 1){
							num2 >>= 1;
							temp >>= 1;
							if(num1 >= num2)
							{
							num1 -= num2;
							quotient += temp;
							}
						}
						c[i][j] =quotient;
					}						
					
					
                }
            }


    } 


	for (int i = 0; i < num_points; i++) {
	  cluster[i] = labels[i];
	 // printf("data point %d is in cluster %d\n", i, cluster[i]);
	}


       return labels;
}



	int main() {

	/* **   extern int *k_means(double**, int, int, int, double, double**);
	- array of data points (double **data)
 ** - number of data points (int n)
 ** - dimension (int m)
 ** - desired number of clusters (int k)
 ** - error tolerance (double t)
 **   - used as the stopping criterion, i.e. when the sum of
 **     squared euclidean distance (standard error for k-means)
 **     of an iteration is within the tolerable range from that
 **     of the previous iteration, the clusters are considered
 **     "stable", and the function returns
 **   - a suggested value would be 0.0001
 ** - output address for the final centroids (double **centroids)
 **   - user must make sure the memory is properly allocated, or
 **     pass the null pointer if not interested in the centroids  */
	arr_str =  &arr[0] ;
	arr_end =  &arr[149] + (&arr[149] - &arr[148]);
			
volatile	int * z = k_means(   100 );
	/*for (int i = 0; i < num_points; i++) {
      printf("data point %d is in cluster %d\n", i, z[i]);
    }*/


	// FILE *myfile = fopen("result_kmeans_first.txt", "w");
	// for (int i = 0; i < num_points; i++) {
      // fprintf(myfile, "data point %d is in cluster %d\n", i, z[i]);
	// }

    return 0;

	}
