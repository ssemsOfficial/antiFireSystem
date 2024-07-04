from kafka import KafkaConsumer


# Configure Kafka consumer
consumer = KafkaConsumer(
    'topic_name',  # Specify the Kafka topic to consume from
    bootstrap_servers=['localhost:9092'],  # Kafka broker address
    group_id='my_consumer_group',  # Consumer group ID
    auto_offset_reset='latest',  # Start consuming from the latest offset
    enable_auto_commit=True,  # Enable auto commit of consumed messages
    value_deserializer=lambda x: x.decode('utf-8')  # Decode message values as strings
)

def processNotification(notification) :
    print("recieved : ", notification)
    
for message in consumer:
    processNotification(message.value)   
    
consumer.close()     