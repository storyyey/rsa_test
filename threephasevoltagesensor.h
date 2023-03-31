#ifndef THREEPHASEVOLTAGESENSOR_H
#define THREEPHASEVOLTAGESENSOR_H

#include "commonsensor.h"

class ThreephaseVoltageSensor: public CommonSensor
{
public:
    ThreephaseVoltageSensor();
    bool apply();
    void setDefConfig(SensorData::sensorType sensorType = SensorData::NON);
    bool presetIsModify(const QString &name);
    bool saveAsPreset(const QString &name);
    bool loadPreset(const QString &name);
};

#endif // THREEPHASEVOLTAGESENSOR_H
