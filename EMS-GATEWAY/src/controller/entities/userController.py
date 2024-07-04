from src.model.userModel import UserModel as User


class UserController : 
    
    def get_users(self):
        users = [
            User('user1', 'user1@example.com'),
            User('user2', 'user2@example.com')
        ]
        return [user.serialize() for user in users]