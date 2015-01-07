from threading import Thread

i = 0


def thread1_func():
	global i # reference the global variable
	for j in xrange(1000000):
		i += 1

def thread2_func():
	global i # reference the global variable
	for j in xrange(1000000):
		i -= 1


def main():
	thread1 = Thread(target = thread1_func, args = (),)
	thread2 = Thread(target = thread2_func, args = (),)
	thread1.start()
	thread2.start()

	thread1.join()	
	thread2.join()
	print "I is now:", i


main()
