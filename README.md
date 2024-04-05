# Traffic Simulator

This project features a traffic simulator written in the C programming
language. This was primarily an exercise in learning C and understanding how
data structures (in particular, linked lists) can be programmed in C.

## About

This simulation models a traffic light with 2 queues: a left queue and a right
queue. During the runtime of the simulation, traffic flows through the traffic
lights depending on their current status. Traffic is represented using the FIFO
queue data structure, and traffic lights maintain some statistics giving the
average and maximum waiting times for each car at the traffic light, along with
the amount of time it takes to clear the queue.

## Requirements

This simulation is written in ANSI C and requires a C compiler (the [GNU C
Compiler](https://gcc.gnu.org/) - `gcc` - is recommended). Additionally, the
[GNU Scientific Library](https://www.gnu.org/software/gsl/) for generating
random numbers.

## Usage

To compile the program, use the `compileSim` Bash script supplied in this
repository. A copy of the binary program is also included in this repository.

## Extras

The `extras/` directory contains some extra files that can be used to analyse
the statistics of traffic moving through the simulation.
