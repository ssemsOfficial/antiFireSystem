from src.model.hydrogenGoupModel import HydrogenGoupModel

#HydrogenGoupModel('updateButtonSelectMode', onTemperature, motorGroup, onReserve, pump, onBattery, battery)




class HydrogenGoupController :
    
    def updateSystemMode(self, data) :
        HydrogenGoupModel.onMode(data)
        
    def updateTemperature(self, data):
       # print(data['value'])
        HydrogenGoupModel.onTemperature(data)
    
    def updateReserve(self, data):
        HydrogenGoupModel.onReserve(data)

    def updateBatteryPower(self, data):
        HydrogenGoupModel.onBatteryPower(data)
        
    def updateMotorGroup(self, data):
        HydrogenGoupModel.onMotorGroup(data)
    
    def updatePump(self, data):
        HydrogenGoupModel.onPump( data)
    
    def updateBattery(self, data):
        HydrogenGoupModel.onBattery(data)    