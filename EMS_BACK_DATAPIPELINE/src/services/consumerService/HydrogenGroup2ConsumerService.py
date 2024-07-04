from kafka import KafkaConsumer
import json

KAFKA_TOPIC = 'hydrogenGroup2'
KAFKA_BOOTSTRAP_SERVERS = ['localhost:9092']

class ElectrogenGroup2ConsumerService:
    KAFKA_TOPIC = 'hydrogenGroup2'
    KAFKA_BOOTSTRAP_SERVERS = ['localhost:9092']   
    
    def __init__(self):
        self.consumer = KafkaConsumer(
            KAFKA_TOPIC,
            bootstrap_servers=KAFKA_BOOTSTRAP_SERVERS,
            group_id='temperature_consumer_group',
            auto_offset_reset='latest',
            enable_auto_commit=True,
            value_deserializer=lambda x: json.loads(x.decode('utf-8'))
        )
    def getKafkaTopic(self):
        return KAFKA_TOPIC    
    def consume_messages(self):
        for message in self.consumer :
            yield message.value

# Example usage:
# consumer_service = KafkaConsumerService()
# for message in consumer_service.consume_messages():
#     print(message)
