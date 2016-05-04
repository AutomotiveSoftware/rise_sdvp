/*
    Copyright 2012 Benjamin Vedder	benjamin@vedder.se

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include "carinfo.h"

CarInfo::CarInfo(int id, Qt::GlobalColor color)
{
    mId = id;
    mColor = color;
    mName = "";
    mName.sprintf("Car %d", mId);
}

int CarInfo::getId()
{
    return mId;
}

void CarInfo::setId(int id, bool changeName = false)
{
    mId = id;

    if (changeName) {
        mName = "";
        mName.sprintf("Car %d", mId);
    }
}

QString CarInfo::getName()
{
    return mName;
}

void CarInfo::setName(QString name)
{
    mName = name;
}

void CarInfo::setLocation(LocPoint &point)
{
    mLocation = point;
}

LocPoint CarInfo::getLocationGps()
{
    return mLocationGps;
}

void CarInfo::setLocationGps(LocPoint &point)
{
    mLocationGps = point;
}

Qt::GlobalColor CarInfo::getColor()
{
    return mColor;
}

void CarInfo::setColor(Qt::GlobalColor color)
{
    mColor = color;
}

LocPoint CarInfo::getLocation()
{
    return mLocation;
}

