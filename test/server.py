import json
import os

from flask import Flask, Response, request

app = Flask(__name__)

@app.route("/animals", methods=["GET", "POST"])
def animal_handler():
    animals = {"cat": "meo meo", "dog": "gâu gâu"}

    if request.method == "POST":
        new_animal = request.get_json()
        animals.append(new_animal)
        
    return Response(
        response=json.dumps(animals),
        content_type="application/json",
    )

@app.route("/favbook", methods=["GET"])
def get_fav_book():
    if request.headers['X-Api-Key'] == 'blobla':
        return Response(response="Harry Porter")
    else:
        return "Access denied", 401 

if __name__ == "__main__":
    app.run(port=3000)
