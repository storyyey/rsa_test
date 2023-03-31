#ifndef THREEPHASESWITCHSENSOR_H
#define THREEPHASESWITCHSENSOR_H

#include "commonsensor.h"

class ThreephaseSwitchSensor: public CommonSensor
{
public:
    ThreephaseSwitchSensor();
    bool apply();
    void setDefConfig(SensorData::sensorType sensorType = SensorData::NON);
    bool presetIsModify(const QString &name);
    bool saveAsPreset(const QString &name);
    bool loadPreset(const QString &name);
};

#endif // THREEPHASESWITCHSENSOR_H
