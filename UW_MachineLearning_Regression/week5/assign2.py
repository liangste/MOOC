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

print("ro[1] is 0 for l1_penalty from %f to %f" % (-ro[1] * 2, ro[1] * 2))
print("ro[2] is 0 for l1_penalty from %f to %f" % (-ro[2] * 2, ro[2] * 2))

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
