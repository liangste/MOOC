#!/usr/bin/env python3

from subprocess import Popen, PIPE
import subprocess
import time
import unittest

g_maxRunTime = 1
g_testDirectory = "tests/"
g_testFile = "build/tree-height.exe"

class TestBracketMethods(unittest.TestCase):

	def test(self):
		for testNum in range(1, 25):
			print("Test %d" % testNum)
			testFile = g_testDirectory + "/%02d" % testNum
			ansFile = g_testDirectory + "/%02d.a" % testNum

			fin = open(testFile, "r")
			inStr = fin.read()

			fout = open(ansFile, "r")
			outStr = fout.read()

			p = Popen([g_testFile], stdout=PIPE, stderr=PIPE, stdin=PIPE)

			try:
				startTime = time.time()
				outBytes, errs = p.communicate(input=str.encode(inStr), timeout=(g_maxRunTime * 2))
				passedTime = time.time() - startTime
				self.assertEqual(outStr, outBytes.decode('utf-8'))
				self.assertLessEqual(passedTime, 1.0)
				print("Pass (took %fs)" % passedTime)
			except subprocess.TimeoutExpired:
				p.kill()
				fin.close()
				fout.close()

			p.wait()
			fin.close()
			fout.close()

if __name__ == '__main__':
	unittest.main()
