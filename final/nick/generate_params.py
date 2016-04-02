#!/usr/bin/python

def frange(start, stop, step):
	i = [start]
	count=0
	while i[count]+step < stop:
		i.append(i[count]+step)
		count=count+1
	return i

def sfrange(start, stop, step):
	i = [start]
	count=0
	while i[count]+step < stop:
		i.append(i[count]+step)
		count=count+1
	return count+1

print frange(1,10,2.5)

camera_pos=[frange(1,2,0.25), sfrange(1,2,0.25)*[1], frange(1,10,2.5)]
camera_lok=[sfrange(1,2,0.25)*[0], sfrange(1,2,0.25)*[0], sfrange(1,2,0.25)*[0]]
print camera_pos
print camera_lok

 
for i in range(len(camera_pos[0])):
	with open('params1.dat', 'r') as input_file:
		#print i
		val = i
	  	num_zeros = 1
		while val > 10:
			val=val%10
			num_zeros+=1
		num_zeros = 6-num_zeros
  		print num_zeros
		with open("params_test" + str(i) + ".dat", 'w') as output_file:
		    count = 0
		    for line in input_file:
			if count == 2:
			    output_file.write(str(camera_pos[0][i]) + " " + str(camera_pos[1][i]) + " " + str(camera_pos[2][i]) + "\n")
			elif count == 28:
				output_file.write('image_test.bmp')
			else:
			    output_file.write(line)
			count+= 1
