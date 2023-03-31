#ifndef RESIDUALELECTRICITYSENSOR_H
#define RESIDUALELECTRICITYSENSOR_H

#include "commonsensor.h"

class ResidualElectricitySensor : public CommonSensor
{
public:
    ResidualElectricitySensor();
    bool apply();
    void setDefConfig(SensorData::sensorType sensorType = SensorData::NON);
    bool presetIsModify(const QString &name);
    bool saveAsPreset(const QString &name);
    bool loadPreset(const QString &name);
};

#endif // RESIDUALELECTRICITYSENSOR_H
