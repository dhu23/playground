# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import matplotlib.pyplot as plt
import mglearn
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn.neighbors import KNeighborsRegressor
from sklearn.datasets import load_breast_cancer

def run_forge_example():
    X, y = mglearn.datasets.make_forge()
    
    fig, axes = plt.subplots(1, 3, figsize=(10, 3))
    
    for n, ax in zip([1, 3, 9], axes):
        clf = KNeighborsClassifier(n_neighbors=n).fit(X, y)
        mglearn.plots.plot_2d_separator(clf, X, fill=True, 
                                        eps=0.5, ax=ax, alpha=0.4)
        mglearn.discrete_scatter(X[:, 0], X[:, 1], y, ax=ax)
        ax.set_title("{} neighbor(s)".format(n))
        ax.set_xlabel("feature 0")
        ax.set_ylabel("feature 1")
        
    axes[0].legend(loc=3)

#import matplotlib
#print(matplotlib.get_backend())

def run_breast_cancer_example():
    cancer = load_breast_cancer()
    X_train, X_test, y_train, y_test = train_test_split(
            cancer.data, cancer.target, stratify=cancer.target, 
            random_state=66)
    training_accuracy = []
    
    test_accuracy = []
    
    neighbors_settings = range(1, 11)
    
    for n in neighbors_settings:
        clf = KNeighborsClassifier(n_neighbors=n)
        clf.fit(X_train, y_train)
        training_accuracy.append(clf.score(X_train, y_train))
        test_accuracy.append(clf.score(X_test, y_test))
        
    plt.plot(neighbors_settings, training_accuracy, label='training accuracy')
    plt.plot(neighbors_settings, test_accuracy, label='test accuracy')
    plt.ylabel('Accuracy')
    plt.xlabel('n_neighbors')
    plt.legend()

def run_wave_example(): 
    # for k-neighbors regression
    X, y = mglearn.datasets.make_wave(n_samples=40)
    X_train, X_test, y_train, y_test = train_test_split(
            X, y, random_state=0)
    reg = KNeighborsRegressor(n_neighbors=3)
    reg.fit(X_train, y_train)
    print(reg.predict(X_test))
    print(reg.score(X_test, y_test))


if __name__ == "__main__":
    #run_forge_example()
    #run_breast_cancer_example()
    run_wave_example()