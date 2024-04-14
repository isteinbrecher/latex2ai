#
# Copyright (c) 2018 Adobe Systems, Inc.
# All rights reserved.
#
# FILENAME:    pipl_gen.py
#
# FUNCTION:    pipl generator
#
# SYNTAX:      N/A
#
# PARAMETERS:  N/A
#
# EXAMPLE:     N/A
#
# AUTHOR :     amisriva
#
##################################################################################
import json
import os
import copy
import binascii
import platform


############################################
#Converts any number to a 4 byte hex string
############################################
def make_4_byte_hex_string(number_string):
	hex_number = hex(number_string).lstrip("0x")
	len_hex = len(hex_number) 
	if(len_hex > 4):
		raise Exception("value shoult not be more than 4 bytes")
	hex_number = "".join('0' for i in range(8 - len_hex)) + hex_number
	hexed_string = binascii.a2b_hex(hex_number)
	return hexed_string

##############################################
#Converts any string to a multiple of 4 bytes
##############################################
def make_string_multiple_of_4(string_val):
	len_string = len(string_val) 
	multiple_4_len = (len_string + 3) & ~3
	final_string_val = string_val + "".join('\0' for i in range(multiple_4_len - len_string))
	return final_string_val


#################################################
#Supports adding field to pipl property
#Supports dumping a pipl object to a bin or json
#################################################
class pipl:
	def __init__(self):
		abs_path = os.path.abspath(__file__)
		(dir_path, file_name) = os.path.split(abs_path)
		fileO = open(dir_path + '/template_plugin.json', 'r')
		self.data = json.load(fileO)
		fileO.close()

##############################################
#Add entry point function
##############################################
	def add_plugin_entry(self, value, id = 0):
		if platform.system() is 'Windows':
			self.data['win']['wx86']['id'] = id
			if type(value) is not type("string"):
				raise Exception('value type should be {0}'.format(type("string")))
			self.data['win']['wx86']['value'] = value + '\0\0\0\0'
		else:
			self.data['mac']['mi32']['id'] = id
			if type(value) is not type("string"):
				raise Exception('value type should be {0}'.format(type("string")))
			self.data['mac']['mi32']['value'] = value + '\0\0\0\0'

##############################################
#Add Plugin name
##############################################
	def add_plugin_name(self, value, id = 0):
		self.data['pinm']['id'] = id
		if type(value) is not type("string"):
			raise Exception('value type should be {0}'.format(type("string")))
		self.data['pinm']['value'] = value

##############################################
#Add Super top secret property
##############################################
	def add_plugin_stsp(self, value, id = 0):
		self.data['StsP']['id'] = id
		if type(value) is not type(0):
			raise Exception('value type should be {0}'.format(type(0)))
		self.data['StsP']['value'] = value



##############################################
#Dump Pipl object to json
##############################################
	def generate_pipl_json(self, pipl_path):
		f_out_json = open(pipl_path, 'w')
		output_data = clean_pipl_json(self)
		json.dump(output_data, f_out_json)
		f_out_json.close()

##############################################
#Dump Pipl object to bin
##############################################
	def generate_pipl_bin(self, pipl_path):
		generate_multi_pipl_bin([self], pipl_path)
		

################################################################
#Dump Pipl from dict object recursively
################################################################

def generate_pipl_bin_recursively_from_dict(pipl_dict, file_obj):
	for key in pipl_dict:
		if key == 'win':
			if platform.system() is 'Windows':
				generate_pipl_bin_recursively_from_dict(pipl_dict['win'], file_obj)
			continue
		elif key == 'mac':
			if platform.system() is not 'Windows':
				generate_pipl_bin_recursively_from_dict(pipl_dict['mac'], file_obj)
			continue
		else:
			#write vendor ID
			file_obj.write(pipl_dict[key]['vendor'])
			#write property key
			file_obj.write(key)
			#write property ID
			prop_id = 0
			if 'id' in pipl_dict[key]:
				prop_id = pipl_dict[key]['id']
			file_obj.write(make_4_byte_hex_string(prop_id))
			
			#determine property value
			out_prop_value = ""
			if type(pipl_dict[key]['value']) == type(0):
				out_prop_value = make_4_byte_hex_string(pipl_dict[key]['value'])
			else:
				out_prop_value = make_string_multiple_of_4(pipl_dict[key]['value'])


			#write property length
			file_obj.write(make_4_byte_hex_string(len(out_prop_value)))
			#write property value
			file_obj.write(out_prop_value)

		
##############################################
#Dump given Pipl object to file_obj as binary
##############################################		
def generate_pipl_bin(pipl_obj, file_obj):
	output_data = clean_pipl_json(pipl_obj)
	#write version
	file_obj.write(make_4_byte_hex_string(0))
	#write count
	file_obj.write(make_4_byte_hex_string(len(output_data)))
	generate_pipl_bin_recursively_from_dict(output_data, file_obj)
	
		

##############################################
#Dump given Pipl objects to file_obj as bin
##############################################
def generate_multi_pipl_bin(pipl_obj_list, pipl_path):
	f_out_bin = open(pipl_path, 'wb')
	#write pipl count
	f_out_bin.write(make_4_byte_hex_string(len(pipl_obj_list)))
	#write pipl objects in list
	for pipl_obj in pipl_obj_list:
		generate_pipl_bin(pipl_obj, f_out_bin)
	f_out_bin.close()


##################################################
#clean given Pipl object by removing unused props
##################################################	
def clean_pipl_json_recursively(pipl_dict):
	output_data = {}
	for key in pipl_dict:
		if key == 'win':
			if platform.system() is 'Windows':
				output_data['win'] = clean_pipl_json_recursively(pipl_dict['win'])
			continue
		elif key == 'mac':
			if platform.system() is not 'Windows':
				output_data['mac'] = clean_pipl_json_recursively(pipl_dict['mac'])
			continue
		elif 'value' in pipl_dict[key]:
			output_data[key] = pipl_dict[key]
	return output_data

##################################################
#clean given Pipl object by removing unused props
##################################################	
def clean_pipl_json(pipl_obj):
	return clean_pipl_json_recursively(pipl_obj.data)



##############################################
#Dump given Pipl objects to file_obj as json
##############################################
def generate_multi_pipl_json(pipl_obj_list, pipl_path):
	f_out_json = open(file_name, 'w')
	list_pipl = []
	for pipl_data in pipl_obj:
		output_data = clean_pipl_json(pipl_data)
		list_pipl += [output_data]
	json.dump(list_pipl, f_out_json)
	f_out_json.close()



			
	
