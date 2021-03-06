#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import struct
from math import atan2, asin

from frame.data.euler import FCEuler

class FCQuat():
    def __init__(self, q0, q1, q2, q3):
        super(FCQuat, self).__init__()
        self.mQ0 = q0
        self.mQ1 = q1
        self.mQ2 = q2
        self.mQ3 = q3

    def ToEuler(self):
        theta = -atan2(self.mQ2 * self.mQ3 + self.mQ0 * self.mQ1, self.mQ0 * self.mQ0 + self.mQ3 * self.mQ3 - 0.5)
        #print(theta)
        phi   = +asin(2 * (self.mQ1 * self.mQ3 - self.mQ0 * self.mQ2))
        #print(phi)
        psi   =  atan2(self.mQ1 * self.mQ2 + self.mQ0 * self.mQ3, self.mQ0 * self.mQ0 + self.mQ1 * self.mQ1 - 0.5)
        #print(psi)
        euler =  FCEuler(theta, phi, psi)
        return euler

    def ToBytes(self):
        quatBytes = struct.pack('>ffff', self.mQ0, self.mQ1, self.mQ2, self.mQ3)
        return quatBytes

    def __str__(self):
        quatText = "(%+5.4f,%+5.4f,%+5.4f,%+5.4f)" % (self.mQ0, self.mQ1, self.mQ2, self.mQ3)
        return quatText


if __name__ == '__main__':
    quat = FCQuat(1, 0, 0, 0)
    quat.Print()

