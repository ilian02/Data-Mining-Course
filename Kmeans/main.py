import numpy as np
import pandas as pd
import random
import sys
import math
import matplotlib.pyplot as plt
from matplotlib import cm

def kmeans_init_centroids(data, k):
    indices = random.sample(range(data.shape[0]), k)
    return data[indices]

def find_nearest_centroid(point, centroids):
    closest_centroid = None
    closest_dist = 9999999999
    for key in centroids:
        dist = math.sqrt(pow(point[0] - centroids[key][0], 2) + pow(point[1] - centroids[key][1], 2))
        if dist < closest_dist:
            closest_dist = dist
            closest_centroid = key
        
    return closest_centroid
    

def check_if_points_have_changed_cluster(new_state, old_state):
    if new_state.keys() != old_state.keys():
        return False
    
    for key in new_state:
        if len(new_state[key]) != len(old_state[key]):
            return False

        val_new = new_state[key]
        val_old = old_state[key]
        for arr_new, arr_old in zip(val_new, val_old):
            if not np.array_equal(arr_new, arr_old):
                return False
    
    return True

def within_cluster_sum(clusters, centroids):
    clusters_inner_sum = 0
    for counter, cluster_id in enumerate(clusters):
        for point in clusters[cluster_id]:
            clusters_inner_sum += math.sqrt(pow(point[0] - centroids[cluster_id][0], 2) + pow(point[1] - centroids[cluster_id][1], 2))

    return clusters_inner_sum

def davies_bouldin_index(centroids, data, clusters):
    cluster_distances = np.zeros(len(centroids))

    sum = 0
    for cluster_id in clusters:
        for point in clusters[cluster_id]:
            sum += math.sqrt(pow(centroids[cluster_id][0] - point[0], 2) + pow(centroids[cluster_id][1] - point[1], 2))
        cluster_distances[cluster_id] = sum / len(clusters[cluster_id])

    db_index = 0
    for i in range(len(centroids)):
        max_ratio = 0
        for j in range(len(centroids)):
            if i != j:
                inter_cluster_dist = math.sqrt(pow(centroids[i][0] - centroids[j][0], 2) + pow(centroids[i][1] - centroids[j][1], 2))
                ratio = (cluster_distances[i] + cluster_distances[j]) / inter_cluster_dist
                max_ratio = max(max_ratio, ratio)
        db_index += max_ratio

    return db_index / len(centroids)

def simulate_kmeans(data, cluster_count, metric):

    best_centroids = None
    best_clusters = None
    best_correctess = 9999999999
    init = 0

    while init < 15:
        centroids_coords = kmeans_init_centroids(data, int(cluster_count))
        centroids = {}
        points_in_centroids = {}

        for counter, centroid in enumerate(centroids_coords):
            centroids[counter] = centroid
            points_in_centroids[counter] = []

        while True:
            new_points_in_centroids = {}
            for counter, centroid in enumerate(centroids):
                new_points_in_centroids[counter] = []

            for point in data:
                closest_centroid = find_nearest_centroid(point, centroids)
                new_points_in_centroids[closest_centroid].append(point)

            if check_if_points_have_changed_cluster(new_points_in_centroids, points_in_centroids):
                break
            else:
                points_in_centroids = new_points_in_centroids

            for counter, centroid in enumerate(centroids):
                points_array = np.array(new_points_in_centroids[counter])
                if len(points_array) != 0:
                    average_x = np.mean(points_array[:, 0])
                    average_y = np.mean(points_array[:, 1])
                    centroids[counter] = [average_x, average_y]
                else:
                    new_coords = kmeans_init_centroids(data, 1)
                    for centroid in new_coords:
                        centroids[counter] = centroid
                        new_points_in_centroids[counter] = []

        current_correctness = None
        if metric == "1":
            print("WCS Score: ")
            current_correctness = within_cluster_sum(points_in_centroids, centroids)
        else:
            print("David Bouldin Score: ")
            current_correctness = davies_bouldin_index(centroids, data, points_in_centroids)

        print(current_correctness)
        if best_correctess > current_correctness:
            best_centroids = centroids
            best_clusters = points_in_centroids
            best_correctess = current_correctness


        init = init + 1

    colors = cm.tab10(np.linspace(0, 1, int(cluster_count)))

    plt.figure(figsize=(8, 6))
    for cluster_id in range(int(cluster_count)):
        cluster_points = np.array(best_clusters[cluster_id])
        plt.scatter(cluster_points[:, 0], cluster_points[:, 1], color=colors[cluster_id], label=f"Cluster {cluster_id}")
        plt.scatter(best_centroids[cluster_id][0], best_centroids[cluster_id][1], s=200, color="black", marker='X')

    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.title('KMeans Clustering with Centroids')
    plt.legend()
    plt.show()

def initialize_centroids_kmeans_plus_plus(data, k):
    centroids = []
    centroids.append(data[random.randint(0, data.shape[0] - 1)])

    for _ in range(1, k):
        distances = np.min([np.linalg.norm(data - centroid, axis=1) for centroid in centroids], axis=0)
        probabilities = distances ** 2 / np.sum(distances ** 2)
        cumulative_probabilities = np.cumsum(probabilities)
        r = random.random()
        for i, p in enumerate(cumulative_probabilities):
            if r < p:
                centroids.append(data[i])
                break

    return np.array(centroids)

def simulate_kmeans_plus(data, cluster_count, metric):
    new_coords = (initialize_centroids_kmeans_plus_plus(data, cluster_count))

   
    centroids_coords = kmeans_init_centroids(data, int(cluster_count))
    centroids = {}
    points_in_centroids = {}

    for counter, centroid in enumerate(centroids_coords):
        centroids[counter] = centroid
        points_in_centroids[counter] = []

    while True:
        new_points_in_centroids = {}
        for counter, centroid in enumerate(centroids):
            new_points_in_centroids[counter] = []

        for point in data:
            closest_centroid = find_nearest_centroid(point, centroids)
            new_points_in_centroids[closest_centroid].append(point)

        if check_if_points_have_changed_cluster(new_points_in_centroids, points_in_centroids):
            break
        else:
            points_in_centroids = new_points_in_centroids

        for counter, centroid in enumerate(centroids):
            points_array = np.array(new_points_in_centroids[counter])
            if len(points_array) != 0:
                average_x = np.mean(points_array[:, 0])
                average_y = np.mean(points_array[:, 1])
                centroids[counter] = [average_x, average_y]
            else:
                new_coords = kmeans_init_centroids(data, 1)
                for centroid in new_coords:
                    centroids[counter] = centroid
                    new_points_in_centroids[counter] = []

    if metric == "1":
        print("WCS Score: ")
        print(within_cluster_sum(points_in_centroids, centroids))
    else:
        print("Davies Bouldin Index: ")
        print(davies_bouldin_index(centroids, data, points_in_centroids))

    colors = cm.tab10(np.linspace(0, 1, int(cluster_count)))

    plt.figure(figsize=(8, 6))
    for cluster_id in range(int(cluster_count)):
        cluster_points = np.array(points_in_centroids[cluster_id])
        plt.scatter(cluster_points[:, 0], cluster_points[:, 1], color=colors[cluster_id], label=f"Cluster {cluster_id}")
        plt.scatter(centroids[cluster_id][0], centroids[cluster_id][1], s=200, color="black", marker='X')

    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.title('KMeans Clustering with Centroids')
    plt.legend()
    plt.show()

def main(*argv):
    file_name, algo, metric, cluster_count = sys.argv[1:]

    delimiter =' '
    if file_name == "res\\normal\\normal.txt":
        delimiter = "\t"
    data = pd.read_csv(file_name, sep=delimiter, header=None).values
    
    if algo == "kmeans":
        simulate_kmeans(data, cluster_count, metric)
    else:
        simulate_kmeans_plus(data, int(cluster_count), metric)


main()