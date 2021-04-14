import collections
import serial
import threading
import time

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

MAX_DEPTH = 200

class SerialPlotter:
    def __init__(self):
        self.isThreadStarted = 0
        self.device = '/dev/ttyACM0'
        self.serialConnection = serial.Serial(self.device)
        self.allData = {
                        'MagX' : [0],
                        'MagY' : [0],
                        'MagZ' : [0],
                        'AccX' : [0],
                        'AccY' : [0],
                        'AccZ' : [0]
        }
        self.x = np.linspace(0, MAX_DEPTH, MAX_DEPTH)
        self.y1 = collections.deque([0]*MAX_DEPTH, maxlen=MAX_DEPTH)
        self.y2 = collections.deque([0]*MAX_DEPTH, maxlen=MAX_DEPTH)

        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(1,1,1)    
        self.line, = self.ax.plot(self.x, self.y1)
        self.line2, = self.ax.plot(self.x, self.y2)
        plt.ylim(-2**15, 2**15)

        self.thread = threading.Thread(target = self._in_background)
        self.ani = animation.FuncAnimation(self.fig, self._animate, interval=50)
        self.ani2 = animation.FuncAnimation(self.fig, self._animate2, interval=50)
        self.ctr = 1

    def _animate(self, i):
        self.line.set_ydata(self.y1)
        #plt.ylim(min(self.y1), max(self.y1))
    
    def _animate2(self, i):
        self.line2.set_ydata(self.y2)
        #plt.ylim(min(self.y2), max(self.y2))

    def _in_background(self):
        while True:
            if self.isThreadStarted:
                try:
                    nBytes = self.serialConnection.inWaiting()
                    if nBytes > 0:
                        line = self.serialConnection.read(nBytes)
                        prepared = line.decode('utf-8').strip(' ').split(' ')
                        for elem in prepared:
                            splitted = elem.split('=')
                            self.allData[splitted[0]].append(int(splitted[1]))
                        for key in self.allData:
                            self.allData[key] = self.allData[key][-MAX_DEPTH:]
                        np.append(self.x,time.time())
                        self.y1.append(self.allData['MagX'][-1])
                        self.y2.append(self.allData['MagY'][-1])
                        # self.y = self.y[-MAX_DEPTH:]
                        # self.x.append(self.ctr)
                        # self.x = self.x[-MAX_DEPTH:]
                        # self.ctr += 1
                        #print(allData['MagX'][-1])
                except Exception as e:
                    print(e)
            else:
                break
        print('Task stopped')

    def start(self):
        self.isThreadStarted = 1
        self.thread.start()
        plt.show()

    def stop(self):
        self.isThreadStarted = 0
        self.serialConnection.close()
        self.thread.join()


if __name__ == '__main__':
    sp = SerialPlotter()
    sp.start()
    sp.stop()
