# python3
from sys import stdin
import numpy as np
from scipy.optimize import linprog

def solve_diet_problem(n, m, A, b, c):
  A = np.array(A)
  b = np.array(b)
  c = np.array(c)

  b_bounds = ((0, None),) * m
  res = linprog(-c, A_ub = A, b_ub = b, bounds = b_bounds, options = { 'tol': 1e-4 })

  if res.status == 3:
	  result_t = 1
  elif res.status == 0:
	  result_t = 0
  else:
	  result_t = -1

  return [result_t, res.x]

n, m = list(map(int, stdin.readline().split()))
A = []
for i in range(n):
	A += [list(map(int, stdin.readline().split()))]
b = list(map(int, stdin.readline().split()))
c = list(map(int, stdin.readline().split()))

anst, ansx = solve_diet_problem(n, m, A, b, c)

if anst == -1:
	print("No solution")
if anst == 0:
	print("Bounded solution")
	print(' '.join(list(map(lambda x : '%.18f' % x, ansx))))
if anst == 1:
	print("Infinity")
