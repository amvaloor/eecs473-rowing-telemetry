#include "rowing_imu.h"

namespace Rowing {

    IMU::IMU(int SDA, int SDC) : SDA(SDA), SDC(SDC) {}

    IMU::IMU() {SDA=21; SDC=22;}

    bool IMU::begin() {

        Wire.begin(SDA, SDC);
        Wire.setClock(100000);

        if (!imu.begin_I2C(0x4B, &Wire) && !imu.begin_I2C(0x4A, &Wire)) {
            return false;
        }

        imu.enableReport(SH2_ACCELEROMETER, 20000);
        imu.enableReport(SH2_GYROSCOPE_CALIBRATED, 20000);

        return true;
    }

    bool IMU::stop(){
        
        bool accStopped  = imu.enableReport(SH2_ACCELEROMETER, 0);
        bool gyroStopped = imu.enableReport(SH2_GYROSCOPE_CALIBRATED, 0);

        return accStopped && gyroStopped;
    }

    bool IMU::update(){

        bool newData = false;

        //You need to check if the packet has been updated, 
        //if it was then you drain it and mark true, else false

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

                default:
                    break;
            }
        }

        return newData;
    }

    float IMU::get_accX() const{
        return accX;
    }

    float IMU::get_accY() const{
        return accY;
    }

    float IMU::get_accZ() const{
        return accZ;
    }

    float IMU::get_gyroX()const{
        return gyroX;
    }

    float IMU::get_gyroY()const{
        return gyroY;
    }

    float IMU::get_gyroZ()const{
        return gyroZ;
    }

}

