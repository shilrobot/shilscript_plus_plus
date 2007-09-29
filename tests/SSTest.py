import subprocess
import shutil
import os
import sys
import time

FAIL="FAIL"
OK="OK"

BASE_PATH = os.path.join(os.getcwd(), '..')
SSC_PATH = os.path.join(BASE_PATH, "Release/ssc.exe")
TMP_PATH = os.path.join(BASE_PATH, "tests/tmp")

class Failure:
	def __init__(self, test, result, stdout, stderr):
		self.test = test
		self.result = result
		self.stdout = stdout
		self.stderr = stderr
		
	def dump(self):
		print '-'*79
		expl = "Expected success, got failure"
		if self.result == OK:
			expl = "Expected failure, got success"
		print 'FAILED: %s: %s' % (self.test.name, expl)
		print 'Stdout:'
		for l in self.stdout.splitlines():
			print '  '+l.rstrip()
		print 'Stderr:'
		for l in self.stderr.splitlines():
			print '  '+l.rstrip()
		
		
g_tests = []
g_failures = []

# TODO: *properly* work with multiple packages once this is implemented
#       (currently it doesn't really care where the bytecode is outputted to, and
#		 totally blows it away anyhow.)
class Test:
	def __init__(self, name, *packages):
		self.name = name
		self.packages = packages
		
	def run(self):
		for p in self.packages:
			 if not p.run(self):
			 	return False
		return True
		
def AddTest(*args, **kwargs):
	t = Test(*args, **kwargs)
	g_tests.append(t)
	
def QuickTest(name, result, filecontents):
	AddTest(name,
			Package('TestPkg', result, 
					File('test.ss', filecontents)))
		
class Package:
	def __init__(self, name, result, *files):
		self.name = name
		self.result = result
		self.files = files
		
	def run(self, test):
		# Create the test dir
		shutil.rmtree(TMP_PATH, ignore_errors=True)
		os.mkdir(TMP_PATH)
		
		os.chdir(TMP_PATH)
		
		for file in self.files:		
			f = open(file.name, "wt")
			f.write(file.data)
			f.close()
		
		args = [SSC_PATH, "-p", self.name]
		for f in self.files:
			args.append(f.name)
		p = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		(stdout, stderr) = p.communicate()
		if p.returncode == 0:
			outcome = OK
		else:
			outcome = FAIL
		if outcome == self.result:
			sys.stdout.write(".")
		else:
			sys.stdout.write("F")
			g_failures.append(Failure(test, outcome, stdout, stderr))
			#print self.testname + " FAILED"
		os.chdir(BASE_PATH)
		shutil.rmtree(TMP_PATH)
		return outcome == self.result
		
class File:
	def __init__(self, name, data):
		self.name = name
		self.data = data

def runTests():
	print "Testing with %s" % os.path.normpath(SSC_PATH)
	print
	
	start = time.time()
	for t in g_tests:
		t.run()
	end = time.time()
	print
	
	for f in g_failures:
		f.dump()
		
	numTests = len(g_tests)
	numFails = len(g_failures)
	numSuccess = numTests-numFails
	print '-'*79
	print 'Ran %d tests in %.2f seconds (avg. %.3f s): %d OK (%.1f%%), %d failed (%.1f%%)' % (
			numTests,
			end-start,
			(end-start)/numTests,
			numSuccess,
			numSuccess / float(numTests) * 100.0,
			numFails,
			numFails / float(numTests) * 100.0)
			
__all__ = ['AddTest', 'QuickTest', 'Test', 'File', 'Package', 'Failure', 'runTests', 'OK', 'FAIL']
