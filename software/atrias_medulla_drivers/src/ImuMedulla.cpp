#include "atrias_medulla_drivers/ImuMedulla.h"

namespace atrias {
namespace medullaDrivers {

ImuMedulla::ImuMedulla() : Medulla() {
	// Inputs
	pdoEntryDatas[0] = {1, (void**) &command};
	pdoEntryDatas[1] = {2, (void**) &counter};

	// Outputs
	pdoEntryDatas[2] = {1, (void**) &id};
	pdoEntryDatas[3] = {1, (void**) &state};
	pdoEntryDatas[4] = {1, (void**) &timingCounter};
	pdoEntryDatas[5] = {1, (void**) &errorFlags};
	pdoEntryDatas[6] = {4, (void**) &gyrX};
	pdoEntryDatas[7] = {4, (void**) &gyrY};
	pdoEntryDatas[8] = {4, (void**) &gyrZ};
	pdoEntryDatas[9] = {4, (void**) &accX};
	pdoEntryDatas[10] = {4, (void**) &accY};
	pdoEntryDatas[11] = {4, (void**) &accZ};
	pdoEntryDatas[12] = {1, (void**) &status};
	pdoEntryDatas[13] = {1, (void**) &seq};
	pdoEntryDatas[14] = {2, (void**) &temperature};
	pdoEntryDatas[15] = {4, (void**) &crc};
}

PDORegData ImuMedulla::getPDORegData() {
    return {MEDULLA_IMU_RX_PDO_COUNT, MEDULLA_IMU_TX_PDO_COUNT, pdoEntryDatas};
};

void ImuMedulla::postOpInit() {
	// Initialize pitch according to gravity.
    pitch = (3.0*M_PI/2.0) + (M_PI/4.0 - atan2(-(*accX), -(*accY)));
}

uint8_t ImuMedulla::getID() {
    return *id;
}

void ImuMedulla::processIMU(RTT::os::TimeService::nsecs deltaTime, atrias_msgs::robot_state &robotState) {
	// TODO: Update this with Ryan's kinemagics.

    // DEBUG
    log(RTT::Info) << "IMU pitch: " << pitch << RTT::endlog();

	// Update pitch with negative of gyro Z because pitch vector points in the
	// opposite direction of the IMU Z vector.
	pitch -= *gyrZ;

	// Update robot state.
	robotState.position.imuPitch = pitch;
	robotState.position.imuPitchVelocity = pitch / (((double) deltaTime) / ((double) SECOND_IN_NANOSECONDS));
}

void ImuMedulla::processTransmitData(atrias_msgs::controller_output& controller_output) {
    *counter = ++local_counter;
    *command = controller_output.command;
}

void ImuMedulla::processReceiveData(atrias_msgs::robot_state& robot_state) {
    // If we don't have new data, don't run. It's pointless, and results in
    // NaN velocities.
    if (*timingCounter == timingCounterValue)
        return;

    // Calculate how much time has elapsed since the previous sensor readings.
    // Note: % isn't actually a modulo, hence the additional 256.
    RTT::os::TimeService::nsecs deltaTime =
        ((((int16_t) *timingCounter) + 256 - ((int16_t) timingCounterValue)) % 256) *
        CONTROLLER_LOOP_PERIOD_NS;
    timingCounterValue = *timingCounter;

    processIMU(deltaTime, robot_state);
}

} // medullaDrivers
} // atrias

// vim: noexpandtab
