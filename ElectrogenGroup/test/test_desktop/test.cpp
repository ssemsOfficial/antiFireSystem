#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../lib/systems/batteryLevelSystem/BatteryLevelSystem.h"

// Define mock class for VoltageSensor0To25DC
class MockVoltageSensor : public VoltageSensor0To25DC {
public:
    MOCK_METHOD(float, getVoltageValue, (), (override));
};

TEST(BatteryLevelSystemTest, UpdateEventsHighVoltageTest) {
    // Create a mock object for VoltageSensor0To25DC
    MockVoltageSensor voltageSensorMock;

    // Create an instance of BatteryLevelSystem with the mock object
    BatteryLevelSystem batteryLevelSystem(voltageSensorMock, /* mock BatteryPowerSupplyRelay */);

    // Set the expected behavior of the mock
    EXPECT_CALL(voltageSensorMock, getVoltageValue())
        .WillOnce(::testing::Return(MIN_VOLTAGE_VALUE + 1.0)); // Voltage above threshold

    // Call the function to be tested
    batteryLevelSystem.updateEvents();

    // Check if the event is set to true (indicating high voltage)
    EXPECT_TRUE(batteryLevelSystem.events.OnBatteryLevel);
}

// Add more test cases as needed...

// Entry point for running the tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}