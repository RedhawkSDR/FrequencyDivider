/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK.
 *
 * REDHAWK is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

/**************************************************************************

    This is the component code. This file contains the child class where
    custom functionality can be added to the component. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

 	Source: FrequencyDivider.spd.xml
 	Generated on: Wed Aug 21 10:55:11 EDT 2013
 	REDHAWK IDE
 	Version: 1.8.4
 	Build id: R201305151907

**************************************************************************/

#include "FrequencyDivider.h"

PREPARE_LOGGING(FrequencyDivider_i)

FrequencyDivider_i::FrequencyDivider_i(const char *uuid, const char *label) :
    FrequencyDivider_base(uuid, label)
{
	m_lastSize = 0;
	m_currentSize = 0;
	m_divisor = 0;
	m_DIVISOR = Divisor;
	m_signLast = m_signCurrent = false;
	m_out = 1;
	setPropertyChangeListener("Divisor", this,
				&FrequencyDivider_i::propertyChangeListener);
}

FrequencyDivider_i::~FrequencyDivider_i()
{
}

int FrequencyDivider_i::serviceFunction()
{
	bulkio::InFloatPort::dataTransfer *input = dataFloat_in->getPacket(bulkio::Const::BLOCKING);

	//Make sure input packet was received
	if (not input) {
		return NOOP;
	}

	if (input->sriChanged) {
		dataFloat_out->pushSRI(input->SRI);
	}

	m_currentSize = input->dataBuffer.size();

	if (m_currentSize == 0) {
		return NOOP;
    }

    if (m_lastSize != m_currentSize) {
    	resizeOutput();
    }

    unsigned int i;

    if (m_lastSize == 0) {
    	m_signLast = (input->dataBuffer[0] > 0 ? true : false);
    	i = 1;
    	m_outputData[0] = m_out;
    } else {
    	i = 0;
    }

    for ( ; i<m_outputData.size(); ++i) {
    	m_signCurrent = (input->dataBuffer[i] > 0 ? true : false);
    	if (m_signCurrent != m_signLast) { // A zero-cross
			if (++m_divisor == m_DIVISOR) { // Got 16 zero-crossings
				m_out *= -1;
				m_divisor = 0;
			}
		}
		m_outputData[i] = m_out;
		m_signLast = m_signCurrent;
    }


    dataFloat_out->pushPacket(m_outputData,input->T,input->EOS,input->streamID);
    m_lastSize = m_currentSize;

    delete input;

    return NORMAL;
}

void FrequencyDivider_i::resizeOutput()
{
	m_outputData.resize(m_currentSize);
}

void FrequencyDivider_i::propertyChangeListener(const std::string &id)
{
	if (id == "Divisor") {
		m_DIVISOR = Divisor;
		if (m_DIVISOR == 0) {
			m_DIVISOR = 1;
			LOG_WARN(FrequencyDivider_i,"Cannot divide by 0! Setting to default value 1.");
		}
		m_divisor = 0;
		m_signLast = m_signCurrent = false;
		m_out = 1;
	}
}
