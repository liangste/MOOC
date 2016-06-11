#!/usr/bin/env python3

from subprocess import Popen, PIPE
import time
import unittest

g_maxRunTime = 1
g_testDirectory = "tests/"
g_testFile = "build/check_brackets.exe"

class TestBracketMethods(unittest.TestCase):

	def test(self):
		self.assertEqual('foo'.upper(), 'FOO')

		for testNum in range(1, 55):
			print("Test %d" % testNum)
			testFile = g_testDirectory + "/%02d" % testNum
			ansFile = g_testDirectory + "/%02d.a" % testNum

			fin = open(testFile, "r")
			inStr = fin.read()
			fin.close()

			fout = open(ansFile, "r")
			outStr = fout.read()
			fout.close()

			p = Popen([g_testFile], stdout=PIPE, stderr=PIPE, stdin=PIPE)

			try:
				startTime = time.time()
				outBytes, errs = p.communicate(input=str.encode(inStr), timeout=(g_maxRunTime * 2))
				passedTime = time.time() - startTime
				self.assertEqual(outStr, outBytes.decode('utf-8'))
				self.assertTrue(passedTime <= 1.0)
			except Exception:
				p.kill()

if __name__ == '__main__':
	unittest.main()
