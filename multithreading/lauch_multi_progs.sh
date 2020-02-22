#!/bin/bash

taskset 1 ./testBp & taskset 2 ./testBp & taskset 4 ./testBp & taskset 8 ./testBp
