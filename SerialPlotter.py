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
                        'MagX' : collections.deque([0]*MAX_DEPTH, maxlen=MAX_DEPTH),
                        'MagY' : collections.deque([0]*MAX_DEPTH, maxlen=MAX_DEPTH),
                        'MagZ' : collections.deque([0]*MAX_DEPTH, maxlen=MAX_DEPTH),
                        'AccX' : collections.deque([0]*MAX_DEPTH, maxlen=MAX_DEPTH),
                        'AccY' : collections.deque([0]*MAX_DEPTH, maxlen=MAX_DEPTH),
                        'AccZ' : collections.deque([0]*MAX_DEPTH, maxlen=MAX_DEPTH),
        }
        self.x = np.linspace(0, MAX_DEPTH, MAX_DEPTH)

        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(1,1,1)    
        self.line, = self.ax.plot(self.x, self.allData['MagX'], label='MagX')
        self.line2, = self.ax.plot(self.x, self.allData['MagY'], label='MagY')
        self.line3, = self.ax.plot(self.x, self.allData['MagZ'], label='MagZ')
        plt.ylim(-1000, 1000)
        plt.legend()
        #plt.ylabel('мкТл')
        plt.tight_layout()
        plt.grid()

        self.thread = threading.Thread(target = self._in_background)
        self.ani = animation.FuncAnimation(self.fig, self._animate, interval=50)
        self.ani2 = animation.FuncAnimation(self.fig, self._animate2, interval=50)
        self.ani3 = animation.FuncAnimation(self.fig, self._animate3, interval=50)

    def _animate(self, i):
        self.line.set_ydata(self.allData['MagX'])
    
    def _animate2(self, i):
        self.line2.set_ydata(self.allData['MagY'])

    def _animate3(self, i):
        self.line3.set_ydata(self.allData['MagZ'])

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
                            if splitted[0] == 'MagX' or splitted[0] == 'MagY' or splitted[0] == 'MagZ':
                                self.allData[splitted[0]].append(int(splitted[1]))
                            else:
                                self.allData[splitted[0]].append(int(splitted[1]))
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
