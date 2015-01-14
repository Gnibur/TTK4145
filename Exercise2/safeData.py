from threading import Thread
from threading import Lock

i = 0
lock = Lock()

def thread1_func():
	global i # reference the global variable
	global lock
	lock.acquire()
	try:
		for j in xrange(1000000):
			i += 1
	finally:
		lock.release()

def thread2_func():
	global i # reference the global variable
	global lock
	lock.acquire()
	try:
		for j in xrange(1000001):
			i -= 1
	finally:		
		lock.release()

def main():
	thread1 = Thread(target = thread1_func, args = (),)
	thread2 = Thread(target = thread2_func, args = (),)
	thread1.start()
	thread2.start()

	thread1.join()	
	thread2.join()
	print "I is now:", i


main()
