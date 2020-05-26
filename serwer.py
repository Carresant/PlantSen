from flask import Flask
from flask import request
app = Flask(__name__)


@app.route('/')
def hello():
    return 'Hello world! <br> <form action="/measurements" method="post">  <label for="fname">First name:</label><br>  <input type="text" id="fname" name="fname" value="John"><br>  <input type="submit" value="Submit"> </form> '

@app.route('/measurements', methods=['POST'])
def save_measurements(): 
    print (request.form['fname'])
    return "ok"

#zmienić nazwe parametru fname na measurements
#zmienic tak zeby bylo widac 'received measurements:pomiar', tam gdzie Tonia 
#adres ip ifconfig, wyslac request z feathera zamiast tonia plantsen - dziala komunikacja 
#127.0.0.1-> mój adres ip
#wyslanie pomiaru zamiast received...:jsonfile
#...
#zmienic naglowki http aplikowac application/json



if __name__ == '__main__':
    app.run()



