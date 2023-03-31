#ifndef WATERPRESSSENSOR_H
#define WATERPRESSSENSOR_H

#include "commonsensor.h"

class WaterPressSensor : public CommonSensor
{
public:
    WaterPressSensor();
    bool apply();
    void setDefConfig(SensorData::sensorType sensorType = SensorData::NON);
    bool presetIsModify(const QString &name);
    bool saveAsPreset(const QString &name);
    bool loadPreset(const QString &name);
};

#endif // WATERPRESSSENSOR_H
