from src.controller.entities.userController import UserController 
from flask import Blueprint, jsonify

controller = UserController()

userRouter = Blueprint('userRouter', __name__)

@userRouter.route('/', methods=['GET'])
def get_all_users():
    users = controller.get_users() 
    return jsonify(users)
