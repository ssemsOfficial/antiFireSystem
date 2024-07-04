#imports
from flask import Flask, jsonify
from flask_socketio import SocketIO,emit, send, Namespace
from flask_cors import CORS

from src.routes.userRouter import userRouter
from src.routes.hydrogenGroupRouter import HydrogenGroupRouter as hydrogenGroupRouter


#app
app = Flask(__name__)
CORS(app)  # Allow requests from http://127.0.0.1:5500
socketio =  SocketIO(app, cors_allowed_origins="*")

@socketio.on('message')
def handle_message(message, *args):
    print('Received message: ' + message)
    send('Response from server')
        
class MyCustomNamespace(Namespace):
    def on_connect(self):
        pass

    def on_disconnect(self):
        pass
    def on_message(self, message, *args):
        print('Received message:', message)
        self.send('Response from server by message')

    def on_custom_event(self, data, *args):
        print('Received custom event:', data)
        self.emit('response', 'a response from the server')
        # Handle the custom event logic here
    
    def on_event(self, data, *args):
        print('Received custom event:', data)
        self.send('response', 'Response to custom event')

        # Handle the custom event logic here
            
    #def on_message(self, message):
    #    print('Received message : ', message)
    #    self.send('Response from server')
            
    #def on_fire(self, message):
    #    print('Received message : ', message)
    #    self.send('Response from server')
    
                    


#routes
#http
app.register_blueprint(userRouter, url_prefix='/api/v1/user')

#ws
socketio.on_namespace(hydrogenGroupRouter('/hydrogen-group'))




if __name__ == "__main__":
    #app.run(debug=True, port=8000)
    socketio.run(app, debug=True, port=8000)