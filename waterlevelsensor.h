#ifndef WATERLEVELSENSOR_H
#define WATERLEVELSENSOR_H

#include "commonsensor.h"

class WaterLevelSensor: public CommonSensor
{
public:
    WaterLevelSensor();
    bool apply();
    void setDefConfig(SensorData::sensorType sensorType = SensorData::NON);
    bool presetIsModify(const QString &name);
    bool saveAsPreset(const QString &name);
    bool loadPreset(const QString &name);
};

#endif // WATERLEVELSENSOR_H
