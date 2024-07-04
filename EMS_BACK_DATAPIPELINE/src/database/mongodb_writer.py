import pymongo


MONGODB_URI =  'mongodb://localhost:27017/?readPreference=primary'
MONGODB_DB_NAME = 'test'
MONGODB_COLLECTION_NAME = 'electrogengroups'

class MongoDBWriter :
    def __init__(self, system_id) :
        self.client = pymongo.MongoClient(MONGODB_URI)
        self.db = self.client[MONGODB_DB_NAME]
        self.collection = self.db[MONGODB_COLLECTION_NAME]
        self.system_id = system_id 

    def write_to_mongodb(self, data):
        query = {"system_id" : self.system_id}
        update = {"$set" : data}
        self.collection.update_one(query, update, upsert=True)

    def listen(self):
        pipeline = [{'$match': {'operationType': 'update'}}]  # Match only update operations
        change_stream = self.collection.watch(pipeline)
        for change in change_stream :
            print('the update is : ', change)
# Example usage:
# mongo_writer = MongoDBWriter()
# data = {"key": "value"}
# mongo_writer.write_to_mongodb(data)
