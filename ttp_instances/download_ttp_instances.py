import argparse
import requests

def download_instances(tsp_bases):

    base_url = "https://cs.adelaide.edu.au/~optlog/CEC2014COMP_InstancesNew/"

    for tsp_base in tsp_bases:
        rar_file = "%s-ttp.rar" % (tsp_base)
        url = base_url + rar_file
        r = requests.get(url)
        with open(rar_file, "wb") as f:
            f.write(r.content)

if __name__ == "__main__":

    parser = argparse.ArgumentParser()

    all_tsp_bases = ["eil51", "berlin52", "st70", "eil76", "pr76", "rat99", "kroA100", "kroB100", "kroC100", "kroD100", "kroE100", "rd100", "eil101", "lin105", "pr107", "pr124", "bier127", "ch130", "pr136", "pr144", "ch150", "kroA150", "kroB150", "pr152", "u159", "rat195", "d198", "kroA200", "kroB200", "ts225", "tsp225", "pr226", "gil262", "pr264", "a280", "pr299", "lin318", "rd400", "fl417", "pr439", "pcb442", "d493", "u574", "rat575", "p654", "d657", "u724", "rat783", "dsj1000", "pr1002", "u1060", "vm1084", "pcb1173", "d1291", "rl1304", "rl1323", "nrw1379", "fl1400", "u1432", "fl1577", "d1655", "vm1748", "u1817", "rl1889", "d2103", "u2152", "u2319", "pr2392", "pcb3038", "fl3795", "fnl4461", "rl5915", "rl5934", "pla7397", "rl11849", "usa13509", "brd14051", "d15112", "d18512", "pla33810", "pla85900", ]

    parser.add_argument('--tspbases', nargs='+')

    args = parser.parse_args()

    tsp_bases = all_tsp_bases
    if args.tspbases:
       tsp_bases = [tsp_base for tsp_base in args.tspbases if tsp_base in all_tsp_bases]

    download_instances(tsp_bases)
