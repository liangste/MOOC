#!/usr/bin/env python3

import pandas as pd
from math import log, sqrt
import numpy as np

# Define the data types for the data to load
dtype_dict = {'bathrooms' : float, 'waterfront' : int, 'sqft_above' : int,
	'sqft_living15' : float, 'grade' : int, 'yr_renovated' : int, 'price' : float,
	'bedrooms' : float, 'zipcode' : str, 'long' : float, 'sqft_lot15' : float,
	'sqft_living' : float, 'floors' : float, 'condition' : int, 'lat' : float,
	'date' : str, 'sqft_basement' : int, 'yr_built' : int, 'id' : str,
	'sqft_lot' : int, 'view' : int}

# Load sales, test and train data
sales = pd.read_csv('kc_house_data.csv', dtype=dtype_dict)
tests = pd.read_csv('kc_house_test_data.csv', dtype=dtype_dict)
trains = pd.read_csv('kc_house_train_data.csv', dtype=dtype_dict)

# Fundtions from week 2
def get_numpy_data(data_frame, features, output):
	data_frame['constant'] = 1
	features = ['constant'] + features

	features_frame = data_frame[features]

	feature_matrix = features_frame.as_matrix()
	output_array = data_frame[output]

	output_array = output_array.as_matrix()
	return(feature_matrix, output_array)

def predict_output(feature_matrix, weights):
	# assume feature_matrix is a numpy matrix containing the features as columns and weights is a corresponding numpy array
	# create the predictions vector by using np.dot()
	return np.dot(feature_matrix, weights)

# Helper function to normalize features
def normalize_features(feature_matrix):
	norms = np.linalg.norm(feature_matrix, axis=0)
	normalized_features = feature_matrix / norms
	return (normalized_features, norms)

# Obtain normalized features from entire sales data
features = ['sqft_living', 'bedrooms']
feature_matrix, output_array = get_numpy_data(sales, features, 'price')
normalized_features, norms = normalize_features(feature_matrix)

# Set initial weights for coordinate descent
weights = [1, 4, 1]

# Test out a single step of coordinate descent
prediction = predict_output(normalized_features, weights)

ro = []
for i in range(len(weights)): # loop over each weight
    ro_i = np.dot(normalized_features[:, i], (output_array - prediction + weights[i] * normalized_features[:, i]))
    ro.append(ro_i)
print(ro)

print("ro[1] is 0 for l1_penalty smaller than %f and greater than %f" % (-ro[1] * 2, ro[1] * 2))
print("ro[2] is 0 for l1_penalty smaller than %f and greater than %f" % (-ro[2] * 2, ro[2] * 2))

# Implement coordinate descent
def lasso_coordinate_descent_step(i, feature_matrix, output, weights, l1_penalty):
	normalized_features, norms = normalize_features(feature_matrix)
	prediction = predict_output(normalized_features, weights)

	ro_i = np.dot(normalized_features[:, i],
		(output - prediction + weights[i] * normalized_features[:, i]))

	if i == 0:
		new_weight_i = ro_i
	elif ro_i < -l1_penalty / 2.:
		new_weight_i = ro_i + l1_penalty / 2.
	elif ro_i > l1_penalty / 2.:
		new_weight_i = ro_i - l1_penalty / 2.
	else:
		new_weight_i = 0.

	return new_weight_i

# Test implementation of coordinate descent step
expected_test_weight = 0.425558846691
import math
print("Next line should print %.12f" % expected_test_weight)
print(lasso_coordinate_descent_step(1, np.array([[3./math.sqrt(13),1./math.sqrt(10)],
	[2./math.sqrt(13),3./math.sqrt(10)]]), np.array([1., 1.]), np.array([1., 4.]), 0.1))

# Cyclic coordinate descent
def lasso_cyclical_coordinate_descent(feature_matrix, output, initial_weights,
	l1_penalty, tolerance):

	weights = initial_weights
	keep_alive = True

	while keep_alive:
		keep_alive = False
		for i in range(len(weights)):
			old_weight = weights[i]
			weights[i] = lasso_coordinate_descent_step(i, feature_matrix, output,
				weights, l1_penalty)

			if abs(old_weight - weights[i]) > tolerance:
				keep_alive = True

	return weights

# Evaluate coordinate descent on our simple model with 2 features
features = ['sqft_living', 'bedrooms']
feature_matrix, output_array = get_numpy_data(sales, features, 'price')
normalized_features, norms = normalize_features(feature_matrix)
initial_weights = np.zeros(len(features) + 1)
l1_penalty = 1e7
tolerance = 1.0

weights = lasso_cyclical_coordinate_descent(normalized_features, output_array,
	initial_weights, l1_penalty, tolerance)

print("### Found these non-zero features")
print(features)
print(weights)

prediction = predict_output(normalized_features, weights)
rss = ((prediction - output_array) ** 2).sum()

print("### RSS is %f" % rss)

# Lasso with more features
features = ['bedrooms', 'bathrooms', 'sqft_living', 'sqft_lot', 'floors',
	'waterfront', 'view', 'condition', 'grade', 'sqft_above', 'sqft_basement',
	'yr_built', 'yr_renovated']
features_with_intercept = ['intercept', 'bedrooms', 'bathrooms', 'sqft_living',
	'sqft_lot', 'floors', 'waterfront', 'view', 'condition', 'grade', 'sqft_above',
	'sqft_basement', 'yr_built', 'yr_renovated']

l1_penalty = 1e7
tolerance = 1.0

print("Test with l1_penalty = %f and tolerance %f" %(l1_penalty, tolerance))
feature_matrix, output = get_numpy_data(trains, features, 'price')
normalized_features, norms = normalize_features(feature_matrix)
initial_weights = np.zeros(len(features) + 1)
weights1e7 = lasso_cyclical_coordinate_descent(normalized_features, output,
	initial_weights, l1_penalty, tolerance)
normalized_weights1e7 = weights1e7 / norms

print("### Found these non-zero features")
for i in range(len(features_with_intercept)):
	if weights1e7[i] != 0.0:
		print(features_with_intercept[i])

l1_penalty = 1e8
tolerance = 1.0

print("Test with l1_penalty = %f and tolerance %f" %(l1_penalty, tolerance))
feature_matrix, output = get_numpy_data(trains, features, 'price')
normalized_features, norms = normalize_features(feature_matrix)
initial_weights = np.zeros(len(features) + 1)
weights1e8 = lasso_cyclical_coordinate_descent(normalized_features, output,
	initial_weights, l1_penalty, tolerance)
normalized_weights1e8 = weights1e8 / norms

print("### Found these non-zero features")
for i in range(len(features_with_intercept)):
	if weights1e8[i] != 0.0:
		print(features_with_intercept[i])

l1_penalty = 1e4
tolerance = 5e5

print("Test with l1_penalty = %f and tolerance %f" %(l1_penalty, tolerance))
feature_matrix, output = get_numpy_data(trains, features, 'price')
normalized_features, norms = normalize_features(feature_matrix)
initial_weights = np.zeros(len(features) + 1)
weights1e4 = lasso_cyclical_coordinate_descent(normalized_features, output,
	initial_weights, l1_penalty, tolerance)
normalized_weights1e4 = weights1e4 / norms

print("### Found these non-zero features")
for i in range(len(features_with_intercept)):
	if weights1e4[i] != 0.0:
		print(features_with_intercept[i])

print(normalized_weights1e7[3])

# Evaluate learned models on test data
(test_feature_matrix, test_output) = get_numpy_data(tests, features, 'price')

print("model weights1e7")
prediction = predict_output(test_feature_matrix, normalized_weights1e7)
rss = 0
for i in range(0, len(prediction)):
    error = prediction[i] - tests['price'][i]
    rss += error * error
print(rss)

print("model weights1e4")
prediction = predict_output(test_feature_matrix, normalized_weights1e4)
rss = 0
for i in range(0, len(prediction)):
    error = prediction[i] - tests['price'][i]
    rss += error * error
print(rss)

print("model weights1e8")
prediction = predict_output(test_feature_matrix, normalized_weights1e8)
rss = 0
for i in range(0, len(prediction)):
    error = prediction[i] - tests['price'][i]
    rss += error * error
print(rss)
