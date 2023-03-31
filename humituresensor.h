#ifndef HUMITURESENSOR_H
#define HUMITURESENSOR_H

#include "commonsensor.h"

class HumitureSensor : public CommonSensor
{
public:
    HumitureSensor();
    bool apply();
    void setDefConfig(SensorData::sensorType sensorType = SensorData::NON);
    bool presetIsModify(const QString &name);
    bool saveAsPreset(const QString &name);
    bool loadPreset(const QString &name);
};

#endif // HUMITURESENSOR_H
