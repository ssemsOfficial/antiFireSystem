from flask import Blueprint, jsonify
from flask_socketio import Namespace, send, emit

from src.controller.entities.HydrogenGoupController import HydrogenGoupController


controller = HydrogenGoupController()


class HydrogenGroupRouter(Namespace):
    def on_connect(self):
        print('client connected')
    def on_disconnect(self):
        print('client disconnected')
    def on_messages(self, messages):
        send('event')
        print('Received message : ', messages)
    
    def on_onMode(self, data, *args):
        controller.updateSystemMode(data)
    
    def on_onTemperature(self, data, *args):
        controller.updateTemperature(data)
    def on_onReserve(self, data, *args):
        controller.updateReserve(data)   
    def on_onBattery(self, data, *args):
        controller.updateBatteryPower(data)    
    
    def on_motorGroup(self, data, *args):
        controller.updateMotorGroup(data)
    def on_pump(self, data, *args):
        controller.updatePump(data)
    def on_battery(self, data, *args):
        controller.updateBattery(data)
    
