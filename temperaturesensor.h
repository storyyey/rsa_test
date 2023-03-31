#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "commonsensor.h"

class TemperatureSensor : public CommonSensor
{
public:
    TemperatureSensor();
    bool apply();
    void setDefConfig(SensorData::sensorType sensorType = SensorData::NON);
    bool presetIsModify(const QString &name);
    bool saveAsPreset(const QString &name);
    bool loadPreset(const QString &name);
};

#endif // TEMPERATURESENSOR_H
