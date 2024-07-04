from kafka import KafkaConsumer

# Configure Kafka consumer
consumer = KafkaConsumer(
    'test_topic',  # Specify the Kafka topic to consume from
    bootstrap_servers=['localhost:9092'],  # Kafka broker address
    group_id='my_consumer_group',  # Consumer group ID
    auto_offset_reset='earliest',  # Start consuming from the earliest offset
    value_deserializer=lambda x: x.decode('utf-8')  # Decode message values as strings
)

# Consume messages from Kafka topic
for message in consumer:
    print(f"Received message: {message.value}")

# Close the consumer
consumer.close()
