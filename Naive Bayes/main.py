import pandas as pd
import numpy as np

pd.set_option('future.no_silent_downcasting', True)

url = "https://archive.ics.uci.edu/ml/machine-learning-databases/voting-records/house-votes-84.data"
columns = [
    "party", "handicapped_infants", "water_project_cost_sharing", "adoption_of_the_budget_resolution",
    "physician_fee_freeze", "el_salvador_aid", "religious_groups_in_schools", "anti_satellite_test_ban",
    "aid_to_nicaraguan_contras", "mx_missile", "immigration", "synfuels_corporation_cutback",
    "education_spending", "superfund_right_to_sue", "crime", "duty_free_exports", "export_administration_act_south_africa"
]


# Step 3: Split data into training and testing sets with stratification
def stratified_train_test_split(X, y, test_size=0.2, random_state=None):
    np.random.seed(random_state)
    unique_classes, class_counts = np.unique(y, return_counts=True)
    test_indices = []
    train_indices = []

    for cls, count in zip(unique_classes, class_counts):
        cls_indices = np.where(y == cls)[0]
        np.random.shuffle(cls_indices)
        test_count = int(count * test_size)

        test_indices.extend(cls_indices[:test_count])
        train_indices.extend(cls_indices[test_count:])

    np.random.shuffle(train_indices)
    np.random.shuffle(test_indices)

    return X[train_indices], X[test_indices], y[train_indices], y[test_indices]


def accuracy_score_manual(y_true, y_pred):
    return np.mean(y_true == y_pred)

def classification_report_manual(y_true, y_pred):
    classes = np.unique(y_true)
    report = ""
    for cls in classes:
        true_positives = np.sum((y_true == cls) & (y_pred == cls))
        false_positives = np.sum((y_true != cls) & (y_pred == cls))
        precision = true_positives / (true_positives + false_positives) if (true_positives + false_positives) > 0 else 0
        report += f"Class {cls}: Precision={precision:.2f}\n "
    return report


def cross_validation(X, y, classifier, n_folds=10):
    fold_size = len(X) // n_folds
    indices = np.arange(len(X))
    np.random.shuffle(indices)
    
    accuracies = []
    for i in range(n_folds):
        test_indices = indices[i*fold_size:(i+1)*fold_size]
        train_indices = np.concatenate([indices[:i*fold_size], indices[(i+1)*fold_size:]])
        
        X_train, X_test = X[train_indices], X[test_indices]
        y_train, y_test = y[train_indices], y[test_indices]
        
        classifier.fit(X_train, y_train)
        y_pred = classifier.predict(X_test)
        accuracies.append(accuracy_score_manual(y_test, y_pred))
        print(f"Accuracy Fold {i + 1}: {round(accuracies[len(accuracies) - 1], 2)}%")
    
    return np.mean(accuracies), np.std(accuracies)


class NaiveBayes:
    def __init__(self):
        self.class_stats = {}
        self.classes = []

    def fit(self, X, y):
        self.classes = np.unique(y)
        for cls in self.classes:
            X_cls = X[y == cls]
            mean = X_cls.mean(axis=0)
            var = X_cls.var(axis=0)
            prior = X_cls.shape[0] / float(X.shape[0])
            self.class_stats[cls] = {"mean": mean, "var": var, "prior": prior}

    def predict(self, X):
        predictions = []
        for x in X:
            class_probs = {}
            for cls, stats in self.class_stats.items():
                mean = stats["mean"]
                var = stats["var"]
                prior = stats["prior"]
                likelihood = -0.5 * np.sum(np.log(2 * np.pi * var)) - 0.5 * np.sum(((x - mean) ** 2) / var)
                class_probs[cls] = np.log(prior) + likelihood
            predictions.append(max(class_probs, key=class_probs.get))
        return np.array(predictions)
    

def main():

    mode = input("Enter mode of classificator\n> ")

    data = pd.read_csv(url, header=None, names=columns)
    data = data.replace({'y': 1, 'n': 0, '?': 2})
    data = data.infer_objects()

    # Replace 'y', 'n', and '?' with numerical values ('y' -> 1, 'n' -> 0, '?' -> 2)
    # Encode party labels (democrat -> 0, republican -> 1)
    data['party'] = data['party'].map({'democrat': 0, 'republican': 1})

    # Split features and target
    X = data.iloc[:, 1:].values
    y = data['party'].values

    X_train, X_test, y_train, y_test = stratified_train_test_split(X, y, test_size=0.2, random_state=42)
    nb = NaiveBayes()
    nb.fit(X_train, y_train)

    # Train the classifier on the training set
    y_pred = nb.predict(X_train)

    train_accuracy = accuracy_score_manual(y_train, y_pred)
    train_report = classification_report_manual(y_train, y_pred)

    print("Accuracy over training set: ", round(train_accuracy, 2))
    print("Classification Report:\n", train_report)

    # Test the classifier on the test set
    y_pred = nb.predict(X_test)

    test_accuracy = accuracy_score_manual(y_test, y_pred)
    test_report = classification_report_manual(y_test, y_pred)

    print("Accuracy over test set:", round(test_accuracy, 2))
    print("Classification Report:\n", test_report)

    cv_mean, cv_std = cross_validation(X_train, y_train, nb)
    print(f"Average accuracy: {round(cv_mean, 2)}%")
    print(f"Standard deviation of accuracy: {round(cv_std, 2)}%")
    



main()