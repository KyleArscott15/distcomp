with open('params1.dat', 'r') as input_file: 
	with open('params_test.dat', 'w') as output_file:
	    count = 0
	    for line in input_file:
	        if count == 3:
	            output_file.write('7 1 7\n')
	        elif count == 28:
	        	output_file.write('image_test.bmp')
	        else:
	            output_file.write(line)
	        count+= 1