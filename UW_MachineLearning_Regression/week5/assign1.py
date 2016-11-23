#!/usr/bin/env python3

import pandas as pd
from math import log, sqrt

# Define the data types for the data to load
dtype_dict = {'bathrooms' : float, 'waterfront' : int, 'sqft_above' : int,
	'sqft_living15' : float, 'grade' : int, 'yr_renovated' : int, 'price' : float,
	'bedrooms' : float, 'zipcode' : str, 'long' : float, 'sqft_lot15' : float,
	'sqft_living' : float, 'floors' : float, 'condition' : int, 'lat' : float,
	'date' : str, 'sqft_basement' : int, 'yr_built' : int, 'id' : str,
	'sqft_lot' : int, 'view' : int}

# Load the sames data
sales = pd.read_csv('kc_house_data.csv', dtype=dtype_dict)

# Create new features from the sales data + preprocessing
sales['sqft_living_sqrt'] = sales['sqft_living'].apply(sqrt)
sales['sqft_lot_sqrt'] = sales['sqft_lot'].apply(sqrt)
sales['bedrooms_square'] = sales['bedrooms'] * sales['bedrooms']
sales['floors_square'] = sales['floors'] * sales['floors']

from sklearn import linear_model

# Define the features that go into our linear model
all_features = ['bedrooms', 'bedrooms_square', 'bathrooms', 'sqft_living',
	'sqft_living_sqrt', 'sqft_lot', 'sqft_lot_sqrt', 'sqft_lot', 'sqft_lot_sqrt',
	'floors', 'floors_square', 'waterfront', 'view', 'condition', 'grade',
	'sqft_above', 'sqft_basement', 'yr_built', 'yr_renovated']

# Perform Lasso regression
model_all = linear_model.Lasso(alpha=5e2, normalize=True)
model_all.fit(sales[all_features], sales['price'])

import numpy

# Q1 : Which features have been chosen by Lasso?
np_all_features = numpy.array(all_features)
model_all_coef = numpy.array(model_all.coef_)

print("### The following features were chosen by Lasso")
print(np_all_features[numpy.where(model_all_coef > 0)])

# Import test, training and validation data
testing = pd.read_csv('wk3_kc_house_test_data.csv', dtype=dtype_dict)
training = pd.read_csv('wk3_kc_house_train_data.csv', dtype=dtype_dict)
validation = pd.read_csv('wk3_kc_house_valid_data.csv', dtype=dtype_dict)

# Create the same 4 features for each of test, training and validation data
testing['sqft_living_sqrt'] = testing['sqft_living'].apply(sqrt)
testing['sqft_lot_sqrt'] = testing['sqft_lot'].apply(sqrt)
testing['bedrooms_square'] = testing['bedrooms'] * testing['bedrooms']
testing['floors_square'] = testing['floors'] * testing['floors']

training['sqft_living_sqrt'] = training['sqft_living'].apply(sqrt)
training['sqft_lot_sqrt'] = training['sqft_lot'].apply(sqrt)
training['bedrooms_square'] = training['bedrooms'] * training['bedrooms']
training['floors_square'] = training['floors'] * training['floors']

validation['sqft_living_sqrt'] = validation['sqft_living'].apply(sqrt)
validation['sqft_lot_sqrt'] = validation['sqft_lot'].apply(sqrt)
validation['bedrooms_square'] = validation['bedrooms'] * validation['bedrooms']
validation['floors_square'] = validation['floors'] * validation['floors']

# Choose best L1 penalty from a range of values
import sys
rss_min = sys.float_info.max
best_l1 = 10
for l1_penalty in numpy.logspace(1, 7, num=13):
	model = linear_model.Lasso(alpha=l1_penalty, normalize=True)
	model.fit(training[all_features], training['price'])
	rss = ((model.predict(validation[all_features]) - validation['price']) ** 2).sum()
	if (rss < rss_min):
		rss_min = rss
		best_l1 = l1_penalty

print("### Best l1 penalty at %f, RSS was %f" % (best_l1, rss_min))

# Compute RSS on Test data using this best l1_penanlty
model = linear_model.Lasso(alpha=best_l1, normalize=True, max_iter=1e9)
model.fit(testing[all_features], testing['price'])
num_nonzero_weights = numpy.count_nonzero(model.coef_) + numpy.count_nonzero(model.intercept_)

print("### Number of non-zero weights is ", num_nonzero_weights)

# Explore a large range of L1 penanlty
l1_nonzeros = []
for l1_penalty in numpy.logspace(1, 4, num=20):
	model = linear_model.Lasso(alpha=l1_penalty, normalize=True)
	model.fit(training[all_features], training['price'])

	# Record number of non-zero coefficients
	num_nonzero_weights = numpy.count_nonzero(model.coef_) + numpy.count_nonzero(model.intercept_)
	l1_nonzeros.append((l1_penalty, num_nonzero_weights))

max_nonzeros = 7
l1_penalty_min = sys.float_info.min
l1_penalty_max = sys.float_info.max

for l1, cnt in l1_nonzeros:
	if cnt > max_nonzeros and l1 > l1_penalty_min:
		l1_penalty_min = l1
	
	if cnt < max_nonzeros and l1 < l1_penalty_max:
		l1_penalty_max = l1

print("### l1_penalty_min = %f, l1_penalty_max = %f" % (l1_penalty_min, l1_penalty_max))

rss_min = sys.float_info.max
best_l1 = l1_penalty_min
# Explore the narrower range of l1 penalty
for l1_penalty in numpy.linspace(l1_penalty_min, l1_penalty_max, 20):
	model = linear_model.Lasso(alpha=l1_penalty, normalize=True)
	model.fit(training[all_features], training['price'])
	rss = ((model.predict(validation[all_features]) - validation['price']) ** 2).sum()
	num_nonzero_weights = numpy.count_nonzero(model.coef_) + numpy.count_nonzero(model.intercept_)

	if num_nonzero_weights == max_nonzeros:
		if (rss < rss_min):
			rss_min = rss
			best_l1 = l1_penalty

print("### l1 penalty %f is the lowest penalty with  %d non-zero coefficients, and rss %f" %(best_l1, max_nonzeros, rss_min))

print("### The following features were chosen by Lasso")
model = linear_model.Lasso(alpha=best_l1, normalize=True)
model.fit(training[all_features], training['price'])
model_coef = numpy.array(model.coef_)
print(np_all_features[numpy.where(model_coef > 0)])
