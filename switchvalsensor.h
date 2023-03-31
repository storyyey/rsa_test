#ifndef SWITCHVALSENSOR_H
#define SWITCHVALSENSOR_H

#include "commonsensor.h"

class SwitchValSensor : public CommonSensor
{
public:
    SwitchValSensor();
    bool apply();
    void setDefConfig(SensorData::sensorType sensorType = SensorData::NON);
    bool presetIsModify(const QString &name);
    bool saveAsPreset(const QString &name);
    bool loadPreset(const QString &name);
};

#endif // SWITCHVALSENSOR_H
