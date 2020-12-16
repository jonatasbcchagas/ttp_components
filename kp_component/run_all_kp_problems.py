#!/usr/bin/python
# -*- coding: utf-8 -*-

import itertools
import os
import multiprocessing
import re

def get_opt_value(tsp_base, n_items_per_city, type_of_items):
    num_items = (int(re.sub('\D', '', tsp_base)) - 1) * n_items_per_city
    if os.path.isfile("kp_opt_sols/%s_n%d_%s_10.opt.kp.value" % (tsp_base, num_items, type_of_items)):
        return
    os.system("./KP ../ttp_instances/%s-ttp/%s_n%d_%s kp_opt_sols/%s_n%d_%s" % (tsp_base, tsp_base, num_items, type_of_items, tsp_base, num_items, type_of_items))

def get_opt_solution(tsp_base, n_items_per_city, type_of_items, delta):
    num_items = (int(re.sub('\D', '', tsp_base)) - 1) * n_items_per_city
    for s in range(1, 11):
        if os.path.isfile("kp_opt_sols/%s_n%d_%s_%02d.opt.kp.value" % (tsp_base, num_items, type_of_items, s)) and not os.path.isfile("kp_opt_sols/%s_n%d_%s_%02d.opt.kp.sol" % (tsp_base, num_items, type_of_items, s)):
            os.system("./KP ../ttp_instances/%s-ttp/%s_n%d_%s_%02d.ttp kp_opt_sols/%s_n%d_%s_%02d.opt.kp.value kp_opt_sols/%s_n%d_%s_%02d.opt.kp.sol %d &" % (tsp_base, tsp_base, num_items, type_of_items, s, tsp_base, num_items, type_of_items, s, tsp_base, num_items, type_of_items, s, delta))

if __name__ == "__main__":

    tsp_bases = ["eil51", "berlin52", "st70", "eil76", "pr76", "rat99", "kroA100", "kroB100", "kroC100", "kroD100", "kroE100", "rd100", "eil101", "lin105", "pr107", "pr124", "bier127", "ch130", "pr136", "pr144", "ch150", "kroA150", "kroB150", "pr152", "u159", "rat195", "d198", "kroA200", "kroB200", "ts225", "tsp225", "pr226", "gil262", "pr264", "a280", "pr299", "lin318", "rd400", "fl417", "pr439", "pcb442", "d493", "u574", "rat575", "p654", "d657", "u724", "rat783", "dsj1000", "pr1002", "u1060", "vm1084", "pcb1173", "d1291", "rl1304", "rl1323", "nrw1379", "fl1400", "u1432", "fl1577", "d1655", "vm1748", "u1817", "rl1889", "d2103", "u2152", "u2319", "pr2392", "pcb3038", "fl3795", "fnl4461", "rl5915", "rl5934", "pla7397", "rl11849", "usa13509", "brd14051", "d15112", "d18512", "pla33810", "pla85900", ]

    number_of_items_per_city = [1, 3, 5, 10, ]

    types_of_items = ["bounded-strongly-corr", "uncorr", "uncorr-similar-weights", ]

    #os.system("make")


    '''
    for comb in itertools.product(tsp_bases, number_of_items_per_city, types_of_items):
        tsp_base, n_items_per_city, type_of_items = comb
        pool.apply_async(get_opt_value, args=(tsp_base, n_items_per_city, type_of_items))
    '''

    delta = 500000

    pool = multiprocessing.Pool(processes=30)

    for comb in itertools.product(tsp_bases, number_of_items_per_city, types_of_items):
        tsp_base, n_items_per_city, type_of_items = comb
        pool.apply_async(get_opt_solution, args=(tsp_base, n_items_per_city, type_of_items,delta))

    pool.close()
    pool.join()
