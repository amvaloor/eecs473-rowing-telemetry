#include "rowing_imu.h"
#include <cmath>

namespace Rowing {

    IMU::IMU(int SDA, int SDC) 
        : SDA(SDA), SDC(SDC), 
          accX(0.0f), accY(0.0f), accZ(0.0f),
          gyroX(0.0f), gyroY(0.0f), gyroZ(0.0f),
          sweep(0.0f), elevation(0.0f), feather(0.0f) {}

    IMU::IMU() 
        : SDA(21), SDC(22), 
          accX(0.0f), accY(0.0f), accZ(0.0f),
          gyroX(0.0f), gyroY(0.0f), gyroZ(0.0f),
          sweep(0.0f), elevation(0.0f), feather(0.0f) {}

    bool IMU::begin() {
        Wire.begin(SDA, SDC);
        Wire.setClock(100000);

        if (!imu.begin_I2C(0x4B, &Wire) && !imu.begin_I2C(0x4A, &Wire)) {
            return false;
        }

        // Enable sensor reports (default 20,000 microseconds / 50 Hz interval)
        bool accOK   = imu.enableReport(SH2_ACCELEROMETER, 20000);
        bool gyroOK  = imu.enableReport(SH2_GYROSCOPE_CALIBRATED, 20000);
        bool quatOK  = imu.enableReport(SH2_ROTATION_VECTOR, 20000);

        return accOK && gyroOK && quatOK;
    }

    bool IMU::stop() {
        bool accStopped  = imu.enableReport(SH2_ACCELEROMETER, 0);
        bool gyroStopped = imu.enableReport(SH2_GYROSCOPE_CALIBRATED, 0);
        bool quatStopped = imu.enableReport(SH2_ROTATION_VECTOR, 0);

        return accStopped && gyroStopped && quatStopped;
    }

    bool IMU::update() {
        bool newData = false;

        while (imu.getSensorEvent(&sensorValue)) {
            newData = true;

            switch (sensorValue.sensorId) {
                case SH2_ACCELEROMETER:
                    accX = sensorValue.un.accelerometer.x;
                    accY = sensorValue.un.accelerometer.y;
                    accZ = sensorValue.un.accelerometer.z;
                    break;

                case SH2_GYROSCOPE_CALIBRATED:
                    gyroX = sensorValue.un.gyroscope.x;
                    gyroY = sensorValue.un.gyroscope.y;
                    gyroZ = sensorValue.un.gyroscope.z;
                    break;

                case SH2_ROTATION_VECTOR:
                    currentQuat.w = sensorValue.un.rotationVector.real;
                    currentQuat.x = sensorValue.un.rotationVector.i;
                    currentQuat.y = sensorValue.un.rotationVector.j;
                    currentQuat.z = sensorValue.un.rotationVector.k;
                    calculateOarAngles();
                    break;

                default:
                    break;
            }
        }

        return newData;
    }

    void IMU::tare() {
        // Inverse (conjugate) of current unit quaternion
        refQuatInv.w =  currentQuat.w;
        refQuatInv.x = -currentQuat.x;
        refQuatInv.y = -currentQuat.y;
        refQuatInv.z = -currentQuat.z;

        calculateOarAngles();
    }

    void IMU::calculateOarAngles() {
        // Relative Quaternion: Q_rel = refQuatInv * currentQuat
        float w = refQuatInv.w * currentQuat.w - refQuatInv.x * currentQuat.x - refQuatInv.y * currentQuat.y - refQuatInv.z * currentQuat.z;
        float x = refQuatInv.w * currentQuat.x + refQuatInv.x * currentQuat.w + refQuatInv.y * currentQuat.z - refQuatInv.z * currentQuat.y;
        float y = refQuatInv.w * currentQuat.y - refQuatInv.x * currentQuat.z + refQuatInv.y * currentQuat.w + refQuatInv.z * currentQuat.x;
        float z = refQuatInv.w * currentQuat.z + refQuatInv.x * currentQuat.y - refQuatInv.y * currentQuat.x + refQuatInv.z * currentQuat.w;

        // Yaw (Sweep angle across boat horizon)
        float siny_cosp = 2.0f * (w * z + x * y);
        float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
        sweep = std::atan2(siny_cosp, cosy_cosp) * (180.0f / M_PI);

        // Pitch (Elevation / Blade height)
        float sinp = 2.0f * (w * y - z * x);
        if (std::abs(sinp) >= 1.0f) {
            elevation = std::copysign(90.0f, sinp);
        } else {
            elevation = std::asin(sinp) * (180.0f / M_PI);
        }

        // Roll (Feathering angle)
        float sinr_cosp = 2.0f * (w * x + y * z);
        float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
        feather = std::atan2(sinr_cosp, cosr_cosp) * (180.0f / M_PI);
    }

    float IMU::get_sweep() const {
        return sweep;
    }

    float IMU::get_elevation() const {
        return elevation;
    }

    float IMU::get_feather() const {
        return feather;
    }

    float IMU::get_accX() const {
        return accX;
    }

    float IMU::get_accY() const {
        return accY;
    }

    float IMU::get_accZ() const {
        return accZ;
    }

    float IMU::get_gyroX() const {
        return gyroX;
    }

    float IMU::get_gyroY() const {
        return gyroY;
    }

    float IMU::get_gyroZ() const {
        return gyroZ;
    }

    Quaternion IMU::get_quaternion() const {
        return currentQuat;
    }

} // namespace Rowing