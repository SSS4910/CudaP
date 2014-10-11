###############################################
#               John Stanley                  #	
#	      Apache log faker		      #
###############################################

# STILL NEEDS:
#		Random
#		Logs	
#		Error handling
#		Seed
#

import random
import string
import sys
import time 	  
 
#---------------Global variables-------------
rSeed = 1337
maxYear = 2014
minYear = 2013
random.seed(rSeed)
#----------------LOG CREATOR------------------
def ranDate():
	out = '['
	month = random.randint(1,12)
	max = 31
	day = -1
	if (month == 4 or month == 6 or month == 9 or month == 11):
		day = random.randint(1, 30)
	elif (month == 2):
		day = random.randint(1, 28)
	else :
		day = random.randint(1, 31)

	mon = 'ERROR'
	
	if( month == 1):
		mon = 'Jan'
	elif( month == 2):
		mon = 'Feb'
	elif( month == 3):
		mon = 'Mar'
	elif( month == 4):
		mon = 'Apr'
	elif( month == 5):
		mon = 'May'
	elif( month == 6):
		mon = 'Jun'
	elif( month == 7):
		mon = 'Jul'
	elif( month == 8):
		mon = 'Aug'
	elif( month == 9):
		mon = 'Sep'
	elif( month == 10):
		mon = 'Oct'
	elif( month == 11):
		mon = 'Nov'
	elif( month == 12):
		mon = 'Dec'
	else :
		mon = 'ERR'

	year = random.randint(minYear, maxYear)

	hour = random.randint(0, 23)

	minute = random.randint(0,59)
	
	second = random.randint(0,59)
	
	zone = random.randint(-12, 12)
	zone = zone * 100


	out = out + '%d/' %(day) + mon +'/%d:%d:%d:%d %d]'% (year, hour, minute, second, zone)
	

	return out

def getCommand():

	ranL = random.randint(1,15)
	ranS = ''.join(random.choice(string.ascii_lowercase) for _ in range(ranL))
	
	ranE = random.randint(1 ,10)
	extention = '.ERROR'
	if (ranE == 1):
		extention = '.jpg'
	elif (ranE == 2):
		extention = '.png'
	elif (ranE == 3): 
		extention = '.exe'
	elif (ranE == 4):
		extention = '.mp3'
	elif (ranE == 5):
		extention = '.mp4'
	elif (ranE == 6):
		extention = '.gif'
	elif (ranE == 7):
		extention = '.cs'
	elif (ranE == 8):
		extention = '.js'
	elif (ranE == 9):
		extention = '.java'
	else :
		extention = ''

		#.jpg .png .exe .mp3 .mp4 .gif .cs .js .java

	return '"GET /%s%s HTTP/1.0"' %(ranS, extention)		
	
def getStatus():
	status = random.randint(1,10)
	if (status == 1):
		return '404'
	else:
		return '200'
def getSize():
	return '%d' % random.randint(0, 10000000)
def getRef():
	return ''
def getUA():
	return ''
def createLine(imp):
		
	# [IP] - - [DATE] "command " 404or200 numbers "-" "internetbrowser?"
	out = ''

	
		# (%h)
	out = out + '%d' % random.randint(0, 254) + '.'
	out = out + '%d' % random.randint(0, 254) + '.'
	out = out + '%d' % random.randint(0, 254) + '.'
	out = out + '%d' % random.randint(0, 256) + ' '
		# (%l)
	out = out + '- '
		# (%u)
	out = out + '- '	
		# (%t)
	out = out + ranDate()
		# (\"%r\")
	out = out + getCommand() + ' '
		# (%>s)
	out = out + getStatus() + ' '
		# (%b)
	out = out + getSize() + ' '
		# (\"%{referer}i\")
	out = out + getRef() + ' '
		# (\"%{User-agent}i\")
	out = out + getUA() 

	out = out + '\n'
	return out 	








	 #return '69.58.%d.58 - - - - [29/Sep/2014:08:49:28 -0400] "GET /robots.txt HTTP/1.1" 404 445 "-" "Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:14.0; ips-agent) Gecko/20100101 Firefox/14.0.1" \n' % (a)

#------------------MAIN------------------------


if __name__ == "__main__":
	n = 0
	if len(sys.argv) != 3:
		print "\n I'm sorry please input: Filename, lines"

	else:
		
		#lets make a file 
		#print "file name ", sys.argv[1] , "\n" 

		file = open(sys.argv[1] , 'w')
		per = 0
		for x in range(0, int(sys.argv[2])):
			max = int(sys.argv[2])
			#print (x*100/max) 
			if (x*100/max > per):
				per = x*100/max
				#print '(%d) ' % per ,
				sys.stdout.write('\b\b\b')
				if (per > 9):
					sys.stdout.write('\b')
				sys.stdout.write('(%d)' % per)
				sys.stdout.flush()
			out = createLine(x)
			#print out
			file.write(out)
		print 'File Created'
		file.close	
