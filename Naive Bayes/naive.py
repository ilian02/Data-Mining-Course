from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.naive_bayes import CategoricalNB
from sklearn.preprocessing import LabelEncoder
from sklearn.metrics import accuracy_score
from ucimlrepo import fetch_ucirepo
import numpy as np


# fetch dataset 
congressional_voting_records = fetch_ucirepo(id=105) 
  
# data (as pandas dataframes) 
X = congressional_voting_records.data.features 
y = congressional_voting_records.data.targets

# Encode categorical features and target
X_encoded = X.apply(LabelEncoder().fit_transform)  # Convert 'y', 'n', '?' to integers
y_encoded = LabelEncoder().fit_transform(y)        # Convert 'democrat', 'republican' to 0, 1

# Split the data (80% train, 20% test, stratified)
X_train, X_test, y_train, y_test = train_test_split(X_encoded, y_encoded, test_size=0.2, stratify=y_encoded, random_state=42)


class NaiveBayes:

    def __init__(self, alpha=1.0):
        self.alpha = alpha
        self.priors = {}
        self.likelihoods = {}
        self.classes = []

    def fit(self, X, y):
        self.classes = np.unique(y)

        self.priors = {c: np.sum(y == c) / len(y) for c in self.classes}


nb = NaiveBayes(alpha=1.0)
nb.fit(X_encoded, y_encoded)

print(nb.classes)
print(nb.priors)