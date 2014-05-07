#!/usr/bin/env python
#
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of REDHAWK.
#
# REDHAWK is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# REDHAWK is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
# for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
#

import time
from ossie.utils import sb
import numpy as np
import matplotlib.pyplot as plt

#Use sb helpers for producing and receiving data from component
src = sb.DataSource()
sink = sb.DataSink()
divider = sb.launch('../FrequencyDivider.spd.xml')

#Make connections
src.connect(divider)
divider.connect(sink)
  
#Start sandbox env
sb.start()
        
#generate message signal
numSamples = 1000
data = []
for i in xrange(numSamples):
    data.append((float(0.3)*float(np.cos(2*np.pi*float(0.013)*i + float(0.0)))) +
               (float(0.2)*float(np.cos(2*np.pi*float(0.021)*i + float(0.4)))) +
               (float(0.4)*float(np.cos(2*np.pi*float(0.037)*i + float(1.7)))))

#Count zero crossings in message data
signCurrent = False
if data[0] > 0:
    signLast = True
else:
    signLast = False
d_zeroCross = 0 
for i in xrange(1,len(data)):
    if data[i] > 0:
        signCurrent = True
    else:
        signCurrent = False
    if signCurrent != signLast:
        d_zeroCross += 1
    signLast = signCurrent

#-----------------------------------------------
# Unit Test 1
# Testing Default Property Settings
#-----------------------------------------------

#Send data across the wave
src.push(data)
time.sleep(1)
received_data = sink.getData()

#Count zero-crossings in received data
signCurrent = False
if received_data[0] > 0:
    signLast = True
else:
    signLast = False
r_zeroCross = 0 
for i in xrange(1,len(received_data)):
    if received_data[i] > 0:
        signCurrent = True
    else:
        signCurrent = False
    if signCurrent != signLast:
        r_zeroCross += 1
    signLast = signCurrent

#Plots 
#plt.plot(data)
#plt.plot(received_data)
#plt.plot([0,len(data)],[0,0],'k-',lw=1)
#plt.show() 
      
#Check expected values against the received values
expected = d_zeroCross
passed = True
if r_zeroCross != expected:
    passed = False 
                
if passed:
    print "Unit Test 1 .........................",u'\u2714'
else:
    print "Unit Test 1 .........................",u'\u2718'


#-----------------------------------------------
# Unit Test 2
# Testing Setting the Divisor Property
#-----------------------------------------------

#Configure prop
divider.configure({'Divisor':16})

#Send data across the wave
src.push(data)
time.sleep(1)
received_data = sink.getData()

#Count zero-crossings in received data
signCurrent = False
if received_data[0] > 0:
    signLast = True
else:
    signLast = False
r_zeroCross = 0 
for i in xrange(1,len(received_data)):
    if received_data[i] > 0:
        signCurrent = True
    else:
        signCurrent = False
    if signCurrent != signLast:
        r_zeroCross += 1
    signLast = signCurrent

#Plots 
#plt.plot(data)
#plt.plot(received_data)
#plt.plot([0,len(data)],[0,0],'k-',lw=1)
#plt.show() 
      
#Check expected values against the received values
expected = d_zeroCross / divider.Divisor
passed = True
if r_zeroCross != expected:
    passed = False 
                
if passed:
    print "Unit Test 2 .........................",u'\u2714'
else:
    print "Unit Test 2 .........................",u'\u2718'
    
  
#-----------------------------------------------
# Unit Test 3
# Testing Zero Divisor
#-----------------------------------------------

#Configure prop
divider.configure({'Divisor':0})

#Send data across the wave
src.push(data)
time.sleep(1)
received_data = sink.getData()

#Count zero-crossings in received data
signCurrent = False
if received_data[0] > 0:
    signLast = True
else:
    signLast = False
r_zeroCross = 0 
for i in xrange(1,len(received_data)):
    if received_data[i] > 0:
        signCurrent = True
    else:
        signCurrent = False
    if signCurrent != signLast:
        r_zeroCross += 1
    signLast = signCurrent

#Plots 
#plt.plot(data)
#plt.plot(received_data)
#plt.plot([0,len(data)],[0,0],'k-',lw=1)
#plt.show() 
      
#Check expected values against the received values
expected = d_zeroCross
passed = True
if r_zeroCross != expected:
    passed = False 
                
if passed:
    print "Unit Test 3 .........................",u'\u2714'
else:
    print "Unit Test 3 .........................",u'\u2718'    
      
#Stop sandbox env
sb.stop()
