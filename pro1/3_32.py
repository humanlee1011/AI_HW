# urs/bin/env python
# -*- coding: utf-8 -*-

import Math

class Graph():
    nodes = [];
    def __init__(self, nodes):
        nodes = nodes;

    def cal_Manhattan(A, B):
        x = (nodes[A] - nodes[B]) % 3;
        y = (nodes[A] - nodes[B]) / 3;
        return Math.abs(x) + Math.abs(y);

    
