from kafka import KafkaProducer
import json

# Configure Kafka producer
producer = KafkaProducer(
    bootstrap_servers=['localhost:9092'],  # Kafka broker address
    value_serializer=lambda x: json.dumps(x).encode('utf-8')  # Serialize message values as JSON
)

topic_name = 'topic_name'

# Define a function to send messages to Kafka
def send_message(topic, message):
    producer.send(topic, message)
    print("Message sent to topic:", topic)

# Example usage: Send a message to a Kafka topic
# Replace 'topic_name' with the actual Kafka topic name
# Replace 'message_data' with the actual message data to send
send_message('topic_name', {'key': 'value'})

# Flush and close the producer
producer.flush()
producer.close()