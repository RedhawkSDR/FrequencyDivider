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

#ifndef FREQUENCYDIVIDER_IMPL_H
#define FREQUENCYDIVIDER_IMPL_H

#include "FrequencyDivider_base.h"

class FrequencyDivider_i;

class FrequencyDivider_i : public FrequencyDivider_base
{
    ENABLE_LOGGING
    public:
        FrequencyDivider_i(const char *uuid, const char *label);
        ~FrequencyDivider_i();
        int serviceFunction();
    private:
        int m_divisor;
        int m_DIVISOR;
        float m_out;
        bool m_signCurrent, m_signLast;
        std::vector<float> m_outputData;
        unsigned int m_currentSize;
        unsigned int m_lastSize;
        void resizeOutput();
        void propertyChangeListener(const std::string &);
};

#endif
