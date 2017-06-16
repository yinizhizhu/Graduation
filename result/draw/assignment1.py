# -*- coding: utf-8 -*-
"""
@author: Jiahe Li
"""

#import matplotlib.pyplot as plt
import matplotlib.pylab as plt
#import pylab as plt

class average:
    def __init__(self):
        self.XLabel = list()
        self.Label = list()

    def getData(self):
        """
            To get the average number from text
        """
        for i in xrange(1, 8):
            self.XLabel.append(i*80)
        print self.XLabel
        for i in xrange(1, 4):
            self.Label.append(list())
        self.readData("coarseOut.txt", 0)
        self.readData("fineOut.txt", 1)
        self.readData("palmOut.txt", 2)
    
    def readData(self, fileName, index):
        print fileName
        f = open(fileName, 'r')
        for i in xrange(1, 8):
            av = float(f.readline())
            print av
            self.Label[index].append(av)
        f.close()
        print self.Label[index]

    def show(self):
        """
            To fit the Sine() with multi-way
        """
        plt.figure('Graduation Project')
        plt.title('Execution Time')
        plt.xlabel('Number')
        plt.ylabel('Time(ms)')
        label = ["coarse", "fine", "palm"]
        plt.plot(self.XLabel, self.Label[0], 'g^', self.XLabel, self.Label[1], 'b^', self.XLabel, self.Label[2], 'r^')
        plt.legend(label, loc = 0)
        plt.axis([60, 600, 10, 1200])
        plt.show()
        
        plt.figure('Graduation Project')
        plt.title('Execution Time')
        plt.xlabel('Number')
        plt.ylabel('Time(ms)')
        plt.plot(self.XLabel, self.Label[0], 'g', self.XLabel, self.Label[1], 'b', self.XLabel, self.Label[2], 'r')
        plt.legend(label, loc = 0)
        plt.axis([60, 600, 10, 1200])
        plt.show()

test = average()
test.getData()
test.show()
