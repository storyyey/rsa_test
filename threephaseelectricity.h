#ifndef THREEPHASEELECTRICITY_H
#define THREEPHASEELECTRICITY_H

#include "commonsensor.h"

class ThreephaseElectricity: public CommonSensor
{
public:
    ThreephaseElectricity();
    bool apply();
    void setDefConfig(SensorData::sensorType sensorType = SensorData::NON);
    bool presetIsModify(const QString &name);
    bool saveAsPreset(const QString &name);
    bool loadPreset(const QString &name);
};

#endif // THREEPHASEELECTRICITY_H
