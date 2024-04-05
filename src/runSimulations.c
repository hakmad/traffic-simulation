/* Compiler directives. */

#include <runSimulations.h>

/* Main program. */

int main(int argc, char *argv[]) {
	/* Setup random number generator. */
	setup_rng();

	/* Create variables for running simulations. */
	unsigned int left_period = 0;
	float left_arrival_rate = 0;
	unsigned int right_period = 0;
	float right_arrival_rate = 0;

	/* Get command line arguments. */
	if (!(argc == 5)) {
		/* Invalid number of arguments supplied. */
		fprintf(stderr, "Fatal! Incorrect number of arguments supplied.\n");
		exit(EINVAL);
	}
	else {
		/* Get periods and arrival rates. */
		left_period = get_period(argv[1]);
		left_arrival_rate = get_arrival_rate(argv[2]);
		right_period = get_period(argv[3]);
		right_arrival_rate = get_arrival_rate(argv[4]);
	}

	/* Perform simulations. */
	RESULT *average = run_multiple_simulations(left_period, left_arrival_rate, right_period, right_arrival_rate);

	/* Show information about parameter values. */
	printf("Parameter values:\n");
	printf("\tFrom left:\n");
	printf("\t\tTraffic light period: %d\n", left_period);
	printf("\t\tTraffic arrival rate: %.2f\n", left_arrival_rate);
	printf("\tFrom right:\n");
	printf("\t\tTraffic light period: %d\n", right_period);
	printf("\t\tTraffic arrival rate: %.2f\n", right_arrival_rate);

	/* Show results. */
	output_result_statistics(average);

	/* Output data in CSV format. */
	output_result_statistics_csv(average, left_period, left_arrival_rate, right_period, right_arrival_rate);

	/* Free allocated memory. */

	/* Free average. */
	free(average);

	/* Free random number generator. */
	gsl_rng_free(RNG);

	/* Exit program. */
	return 0;
}

/* Function definitions. */

/* Setup random number generator. */
void setup_rng() {
	/* Initialise random number generator. */
	const gsl_rng_type *T;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	RNG = gsl_rng_alloc(T);

	/* Seed random number generator. */
	gsl_rng_set(RNG, time(0));
}

/* Generate random number in the range [0, 1). */
double random() {
	/* Use GSL to return random number. */
	return gsl_rng_uniform(RNG);
}

/* Return a running average for a set of values. */
float running_average(float average, unsigned int n, unsigned int x) {
	/* Calculate and return average. */
	return ((average * n) + x) / (n + 1);
}

/* Get the period from a string. */
unsigned int get_period(char *string) {
	/* Create variables. */
	char *endptr;
	errno = 0;
	
	/* Attempt to get period from string using base 10. */
	unsigned int period = strtol(string, &endptr, 10);

	/* Failure occurred (where?). */
	if (errno != 0) {
		perror("strtol");
		exit(EXIT_FAILURE);
	}

	/* String has no digits. */
	if (endptr == string) {
		fprintf(stderr, "Fatal! No digits found.\n");
		exit(EXIT_FAILURE);
	}

	/* Period is negative. */
	if (period < 0) {
		fprintf(stderr, "Fatal! Invalid argument supplied (period was negative).\n");
		exit(EINVAL);
	}

	/* Return the period. */
	return period;
}

/* Get the arrival rate from a string. */
float get_arrival_rate(char *string) {
	/* Create variables. */
	char *endptr;
	errno = 0;

	/* Attempt to get arrival rate from string. */
	float arrival_rate = strtod(string, &endptr);

	/* Failure occurred (where?). */
	if (errno != 0) {
		perror("strtod");
		exit(EXIT_FAILURE);
	}

	/* String has no digits. */
	if (endptr == string) {
		fprintf(stderr, "Fatal! No digits found.\n");
		exit(EXIT_FAILURE);
	}

	/* Arrival rate not between 0 and 1. */
	if (arrival_rate < 0 || arrival_rate > 1) {
		fprintf(stderr, "Fatal! Invalid argument supplied (arrival rate not between 0 and 1).\n");
		exit(EINVAL);
	}

	/* Return the arrival rate. */
	return arrival_rate;
}

/* Create a new car. */
CAR *new_car(unsigned int arrival_time) {
	/* Allocate memory for car structure. */
	CAR *car = (CAR *) safe_malloc(sizeof(CAR));
	
	/* Set car attributes. */
	car->arrival_time = arrival_time;

	/* Return new car. */
	return car;
}

/* Create a new traffic light. */
TRAFFIC_LIGHT *new_traffic_light(unsigned int period, float arrival_rate) {
	/* Allocate memory for traffic light structure. */
	TRAFFIC_LIGHT *traffic_light = (TRAFFIC_LIGHT *) safe_malloc(sizeof(TRAFFIC_LIGHT));

	/* Set traffic light attributes. */
	traffic_light->period = period;
	traffic_light->arrival_rate = arrival_rate;

	traffic_light->queue = new_queue();
	traffic_light->is_green = false;

	traffic_light->number_of_cars = 0;
	traffic_light->average_waiting_time = 0;
	traffic_light->maximum_waiting_time = 0;
	traffic_light->queue_cleared = false;
	traffic_light->time_to_clear_queue = 0;

	/* Return new traffic light. */
	return traffic_light;
}

/* Create a new result object with all values set to 0. */
RESULT *new_result() {
	/* Allocate memory for result structure. */
	RESULT *result = (RESULT *) safe_malloc(sizeof(RESULT));

	/* Set result attributes. */
	result->left_number_of_cars = 0;
	result->left_average_waiting_time = 0;
	result->left_maximum_waiting_time = 0;
	result->left_time_to_clear_queue = 0;

	result->right_number_of_cars = 0;
	result->right_average_waiting_time = 0;
	result->right_maximum_waiting_time = 0;
	result->right_time_to_clear_queue = 0;

	/* Return new result. */
	return result;
}

/* Save the result of a simulation to a structure. */
RESULT *save_result(TRAFFIC_LIGHT *left, TRAFFIC_LIGHT *right) {
	/* Allocate memory for result structure. */
	RESULT *result = (RESULT *) safe_malloc(sizeof(RESULT));
	
	/* Set result attributes. */
	result->left_number_of_cars = left->number_of_cars;
	result->left_average_waiting_time = left->average_waiting_time;
	result->left_maximum_waiting_time = left->maximum_waiting_time;
	result->left_time_to_clear_queue = left->time_to_clear_queue;

	result->right_number_of_cars = right->number_of_cars;
	result->right_average_waiting_time = right->average_waiting_time;
	result->right_maximum_waiting_time = right->maximum_waiting_time;
	result->right_time_to_clear_queue = right->time_to_clear_queue;

	/* Return new result. */
	return result;
}

/* Add a car to a traffic lights queue. */
void add_car_to_traffic_light(unsigned int count, TRAFFIC_LIGHT *traffic_light) {
	/* Compare arrival rate with a random number. */
	if (traffic_light->arrival_rate > random()) {
		/* Create a new car and add it to the traffic lights queue. */
		CAR *car = new_car(count);
		enqueue(traffic_light->queue, car);
	}
}

/* Drive a car through the traffic lights. */
void drive_car_through_traffic_light(unsigned int count, TRAFFIC_LIGHT *traffic_light) {
	/* Check queue of traffic light. */
	if (!(is_empty(traffic_light->queue))) {
		/* Queue is not empty, drive car through. */
		CAR *car = dequeue(traffic_light->queue);

		/* Update statistics. */

		/* Update maximum waiting time. */
		unsigned int waiting_time = count - car->arrival_time;
		if (waiting_time > traffic_light->maximum_waiting_time) {
			traffic_light->maximum_waiting_time = waiting_time;
		}

		/* Update average. */
		traffic_light->average_waiting_time = running_average(traffic_light->average_waiting_time,
				traffic_light->number_of_cars, waiting_time);

		/* Update number of cars. */
		traffic_light->number_of_cars++;

		/* Free allocated memory. */
		free(car);
	}
}

/* Update the time taken to clear a traffic light. */
void update_time_to_clear_traffic_light(unsigned int count, TRAFFIC_LIGHT *traffic_light) {
	/* Check if queue is empty and flag has not been set. */
	if (is_empty(traffic_light->queue) && !(traffic_light->queue_cleared)) {
		/* Update time to clear queue. */
		traffic_light->time_to_clear_queue = count - SIMULATION_CAP;
		traffic_light->queue_cleared = true;
	}
}

/* Output statistics for traffic lights. */
void output_traffic_light_statistics(TRAFFIC_LIGHT *traffic_light) {
	printf("Number of cars: %d\n", traffic_light->number_of_cars);
	printf("Average waiting time: %.2f\n", traffic_light->average_waiting_time);
	printf("Maximum waiting time: %d\n", traffic_light->maximum_waiting_time);
	printf("Time to clear queue: %d\n", traffic_light->time_to_clear_queue);
}

/* Output statistics from a result. */
void output_result_statistics(RESULT *result) {
	printf("Results (averaged over %d runs):\n", NUMBER_OF_SIMULATIONS);

	printf("\tFrom left:\n");
	printf("\t\tNumber of cars: %d\n", result->left_number_of_cars);
	printf("\t\tAverage waiting time: %.2f\n", result->left_average_waiting_time);
	printf("\t\tMaximum waiting time: %d\n", result->left_maximum_waiting_time);
	printf("\t\tTime to clear queue: %d\n", result->left_time_to_clear_queue);

	printf("\tFrom right:\n");
	printf("\t\tNumber of cars: %d\n", result->right_number_of_cars);
	printf("\t\tAverage waiting time: %.2f\n", result->right_average_waiting_time);
	printf("\t\tMaximum waiting time: %d\n", result->right_maximum_waiting_time);
	printf("\t\tTime to clear queue: %d\n", result->right_time_to_clear_queue);
}

/* Output statistics from a result to a CSV file. */
void output_result_statistics_csv(RESULT *result, unsigned int left_period, float left_arrival_rate, unsigned int right_period, float right_arrival_rate) {
	/* Open file for appending. */
	FILE *f = fopen(OUTPUT_CSV_FILE, "a");

	/* Check if file was opened successfully. */
	if (f == NULL) {
		/* File was not opened successfully, report error and exit. */
		perror("fopen");
		fprintf(stderr, "Fatal! Could not open file for writing.\n");
		exit(EIO);
	}

	/* Append results to file. */
	fprintf(f, "%d,%.2f,%d,%.2f,", left_period, left_arrival_rate, right_period, right_arrival_rate);
	fprintf(f, "%d,", result->left_number_of_cars);
	fprintf(f, "%.2f,", result->left_average_waiting_time);
	fprintf(f, "%d,", result->left_maximum_waiting_time);
	fprintf(f, "%d,", result->left_time_to_clear_queue);
	fprintf(f, "%d,", result->right_number_of_cars);
	fprintf(f, "%.2f,", result->right_average_waiting_time);
	fprintf(f, "%d,", result->right_maximum_waiting_time);
	fprintf(f, "%d,", result->right_time_to_clear_queue);
	fprintf(f, "\n");

	/* Close the file. */
	fclose(f);
}

/* Run a single simulation. */
RESULT *runOneSimulation(unsigned int left_period, float left_arrival_rate, unsigned int right_period, float right_arrival_rate) {
	/* Check if parameters are valid. */
	if ((left_period < 0) || (right_period < 0)) {
		/* Periods less than 0. */
		fprintf(stderr, "Fatal! Invalid argument supplied (period was negative).\n");
		exit(EINVAL);
	}
	if ((left_arrival_rate < 0 || left_arrival_rate > 1) || (right_arrival_rate < 0 || right_arrival_rate > 1)) {
		/* Arrival rates not between 0 and 1. */
		fprintf(stderr, "Fatal! Invalid argument supplied (arrival rate not between 0 and 1).\n");
		exit(EINVAL);
	}

	/* Create environment variables used in simulation. */
	BOOL done = false;
	BOOL new_arrivals = true;
	unsigned int count = 0;
	unsigned int light_counter;

	/* Setup traffic lights. */
	TRAFFIC_LIGHT *left_traffic_light = new_traffic_light(left_period, left_arrival_rate);
	TRAFFIC_LIGHT *right_traffic_light = new_traffic_light(right_period, right_arrival_rate);

	/* Set left traffic light to green. */
	left_traffic_light->is_green = true;
	light_counter = left_traffic_light->period;
	
	/* Run simulation. */
	while (!(done)) {
		/* Check if lights need to be changed. */
		if (light_counter == 0) {
			/* Lights need to be changed. */

			/* Check which light is currently green. */
			if (left_traffic_light->is_green) {
				/* Left light green, reverse lights. */
				left_traffic_light->is_green = false;
				right_traffic_light->is_green = true;

				/* Update counter for switching lights to right light period. */
				light_counter = right_traffic_light->period + 1;
			}
			else if (right_traffic_light->is_green) {
				/* Right light green, reverse lights. */
				right_traffic_light->is_green = false;
				left_traffic_light->is_green = true;

				/* Update counter for switching lights to left light period. */
				light_counter = left_traffic_light->period + 1;
			}
		}
		else {
			/* No need to change lights. Run simulation. */

			/* Add cars to traffic lights. */
			if (new_arrivals) {
				/* Add cars to left traffic light. */
				add_car_to_traffic_light(count, left_traffic_light);

				/* Add cars to right traffic light. */
				add_car_to_traffic_light(count, right_traffic_light);
			}

			/* Drive cars through protected area depending on lights. */
			if (left_traffic_light->is_green) {
				/* Left traffic light is green, drive cars through. */
				drive_car_through_traffic_light(count, left_traffic_light);
			}
			else if (right_traffic_light->is_green) {
				/* Right traffic light is green, drive cars through. */
				drive_car_through_traffic_light(count, right_traffic_light);
			}
		}

		/* Check how many iterations have passed. */
		if (count > SIMULATION_CAP) {
			/* Prevent more cars from arriving. */
			new_arrivals = false;

			/* Update times to clear left and right traffic lights. */
			update_time_to_clear_traffic_light(count, left_traffic_light);
			update_time_to_clear_traffic_light(count, right_traffic_light);
		}

		/* Check if simulation is complete. */
		if (!(new_arrivals) && is_empty(left_traffic_light->queue) && is_empty(right_traffic_light->queue)) {
			/* No new arrivals, both queues empty - stop the simulation. */
			done = true;
		}

		/* Check if simulation has somehow entered an erroneous state. */
		if (left_traffic_light->is_green == right_traffic_light->is_green) {
			/* Somehow (?) both traffic lights are the same. */ 
			fprintf(stderr, "Fatal! Simulation has entered erroneous state (traffic lights in same state).\n");
			exit(EXIT_FAILURE);
		}

		/* Update counters. */
		count++;
		light_counter--;
	}

	/* Save result. */
	RESULT *result = save_result(left_traffic_light, right_traffic_light);

	/* Free allocated memory. */

	/* Free queues. */
	free(left_traffic_light->queue);
	free(right_traffic_light->queue);

	/* Free actual traffic lights. */
	free(left_traffic_light);
	free(right_traffic_light);

	/* Return result. */
	return result;
}

/* Run a simulation multiple times. */
RESULT *run_multiple_simulations(unsigned int left_period, float left_arrival_rate, unsigned int right_period, float right_arrival_rate) {
	/* Create empty result structure to store average results. */
	RESULT *average = new_result();

	/* Repeat simulation multiple times, update running average for results. */
	int i;
	for (i = 0; i < NUMBER_OF_SIMULATIONS; i++) {
		/* Perform one simulation. */
		RESULT *r = runOneSimulation(left_period, left_arrival_rate, right_period, right_arrival_rate);

		/* Update left statistics. */
		average->left_number_of_cars = running_average(average->left_number_of_cars, i, r->left_number_of_cars);
		average->left_average_waiting_time = running_average(average->left_average_waiting_time, i, r->left_average_waiting_time);
		average->left_maximum_waiting_time = running_average(average->left_maximum_waiting_time, i, r->left_maximum_waiting_time);
		average->left_time_to_clear_queue = running_average(average->left_time_to_clear_queue, i, r->left_time_to_clear_queue);

		/* Update right statistics. */
		average->right_number_of_cars = running_average(average->right_number_of_cars, i, r->right_number_of_cars);
		average->right_average_waiting_time = running_average(average->right_average_waiting_time, i, r->right_average_waiting_time);
		average->right_maximum_waiting_time = running_average(average->right_maximum_waiting_time, i, r->right_maximum_waiting_time);
		average->right_time_to_clear_queue = running_average(average->right_time_to_clear_queue, i, r->right_time_to_clear_queue);

		/* Free allocated memory. */
		free(r);
	}

	/* Return the average result. */
	return average;
}
