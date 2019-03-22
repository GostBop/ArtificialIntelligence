#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
network.py
~~~~~~~~~~
A module to implement the stochastic gradient descent learning
algorithm for a feedforward neural network.  Gradients are calculated
using backpropagation.  Note that I have focused on making the code
simple, easily readable, and easily modifiable.  It is not optimized,
and omits many desirable features.
"""

#### Libraries
# Standard library
import random

# Third-party libraries
import numpy as np

class Network(object):

    def __init__(self, sizes):
        """
        :param sizes: list���ͣ�����ÿ�����������Ԫ��Ŀ
                      Ʃ��˵��sizes = [2, 3, 2] ��ʾ�������������Ԫ��
                      ���ز���3����Ԫ�Լ��������2����Ԫ
        """
        # �м���������
        self.num_layers = len(sizes)
        self.sizes = sizes
        # ��ȥ����㣬�������ÿ���� y ����Ԫ�� biase ֵ��0 - 1��
        self.biases = [np.random.randn(y, 1) for y in sizes[1:]]
        # �������ÿ�������ߵ� weight ֵ��0 - 1��
        self.weights = [np.random.randn(y, x)
                        for x, y in zip(sizes[:-1], sizes[1:])]

    def feedforward(self, a):
        """
        ǰ�������ÿ����Ԫ��ֵ
        :param a: ����ֵ
        :return: �����ÿ����Ԫ��ֵ
        """
        for b, w in zip(self.biases, self.weights):
            # ��Ȩ����Լ����� biase
            a = sigmoid(np.dot(w, a) + b)
        return a

    def SGD(self, training_data, epochs, mini_batch_size, eta,
            test_data=None):
        """
        ����ݶ��½�
        :param training_data: �����ѵ����
        :param epochs: ��������
        :param mini_batch_size: С��������
        :param eta: ѧϰ��
        :param test_data: �������ݼ�
        """
        if test_data: n_test = len(test_data)
        n = len(training_data)
        for j in xrange(epochs):
            # ����ѵ��������������˳�����仯
            random.shuffle(training_data)
            # ����С������������ѵ����
            mini_batches = [
                training_data[k:k+mini_batch_size]
                for k in xrange(0, n, mini_batch_size)]
            for mini_batch in mini_batches:
                # ����ÿ��С���������� w �� b����������һ��
                self.update_mini_batch(mini_batch, eta)
            # �������ÿ�ֽ������������׼ȷ��
            if test_data:
                print "{0} ��ȷ��: {1} / {2}".format(
                    j, self.evaluate(test_data), n_test)
            else:
                print "Epoch {0} complete".format(j)

    def update_mini_batch(self, mini_batch, eta):
        """
        ���� w �� b ��ֵ
        :param mini_batch: һ���ֵ�����
        :param eta: ѧϰ��
        """
        # ���� biases �� weights ��������������Ӧ��ȫ��Ԫ��ֵΪ 0 �Ŀվ���
        nabla_b = [np.zeros(b.shape) for b in self.biases]
        nabla_w = [np.zeros(w.shape) for w in self.weights]
        for x, y in mini_batch:
            # ���������е�ÿһ������ x ������� y������ w �� b ��ƫ����
            delta_nabla_b, delta_nabla_w = self.backprop(x, y)
            # �ۼӴ���ƫ��ֵ delta_nabla_b �� delta_nabla_w
            nabla_b = [nb+dnb for nb, dnb in zip(nabla_b, delta_nabla_b)]
            nabla_w = [nw+dnw for nw, dnw in zip(nabla_w, delta_nabla_w)]
        # ���¸����ۼӵ�ƫ��ֵ���� w �� b��������Ϊ����С������
        # ���� eta Ҫ����С�����ĳ���
        self.weights = [w-(eta/len(mini_batch))*nw
                        for w, nw in zip(self.weights, nabla_w)]
        self.biases = [b-(eta/len(mini_batch))*nb
                       for b, nb in zip(self.biases, nabla_b)]

    def backprop(self, x, y):
        """
        :param x:
        :param y:
        :return:
        """
        nabla_b = [np.zeros(b.shape) for b in self.biases]
        nabla_w = [np.zeros(w.shape) for w in self.weights]
        # ǰ����
        activation = x
        # ����ÿ�����Ԫ��ֵ�ľ�������ѭ���� append ÿ�����Ԫ��ֵ
        activations = [x]
        # ����ÿ��δ���� sigmoid �������Ԫ��ֵ
        zs = []
        for b, w in zip(self.biases, self.weights):
            z = np.dot(w, activation)+b
            zs.append(z)
            activation = sigmoid(z)
            activations.append(activation)
        # �� �� ��ֵ(���һ��).el=(ol-y)��f'(al)
        delta = self.cost_derivative(activations[-1], y) * \
            sigmoid_prime(zs[-1]) 
        nabla_b[-1] = delta
        # ����ǰһ������ֵ
        nabla_w[-1] = np.dot(delta, activations[-2].transpose())
        for l in xrange(2, self.num_layers):
            # �ӵ����� **l** �㿪ʼ���£�**-l** �� python �����е��﷨��ʾ�ӵ����� l �㿪ʼ����
            # ������������ **l+1** ��� �� ֵ������ **l** �� �� ֵ
            # wl=wl-n*el*ol-1, el = (Wl+1 ^ T * el+1) ` f'(al)
            z = zs[-l]
            sp = sigmoid_prime(z) #f'(al)
            delta = np.dot(self.weights[-l+1].transpose(), delta) * sp # el = (Wl+1 ^ T * el+1) ` f'(al)
            nabla_b[-l] = delta
            nabla_w[-l] = np.dot(delta, activations[-l-1].transpose()) #el*ol-1
        return (nabla_b, nabla_w)

    def evaluate(self, test_data):
        # ���Ԥ����
        test_results = [(np.argmax(self.feedforward(x)), y) #argmax�õ���ֵ
                        for (x, y) in test_data]
        # ������ȷʶ��ĸ���
        return sum(int(x == y) for (x, y) in test_results)

    def cost_derivative(self, output_activations, y):
        """
        ������ʧ����
        :param output_activations:
        :param y:
        :return:
        """
        return (output_activations-y)

#### Miscellaneous functions
def sigmoid(z):
    """
    �� sigmoid ������ֵ
    :param z:
    :return:
    """
    return 1.0/(1.0+np.exp(-z))

def sigmoid_prime(z):
    """
    �� sigmoid �����ĵ���
    :param z:
    :return:
    """
    return sigmoid(z)*(1-sigmoid(z))