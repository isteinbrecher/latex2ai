#
# Copyright (c) 2018 Adobe Systems, Inc.
# All rights reserved.
#
# FILENAME:    create_pipl.py
#
# FUNCTION:    create pipl file for given plugin pipl info.
#
# SYNTAX:      python create_pipl.py -input '[{"name" : "plugin_name1", "entry_point" : "entry_point1"}, {"name" : "plugin_name2", "entry_point" : "entry_point2"}, ...] -output "pipl_name.pipl"
#
# PARAMETERS:  N/A
#
# EXAMPLE:     see Help for more detailed info
#
# AUTHOR :     amisriva
#
##################################################################################

import sys
import os
import json
import argparse

from pipl_gen import pipl
from pipl_gen import generate_multi_pipl_bin

def parse_argument():
	parser = argparse.ArgumentParser(description='Creates a PIPL file from given pipl info at given path')
	parser.add_argument('-input', help='Input json list of pipl dictionaries, with keys having name and entry_point', nargs = '?', default = [], dest = 'input_list')
	parser.add_argument('-output', help='file path of output pipl', dest = "output_file", nargs = 1)
	return parser.parse_args()

def main(args):
	args = parse_argument()
	pipl_obj_list = []
	if len(args.input_list):
		list_pipl = json.loads(args.input_list)
		for pipl_data in list_pipl:
			obj = pipl()
			if 'name' in pipl_data and len(pipl_data['name']):
				obj.add_plugin_name(str(pipl_data['name']))
			if 'entry_point' in pipl_data and len(pipl_data['entry_point']):
				obj.add_plugin_entry(str(pipl_data['entry_point']))
			if 'stsp' in pipl_data and pipl_data['stsp'] is True:
				obj.add_plugin_stsp(1)
            
			pipl_obj_list += [obj]
	else:
		obj = pipl()
		pipl_obj_list += [obj]
	generate_multi_pipl_bin(pipl_obj_list, "plugin.pipl")


if __name__ == "__main__":
	main(sys.argv[1:])



			
	