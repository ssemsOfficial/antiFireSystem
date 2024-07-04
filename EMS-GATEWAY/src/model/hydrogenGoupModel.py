from kafka import KafkaProducer
import json

# Configure Kafka producer
producer = KafkaProducer(
    bootstrap_servers=['localhost:9092'],  # Kafka broker address
    value_serializer=lambda x: json.dumps(x).encode('utf-8')  # Serialize message values as JSON
)

topic_name = 'test_topic'

def send_message(topic, message):
    producer.send(topic, message)
    #producer.flush()
    #producer.close()

class HydrogenGoupModel : 
    def __init__(self, onButtonSelectMode, onTemperature, motorGroup, onReserve, pump, onBattery, battery ) -> None:
        self.onButtonSelectMode 
        self.onTemperature 
        self.motorGroup 
        self.onReserve 
        self.pump 
        self.onBattery 
        self.battery
    
    #Events        
    @staticmethod
    def onSystemMode( data) :
        print('data : ', data)
        send_message(data['topic'], { 'system_mode' : data['value'] })   
    @staticmethod
    def onTemperature( data):
        print('data : ', data)
        send_message(data['topic'], { 'temperature' : data['value'] })
    @staticmethod
    def onReserve( data):
        print('data : ', data)
        send_message(data['topic'], { 'reserve' : data['value'] })     
    @staticmethod
    def onMode( data):
        print('data : ', data)
        send_message(data['topic'], { 'system_mode' : data['value'] })
    @staticmethod
    def onBatteryPower( data):
        print('data  : ', data)
        send_message(data['topic'], { 'battery_power' : data['value'] })    
    @staticmethod
    def onMotorGroup( data):
        print('data  : ', data)
        send_message(data['topic'], { 'motor_group' : data['value'] })
    @staticmethod
    def onPump( data):
        print('data', data)
        send_message(data['topic'], { 'pump' : data['value'] })
    @staticmethod
    def onBattery( data):
        print('data', data)
        send_message(data['topic'], { 'battery' : data['value'] })
            
            
            
    #setters and getters
    @staticmethod
    def getButtonSelectMode(self):
        return self.onButtonSelectMode
    @staticmethod
    def setButtonSelectMode(self, mode) :
        self.onButtonSelectMode = mode        
    @staticmethod
    def getTemperature(self) :
        return self.onTemperature
    @staticmethod
    def setTemperature(self, temperature) :
        self.onTemperature = temperature
    @staticmethod
    def getMotorGroup(self) :
        return self.motor
    @staticmethod
    def setMotorGroup(self, motorState) :
        self.motor = motorState     
    @staticmethod
    def getReserve(self) :
        return self.onReserve
    @staticmethod
    def setReserve(self, reserveState) :
        self.onReserve = reserveState
    @staticmethod
    def getPump(self) :
        return self.pump
    @staticmethod
    def setPump(self, motorState) :
        self.pump = motorState         
    @staticmethod
    def getBatteryCharger(self) :
        return self.onBattery
    @staticmethod
    def setBatteryCharger(self, batteryChargerState) :
        self.onBattery = batteryChargerState        
    @staticmethod
    def getBattery(self) :
        return self.battery
    @staticmethod
    def setBattery(self, batteryState) :
        self.battery = batteryState            
        
        