from kafka import KafkaProducer
import json

# Configure Kafka producer
producer = KafkaProducer(
    bootstrap_servers=['localhost:9092'],  # Kafka broker address
    value_serializer=lambda x: json.dumps(x).encode('utf-8')  # Serialize message values as JSON
)

# Send messages to Kafka topic
topic_name = 'test_topic'

for i in range(10):
    message = {'message': f'Message {i}'}
    producer.send(topic_name, message)
    print(f"Sent message: {message}")

# Flush and close the producer
producer.flush()
producer.close()
