from collections import deque
from heapq import *

class TreeNode:
    def __init__(self, symbol, frequency, left, right):
        self.value = None
        self.symbol = symbol
        self.frequency = frequency
        self.left = left
        self.right = right

    @staticmethod
    def createLeaf(symbol, frequency):
        return TreeNode(symbol, frequency, None, None)

    @staticmethod
    def createInnerNode(left, right):
        left.value = "0"
        right.value = "1"
        return TreeNode(None, left.frequency + right.frequency, left, right)

    def isLeaf(self):
        return self.left is None and self.right is None

    def __lt__(self, other):
        return self.frequency < other.frequency

def frequencies(input):
    freqTable = {}
    for symbol in input:
        freqTable[symbol] = freqTable.get(symbol, 0) + 1
    return [(symbol, freq) for symbol, freq in freqTable.items()]

def makeTree(freqs):
    treeHeap = []
    for symbol, freq in freqs:
        heappush(treeHeap, TreeNode.createLeaf(symbol, freq))
    while len(treeHeap) > 1:
        heappush(treeHeap, TreeNode.createInnerNode(heappop(treeHeap), heappop(treeHeap)))
    return heappop(treeHeap)

def encode(freqs, input):
    if len(freqs) < 2:
        return None
    if len(input) == 0:
        return ""

    encodingTable = {}
    q = deque()
    q.appendleft((makeTree(freqs), ""))
    while len(q) > 0:
        node, prefix = q.pop()
        if node.isLeaf():
            encodingTable[node.symbol] = prefix
        else:
            q.appendleft((node.left, prefix + node.left.value))
            q.appendleft((node.right, prefix + node.right.value))

    output = ""
    for symbol in input:
        output += encodingTable[symbol]
    return output

def decode(freqs, input):
    if len(freqs) < 2:
        return None
    if len(input) == 0:
        return ""

    root = makeTree(freqs)
    output = ""
    node = root
    for value in input:
        node = node.left if value == node.left.value else node.right
        if node.isLeaf():
            output += node.symbol
            node = root
    return output
