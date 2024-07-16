import telnetlib
import urllib

from pymongo import MongoClient 
from bson.objectid import ObjectId
import json



class AnimalShelter(object):
    """ CRUD operations for Animal collection in MongoDB """

    def __init__(self, username, password):
        # Initializing the MongoClient. This helps to 
        # access the MongoDB databases and collections. 
        self.client = MongoClient('mongodb://%s:%s@localhost:45335/test?authSource=AAC' % (urllib.parse.quote(username), urllib.parse.quote(password)))
        # where xxxxx is your unique port number
        self.database = self.client["AAC"]

    def insertDocument(self, insertDoc=dict()):
        if insertDoc is not None:
            self.database.animals.insert_one(insertDoc)  # data should be dictionary
            return True           
        else:
            print('Nothing to save, because data parameter is empty')
            return False
    
    def queryDocument(self, docQuery):
        if docQuery is not None:
            return self.database.animals.find(docQuery, {"_id":False})
            
        
        else:
            raise Exception('Nothing to read, because data parameter is empty')
            
            
    def updateDocument(self, findDocUpdate, updateDocUpdate):
        if findDocUpdate is not None:
            tempDoc = self.database.animals.update_many(findDocUpdate, {"$set":updateDocUpdate})
            return json.dumps(str(tempDoc) + " document update")

        else:
            print('Nothing to udate, because data parameter is empty')
            return False

    def deleteDocument(self, deleteDoc):
        if deleteDoc is not None:
            tempDoc = self.database.animals.delete_one(deleteDoc)
            return json.dumps(str(tempDoc) + " document deleted")

        else:
            print('Nothing to delete, because data parameter is empty')
            return False