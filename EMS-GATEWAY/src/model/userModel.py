class UserModel :
    def __init__(self, username, email):
        self.username = username
        self.email = email
    
    def serialize(self) : 
        return {
            'username' : self.username,
            'email' : self.email 
        }     