import threading
from services.consumerService.HydrogenGroup1ConsumerService import ElectrogenGroup1ConsumerService
from services.consumerService.HydrogenGroup2ConsumerService import ElectrogenGroup2ConsumerService
from database.mongodb_writer import MongoDBWriter

def consume_and_write(consumer_service, mongo_writer):
    try:
        for message in consumer_service.consume_messages():
            mongo_writer.write_to_mongodb(message)
            print(f"Message written to MongoDB: {message}")
    except Exception as e:
        print(f"An error occurred: {e}")

def main() :
    try:
        electrogenGroup1ConsumerService = ElectrogenGroup1ConsumerService()
        mongo_electrogenGroup1_writer = MongoDBWriter(electrogenGroup1ConsumerService.getKafkaTopic())
        
        electrogenGroup2ConsumerService = ElectrogenGroup2ConsumerService()
        mongo_electrogenGroup2_writer = MongoDBWriter(electrogenGroup2ConsumerService.getKafkaTopic())
        
        #mongo_writer.listen()
        
        # Create threads for consuming and writing
        thread1 = threading.Thread(target=consume_and_write, args=(electrogenGroup1ConsumerService, mongo_electrogenGroup1_writer))
        thread2 = threading.Thread(target=consume_and_write, args=(electrogenGroup2ConsumerService, mongo_electrogenGroup2_writer))

        # Start the threads
        thread1.start()
        thread2.start()

        # Wait for threads to complete
        thread1.join()
        thread2.join()
    except Exception as e:
        print(f"An error occurred: {e}")
        
if __name__ == "__main__" :
    main()