/* Compiler directives. */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_rng.h>

#include <queue.h>

#ifndef __UTIL_H
#define __UTIL_H
#include <util.h>
#endif

/* Number of simulations to run. */
#define NUMBER_OF_SIMULATIONS 100

/* When to cap the simulation. */
#define SIMULATION_CAP 500

/* Output CSV file. */
#define OUTPUT_CSV_FILE "result.csv"

/* Global variables. */

/* Random number generator. */
gsl_rng *RNG;

/* Structure definitions. */

/* Car structure, used for storing information about cars. */
struct car {
	unsigned int arrival_time;
};
typedef struct car CAR;

/* Traffic light structure, used for storing information about lights. */
struct traffic_light {
	unsigned int period;
	float arrival_rate;
	
	QUEUE *queue;
	BOOL is_green;

	unsigned int number_of_cars;
	float average_waiting_time;
	unsigned int maximum_waiting_time;
	BOOL queue_cleared;
	unsigned int time_to_clear_queue;
};
typedef struct traffic_light TRAFFIC_LIGHT;

/* Result structure, used for storing simulation results. */
struct result {
	unsigned int left_number_of_cars;
	float left_average_waiting_time;
	unsigned int left_maximum_waiting_time;
	unsigned int left_time_to_clear_queue;
	
	unsigned int right_number_of_cars;
	float right_average_waiting_time;
	unsigned int right_maximum_waiting_time;
	unsigned int right_time_to_clear_queue;
};
typedef struct result RESULT;

/* Function prototypes. */

void setup_rng();
double random();
float running_average(float average, unsigned int n, unsigned int x);

unsigned int get_period(char *string);
float get_arrival_rate(char *string);

CAR *new_car(unsigned int arrival_time);
TRAFFIC_LIGHT *new_traffic_light(unsigned int period, float arrival_rate);
RESULT *new_result();
RESULT *save_result(TRAFFIC_LIGHT *left, TRAFFIC_LIGHT *right);

void add_car_to_traffic_light(unsigned int count, TRAFFIC_LIGHT *traffic_light);
void drive_car_through_traffic_light(unsigned int count, TRAFFIC_LIGHT *traffic_light);
void update_time_to_clear_traffic_light(unsigned int count, TRAFFIC_LIGHT *traffic_light);
void output_traffic_light_statistics(TRAFFIC_LIGHT *traffic_light);
void output_result_statistics(RESULT *result);
void output_result_statistics_csv(RESULT *result, unsigned int left_period, float left_arrival_rate, unsigned int right_period, float right_arrival_rate);

RESULT *runOneSimulation(unsigned int left_period, float left_arrival_rate, unsigned int right_period, float right_arrival_rate);
RESULT *run_multiple_simulations(unsigned int left_period, float left_arrival_rate, unsigned int right_period, float right_arrival_rate);
